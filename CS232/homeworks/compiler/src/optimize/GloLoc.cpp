#include "GloLoc.h"

//找后继
void GloLoc::initfuncSuc() 
{
    for (auto func : module->function_list_) 
    {
        for (auto use : func->use_list_) 
        {
            auto call = dynamic_cast<Call_Instruction*>(use.val);
            auto funcUse = call->parent_->parent_;
            funcSuc[funcUse].insert(func);
        }
    }
}

//找递归函数
void GloLoc::initfuncRec(Function* cur, std::set<Function*>& visited) 
{
    visited.insert(cur);
    for (auto succ : funcSuc[cur]) 
    {
        if (visited.find(succ) != visited.end()) 
        {
            funcRec.insert(succ);
        }
        else 
        {
            initfuncRec(succ, visited);
        }
    }
    visited.erase(cur);
}

void GloLoc::execute() 
{
    if (module->global_list_.size() > 10) //特判，有关很多的不需要改局部
        return; 
    funcEntry = module->getMainFunc();
    initfuncSuc();
    std::set<Function*> visited;
    initfuncRec(funcEntry, visited);
    //找到对应全局变量的使用函数
    for (auto var : module->global_list_) 
    {
        for (auto use : var->use_list_) 
        {
            auto ins = dynamic_cast<Instruction*>(use.val);
            auto funcUse = ins->parent_->parent_;
            funcVar[var].insert(funcUse);
        }
    }
    for (auto iter = module->global_list_.begin(); iter != module->global_list_.end();) 
    {
        auto var = *iter;
        //无用的全局变量
        if (funcVar[var].empty()) 
        {
            iter = module->global_list_.erase(iter);
#ifdef DBG
            printf("删除了全局变量：%s\n", var->name_.c_str());
#endif
            continue;
        }
        else if (funcVar[var].size() == 1) 
        {
            auto func = *(funcVar[var].begin());
            //全局变量只在main函数中使用且不递归则可以局部化
            if (func == funcEntry && !funcRec.count(func)) 
            {
                auto contained = dynamic_cast<Pointer_Type*>(var->type_)->contained_;
                //不将数组局部化
                if (contained->tid_ != Type::ArrayTyID) {
                    auto entryBB = func->basic_blocks_[0];
                    auto alloca = new Alloca_Instruction(contained, entryBB, false); //创建指令，不插入基本块，但设定parent
                    auto store = new Store_Instruction(var->init_val_, alloca, entryBB, false);
                    entryBB->add_instruction_front(store);      //反着插入
                    entryBB->add_instruction_front(alloca);
                    var->replace_all_use_with(alloca);
                    iter = module->global_list_.erase(iter);
#ifdef DBG
                    printf("局部化全局变量：%s\n", var->name_.c_str());
#endif
                    continue;
                }
            }
        }
        iter++;
    }
}
