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
        // ��Ϊ��
        if (func->basic_blocks_.empty()) 
            continue;
        for (auto arg : func->arguments_) 
        {
            // ָ��
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

//�жϹؼ������������ʹ��storeָ��
void DeadCodeEli::initPre() 
{
    storeIns.clear();
    for (auto bb : func_->basic_blocks_) 
    {
        for (auto ins : bb->instr_list_) 
        {
            if (ins->op_id_ == Instruction::GetElementPtr) 
            {
                //TODO ����
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

//�ж�ָ���Ƿ�ؼ�
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
        //��Ȼmain�����е�ĳЩָ�ʹ�޸���ȫ�ֱ���Ҳ��һ���ؼ�
        //�洢��ȫ�ֱ�������getelementptrָ��ĵ�ַ�������飩
        //ȫ�ֱ����������п��ܱ��������ⲿ����
        if (dynamic_cast<Global_Variable*>(ins->get_operand(1))) 
            return true;
        if (dynamic_cast<GetElementPtr_Instruction*>(ins->get_operand(1))) 
            return true;
        if (funcPArg[func_].count(ins->get_operand(1))) //��������
            return true; 
        return false;
    }
    return false;
}

// ���
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
            //����������
            auto temp = dynamic_cast<Instruction*>(operand);
            if (!temp) continue;
            if (markedIns.insert(temp).second) 
            {
                workList.push_back(temp); //��δ����ǣ������workList
            }
        }
        //��������ΪPHI�������ת���
        if (ins->op_id_ == Instruction::PHI) 
        {
            for (int i = 1; i < ins->operands_.size(); i += 2) 
            {
                auto bb = dynamic_cast<Basic_Block*>(ins->get_operand(i));
                auto br = bb->get_terminator();
                //��δ����ǣ������workList
                if (markedIns.insert(br).second) 
                {
                    workList.push_back(br);
                }
            }
        }
        //��Ϊ��ַ�����Ӧ��store����
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
        //����һ�θÿ����֧��߽��������ת���
        if (markedBB.insert(ins->parent_).second) 
        { //˵���ÿ�Ĳ�����ˣ�����Ҫ�ظ�����
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

//ɾ�����滻δ����ǵ�ָ��
void DeadCodeEli::sweepIns() 
{
    int delCnt = 0, chgCnt = 0;
    for (auto bb : func_->basic_blocks_) 
    {
        std::vector<Instruction*> ins2Del;
        for (auto ins : bb->instr_list_) 
        {
            if (!markedIns.count(ins)) 
            { //û�б����
                if (ins->op_id_ != Instruction::Br) 
                {
                    ins2Del.push_back(ins); //������ת�����ɾ
                }
                else {
                    if (ins->operands_.size() == 3) 
                    { 
                        //�Ƿ�֧���
                        ++chgCnt;
                        auto trueBB = dynamic_cast<Basic_Block*>(ins->get_operand(1));
                        auto falseBB = dynamic_cast<Basic_Block*>(ins->get_operand(2));
                        trueBB->remove_pre_basic_block(bb);
                        falseBB->remove_pre_basic_block(bb);
                        bb->remove_succ_basic_block(trueBB);
                        bb->remove_succ_basic_block(falseBB);
                        Basic_Block* temp = retBB; //bb->rdoms_�ﲻ��retBB
                        std::vector<Basic_Block*> rdoms(bb->rdoms_.begin(), bb->rdoms_.end());
                        std::sort(rdoms.begin(),rdoms.end(),[=](Basic_Block* x, Basic_Block* y) -> bool 
                            {
                                return x->rdoms_.count(y);
                            });//���������֧��ڵ�
                        for (auto rdbb : rdoms) 
                        {
                            if (rdbb != bb && markedBB.count(rdbb)) 
                            {
                                temp = rdbb;
                                break;
                            }
                        }
                        //�޸�Ϊ��ת
                        ins->remove_operands(0, 2); //ɾ��ԭ���Ĳ�����
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
        printf("ɾ��ָ��:%d��\t�ı�ָ�%d��\n", delCnt, chgCnt);
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
