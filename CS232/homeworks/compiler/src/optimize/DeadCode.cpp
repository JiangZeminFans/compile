#include "DeadCode.h"
#include "ConstPro.h"

std::set<std::string> markedFunc = { "getint", "getfloat", "getch", "getarray", "getfarray", "putint", "putfloat",
                                    "putch", "putarray", "putfarray", "_sysy_starttime", "_sysy_stoptime", "memcpy",
                                    "memclr", "memset", "llvm.memset.p0.i32", "__aeabi_memcpy4", "__aeabi_memclr4",
                                    "__aeabi_memset4" };

void DeadCodeEli::initFuncPArg() 
{
    for (auto& func : module->function_list_) 
    {
        // 若为空
        if (func->basic_blocks_.empty()) 
            continue;
        for (auto arg : func->arguments_) 
        {
            // 指针
            if (arg->type_->tid_ == Type::PointerTyID) 
            {
                if (!funcPArg.count(func)) 
                {
                    funcPArg.insert({ func, {} });
                }
                funcPArg[func].insert(arg);
            }
        }
    }
}

//判断关键函数，构造待使用store指令
void DeadCodeEli::initPre() 
{
    storeIns.clear();
    for (auto bb : func_->basic_blocks_) 
    {
        for (auto ins : bb->instr_list_) 
        {
            if (ins->op_id_ == Instruction::GetElementPtr) 
            {
                //TODO 待加
            }
            else if (ins->op_id_ == Instruction::Store) 
            {
                if (!storeIns.count(ins->get_operand(1))) 
                {
                    storeIns.insert({ ins->get_operand(1), {} });
                }
                storeIns[ins->get_operand(1)].push_back(ins);
                if (dynamic_cast<Global_Variable*>(ins->get_operand(1))) 
                    markedFunc.insert(func_->name_);
                if (dynamic_cast<GetElementPtr_Instruction*>(ins->get_operand(1))) 
                    markedFunc.insert(func_->name_);
                if (funcPArg[func_].count(ins->get_operand(1))) 
                    markedFunc.insert(func_->name_);
            }
            else if (ins->op_id_ == Instruction::Call) 
            {
                auto f = ins->get_operand(ins->operands_.size() - 1);
                if (markedFunc.count(f->name_)) 
                    markedFunc.insert(func_->name_);
            }
        }
    }
}

//判断指令是否关键
bool DeadCodeEli::isCrit(Instruction* ins) 
{
    if (ins->op_id_ == Instruction::Ret) 
    {
        retBB = ins->parent_;
        return true;
    }
    else if (ins->op_id_ == Instruction::Call) 
    {
        auto f = ins->get_operand(ins->operands_.size() - 1);
        return markedFunc.count(f->name_);
    }
    else if (ins->op_id_ == Instruction::Store) 
    {
        //虽然main函数中的某些指令即使修改了全局变量也不一定关键
        //存储了全局变量或者getelementptr指令的地址（即数组）
        //全局变量和数组有可能被函数的外部访问
        if (dynamic_cast<Global_Variable*>(ins->get_operand(1))) 
            return true;
        if (dynamic_cast<GetElementPtr_Instruction*>(ins->get_operand(1))) 
            return true;
        if (funcPArg[func_].count(ins->get_operand(1))) //函数参数
            return true; 
        return false;
    }
    return false;
}

