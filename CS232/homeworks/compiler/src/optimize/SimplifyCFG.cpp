
#include "SimplifyCFG.h"

//执行函数
void SimplifyCFG::execute()
{
//    std::cout << "SimplifyCFG:begin" << std::endl << std::endl;
//    std::cout << module->print() << std::endl;
    for (auto func : module->function_list_)
    {   //遍历函数表
        if (func->basic_blocks_.empty()) continue;

        func_ = func;
        removeNoPreBB();
        mergeSinglePreBB();
        eliSinglePrePhi();
        eliSingleJumpBB();
    }
//    std::cout << "SimplifyCFG:end" << std::endl << std::endl;
//    std::cout << module->print() << std::endl;
}

//删除没有前驱的基本块
void SimplifyCFG::removeNoPreBB()
{
    std::vector<Basic_Block *> toDelete;
    //从第三块开始遍历，第1块为入口，第2块为ret块，不做处理

    for (int i = 2; i < func_->basic_blocks_.size(); i++)
    {
        auto bb = func_->basic_blocks_[i];
        if (bb->pre_bbs_.empty()) {
            toDelete.push_back(bb);
            //没有前驱，只有可能在phi语句中使用该基本块，删除对应的phi指令操作数
            for (auto use : bb->use_list_)
            {
                auto ins = dynamic_cast<PhiInst *>(use.val);
                int idx = use.num;

                if (ins == nullptr) continue;

                ins->remove_operands(idx - 1, idx);
            }
        }
    }

    for (auto bb: toDelete)
        func_->remove_bb(bb);
    
#ifdef DBG
    if (toDelete.size())
        printf("函数%-24s删除无前驱块:%d块\n", func_->name_.c_str(), toDelete.size());
#endif
}

//如果仅有一个前驱且该前驱仅有一个后继，将此基本块与前驱合并
void SimplifyCFG::mergeSinglePreBB()
{
    std::vector<Basic_Block *> toDelete;
    //从第三块开始遍历，第1块为入口，第2块为ret块，不做处理
    for (int i = 2; i < func_->basic_blocks_.size(); i++)
    {
        auto bb = func_->basic_blocks_[i];
        if (bb->pre_bbs_.size() != 1) continue; //不止一个前驱

        auto preBB = *bb->pre_bbs_.begin();
        auto preBr = preBB->get_terminator();

        if (preBB->succ_bbs_.size() != 1) continue; //前驱不止一个后继

        preBB->delete_instr(preBr);
        
        for (auto iter = bb->instr_list_.begin(); iter != bb->instr_list_.end();)
        {
            auto ins = *iter++;
            bb->remove_instr(ins);
            preBB->add_instruction(ins);
        }

        //后面func_->remove_bb(bb)会解除相对bb的关系，这里只需要增加新关系即可，但是会导致无法连续合并
        preBB->remove_succ_basic_block(bb);

        for (auto succBB : bb->succ_bbs_)
        {
            preBB->add_succ_basic_block(succBB);
            succBB->remove_pre_basic_block(bb);
            succBB->add_pre_basic_block(preBB);
        }
        bb->replace_all_use_with(preBB);
        toDelete.push_back(bb);
    }
    for (auto bb : toDelete)
        func_->remove_bb(bb);
    
#ifdef DBG
    if (toDelete.size())
        printf("函数%-24s合并基本块:%d块\n", func_->name_.c_str(), toDelete.size());
#endif
}

//消除只有一个前驱的基本块的PHI指令
void SimplifyCFG::eliSinglePrePhi()
{
    int cnt = 0;
    for (auto bb: func_->basic_blocks_)
    {
        if (bb->pre_bbs_.size() != 1) continue; //不止一个前驱
        
        for (auto iter = bb->instr_list_.begin(); iter != bb->instr_list_.end();)
        {
            auto ins = *iter++;

            if (ins->is_phi())
            {
                ins->replace_all_use_with(ins->get_operand(0));
                bb->delete_instr(ins);
                cnt++;
            }
        }
    }
#ifdef DBG
    if (cnt)
        printf("函数%-24s消除phi指令:%d条\n", func_->name_.c_str(), cnt);
#endif
}

//判断纯跳转block能否删除
bool eliminableJumpBB(Basic_Block *bb)
{
    auto target = bb->get_terminator()->get_operand(0);

    for (auto preBB : bb->pre_bbs_)
    {
        auto br = preBB->get_terminator();

        if (br->operands_.size() == 1) continue;

        auto trueBB = br->get_operand(1);
        auto falseBB = br->get_operand(2);

        //前驱不能分支跳转到bb的后继，不然bb后继的phi会出错
        if (trueBB == target || falseBB == target) return false;
    }
    return true;
}

//消除仅包含无条件分支的基本块
void SimplifyCFG::eliSingleJumpBB()
{
    std::vector<Basic_Block *> toDelete;

    //从第三块开始遍历，第1块为入口，第2块为ret块，不做处理
    for (int i = 2; i < func_->basic_blocks_.size(); i++)
    {
        auto bb = func_->basic_blocks_[i];

        if (bb->instr_list_.size() != 1) continue; //不止一条语句

        auto br = bb->get_terminator();

        if (br->operands_.size() != 1 || br->is_ret()) continue; //是分支或者ret

        if (!eliminableJumpBB(bb)) continue;
        toDelete.push_back(bb);

        auto target = dynamic_cast<Basic_Block *>(br->get_operand(0));

        //处理phi指令，可能增加操作数
        for (auto ins : target->instr_list_)
        {
            if (!ins->is_phi()) continue;

            for (int i = 1; i < ins->operands_.size(); i += 2)
            {
                if (ins->get_operand(i) == bb)
                {
                    auto val = ins->get_operand(i - 1);
                    ins->remove_operands(i - 1, i);

                    for (auto preBB : bb->pre_bbs_)
                    {
                        ins->add_operand(val);
                        ins->add_operand(preBB);
                    }
//                    ins->get_operand(i)->remove_use(ins->use_pos_[i]);
//                    ins->set_operand(i, bb->pre_bbs_[0]);
//                    for (int i = 1; i < bb->pre_bbs_.size(); i++) {
//                        ins->add_operand(val);
//                        ins->add_operand(bb->pre_bbs_[i]);
//                    }
                    break;
                }
            }
        }

        bb->replace_all_use_with(target); 
        //phi指令的use已经被替换，再替换所有跳转指令

        //后面func_->remove_bb(bb)会解除相对bb的关系，这里只需要增加新关系即可
        for (auto preBB : bb->pre_bbs_)
        {
            preBB->add_succ_basic_block(target);
            target->add_pre_basic_block(preBB);
        }
    }
    for (auto bb : toDelete)    func_->remove_bb(bb);
    
#ifdef DBG
    if (toDelete.size())
        printf("函数%-24s删除纯跳转块:%d块\n", func_->name_.c_str(), toDelete.size());
#endif
}
