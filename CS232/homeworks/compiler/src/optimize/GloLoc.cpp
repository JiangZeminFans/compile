#include "GloLoc.h"

//�Һ��
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

//�ҵݹ麯��
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
    if (module->global_list_.size() > 10) //���У��йغܶ�Ĳ���Ҫ�ľֲ�
        return; 
    funcEntry = module->getMainFunc();
    initfuncSuc();
    std::set<Function*> visited;
    initfuncRec(funcEntry, visited);
    //�ҵ���Ӧȫ�ֱ�����ʹ�ú���
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
        //���õ�ȫ�ֱ���
        if (funcVar[var].empty()) 
        {
            iter = module->global_list_.erase(iter);
#ifdef DBG
            printf("ɾ����ȫ�ֱ�����%s\n", var->name_.c_str());
#endif
            continue;
        }
        else if (funcVar[var].size() == 1) 
        {
            auto func = *(funcVar[var].begin());
            //ȫ�ֱ���ֻ��main������ʹ���Ҳ��ݹ�����Ծֲ���
            if (func == funcEntry && !funcRec.count(func)) 
            {
                auto contained = dynamic_cast<Pointer_Type*>(var->type_)->contained_;
                //��������ֲ���
                if (contained->tid_ != Type::ArrayTyID) {
                    auto entryBB = func->basic_blocks_[0];
                    auto alloca = new Alloca_Instruction(contained, entryBB, false); //����ָ�����������飬���趨parent
                    auto store = new Store_Instruction(var->init_val_, alloca, entryBB, false);
                    entryBB->add_instruction_front(store);      //���Ų���
                    entryBB->add_instruction_front(alloca);
                    var->replace_all_use_with(alloca);
                    iter = module->global_list_.erase(iter);
#ifdef DBG
                    printf("�ֲ���ȫ�ֱ�����%s\n", var->name_.c_str());
#endif
                    continue;
                }
            }
        }
        iter++;
    }
}