// 标记
void DeadCodeEli::mark() 
{
    std::list<Value*> workList;
    for (auto bb : func_->basic_blocks_) 
    {
        for (auto ins : bb->instr_list_) 
        {
            if (isCrit(ins)) 
            {
                markedIns.insert(ins);
                workList.push_back(ins);
            }
        }
    }
    while (!workList.empty()) 
    {
        auto ins = dynamic_cast<Instruction*>(workList.back());
        workList.pop_back();
        if (ins == nullptr) 
        {
            std::cout << "non-instruction found in workList!" << std::endl;
            continue;
        }
        for (auto operand : ins->operands_) 
        { 
            //遍历操作数
            auto temp = dynamic_cast<Instruction*>(operand);
            if (!temp) continue;
            if (markedIns.insert(temp).second) 
            {
                workList.push_back(temp); //还未被标记，则插入workList
            }
        }
        //如果该语句为PHI，检测跳转语句
        if (ins->op_id_ == Instruction::PHI) 
        {
            for (int i = 1; i < ins->operands_.size(); i += 2) 
            {
                auto bb = dynamic_cast<Basic_Block*>(ins->get_operand(i));
                auto br = bb->get_terminator();
                //还未被标记，则插入workList
                if (markedIns.insert(br).second) 
                {
                    workList.push_back(br);
                }
            }
        }
        //若为地址，则对应的store保留
        if (storeIns.count(ins)) 
        {
            for (auto instruction : storeIns[ins]) 
            {
                if (markedIns.insert(instruction).second) 
                {
                    workList.push_back(instruction);
                }
            }
            storeIns.erase(ins);
        }
        //插入一次该块的逆支配边界结束的跳转语句
        if (markedBB.insert(ins->parent_).second) 
        { //说明该块的插入过了，不需要重复遍历
            for (auto RFrontier : ins->parent_->rdom_frontier_) 
            {
                auto t = RFrontier->get_terminator();
                if (markedIns.insert(t).second) 
                {
                    workList.push_back(t);
                }
            }
        }
    }
}

//删除或替换未被标记的指令
void DeadCodeEli::sweepIns() 
{
    int delCnt = 0, chgCnt = 0;
    for (auto bb : func_->basic_blocks_) 
    {
        std::vector<Instruction*> ins2Del;
        for (auto ins : bb->instr_list_) 
        {
            if (!markedIns.count(ins)) 
            { //没有被标记
                if (ins->op_id_ != Instruction::Br) 
                {
                    ins2Del.push_back(ins); //不是跳转语句则删
                }
                else {
                    if (ins->operands_.size() == 3) 
                    { 
                        //是分支语句
                        ++chgCnt;
                        auto trueBB = dynamic_cast<Basic_Block*>(ins->get_operand(1));
                        auto falseBB = dynamic_cast<Basic_Block*>(ins->get_operand(2));
                        trueBB->remove_pre_basic_block(bb);
                        falseBB->remove_pre_basic_block(bb);
                        bb->remove_succ_basic_block(trueBB);
                        bb->remove_succ_basic_block(falseBB);
                        Basic_Block* temp = retBB; //bb->rdoms_里不含retBB
                        std::vector<Basic_Block*> rdoms(bb->rdoms_.begin(), bb->rdoms_.end());
                        std::sort(rdoms.begin(),rdoms.end(),[=](Basic_Block* x, Basic_Block* y) -> bool 
                            {
                                return x->rdoms_.count(y);
                            });//找最近的逆支配节点
                        for (auto rdbb : rdoms) 
                        {
                            if (rdbb != bb && markedBB.count(rdbb)) 
                            {
                                temp = rdbb;
                                break;
                            }
                        }
                        //修改为跳转
                        ins->remove_operands(0, 2); //删除原来的操作数
                        ins->num_ops_ = 1;
                        ins->operands_.resize(1);
                        ins->use_pos_.resize(1);
                        ins->set_operand(0, temp);
                        bb->add_succ_basic_block(temp);
                        temp->add_pre_basic_block(bb);
                    }
                }
            }
        }
        delCnt += ins2Del.size();
        for (auto ins : ins2Del) 
        {
            bb->delete_instr(ins);
        }
    }
#ifdef DBG
    if (delCnt + chgCnt != 0)
        printf("删除指令:%d条\t改变指令：%d条\n", delCnt, chgCnt);
#endif
}

void DeadCodeEli::execute() 
{
    RDTree RDTree(module);
    RDTree.execute();
    initFuncPArg();
    for (auto& func : module->function_list_) 
    {
        if (func->basic_blocks_.empty())
            continue;
        func_ = func;
        initPre();
        mark();
        sweepIns();
        // TODO Help Dlt_Unused_BB(func_);
    }
}
