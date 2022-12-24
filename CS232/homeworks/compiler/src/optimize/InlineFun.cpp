#include "InlineFun.h"
extern int id;
extern std::map<Instruction::OpID, std::string> instr_id2string_;

void InlineFun::execute() 
{
    initfunSuc();
    initrecFunc(); //ֻ�в��ݹ������
    for (auto iter = module->function_list_.begin(); iter != module->function_list_.end();) {
        auto func = *iter;
        //ѭ��������
        if (recFunc.count(func) || func->basic_blocks_.empty() || func->basic_blocks_.size() > 10 ||func->name_ == "main") 
        {
            iter++;
            continue;
        }
        int cnt = func->use_list_.size();
        for (auto it = func->use_list_.begin(); it != func->use_list_.end();) 
        {
            auto call = *it++;
            inlineFunc(dynamic_cast<Call_Instruction*>(call.val));
        }
        iter = module->function_list_.erase(iter);
#ifdef DBG
        printf("��������:%s��%d��\n", func->name_.c_str(), cnt);
#endif
    }
}


//�Һ��
void InlineFun::initfunSuc() 
{
    for (auto func : module->function_list_) 
    {
        for (auto use : func->use_list_) 
        {
            auto call = dynamic_cast<Call_Instruction*>(use.val);
            auto useFunc = call->parent_->parent_;
            funSuc[useFunc].insert(func);
        }
    }
}

//�ҵݹ麯��
void InlineFun::initrecFunc() 
{
    std::set<Function*> judged, visited;
    for (auto func : module->function_list_) 
    {
        initrecFunc(func, visited, judged);
    }
}

//�ҵݹ麯��
void InlineFun::initrecFunc(Function* cur, std::set<Function*>& visited, std::set<Function*>& judged) 
{
    visited.insert(cur);
    if (!judged.count(cur)) 
    { //��û���жϹ�
        for (auto succ : funSuc[cur]) 
        {
            if (visited.find(succ) != visited.end()) 
            {
                recFunc.insert(succ);
            }
            else 
            {
                initrecFunc(succ, visited, judged);
            }
        }
    }
    judged.insert(cur);
    visited.erase(cur);
}


//�滻����������Ĳ������������еĲ��������ܲ�����ǰ��ָ������������ȫ�ֱ�����������
Value* transOperand(std::map<Value*, Value*>& old2NewIns, Value* old) 
{
    auto iter = old2NewIns.find(old);
    if (iter != old2NewIns.end()) 
        return iter->second;
    return old;
}

//������ͬ��ָ��ɲ�����ʹ���²������滻
Instruction* insFactory(Instruction* ins, std::map<Value*, Value*>& old2NewIns, std::map<Basic_Block*, Basic_Block*>& old2NewBB) 
{
    Instruction* dup = nullptr;
    std::vector<Value*> args;
    switch (ins->op_id_) 
    {
    case Instruction::Br:
        if (ins->num_ops_ == 3) 
        {
            dup = new Branch_Instruction(transOperand(old2NewIns, ins->get_operand(0)),
                old2NewBB[(Basic_Block*)ins->get_operand(1)],
                old2NewBB[(Basic_Block*)ins->get_operand(2)],
                old2NewBB[ins->parent_]);
        }
        else 
        {
            dup = new Branch_Instruction(old2NewBB[(Basic_Block*)ins->get_operand(0)],
                old2NewBB[ins->parent_]);
        }
        break;
    case Instruction::Add:
    case Instruction::Sub:
    case Instruction::Mul:
    case Instruction::SDiv:
    case Instruction::SRem:
    case Instruction::FAdd:
    case Instruction::FSub:
    case Instruction::FMul:
    case Instruction::FDiv:
        dup = new Binary_Instruction(ins->type_,
            ins->op_id_,
            transOperand(old2NewIns, ins->get_operand(0)),
            transOperand(old2NewIns, ins->get_operand(1)),
            old2NewBB[ins->parent_]);
        break;
    case Instruction::Alloca:
        dup = new Alloca_Instruction(((Alloca_Instruction*)ins)->alloca_ty_, old2NewBB[ins->parent_]);
        break;
    case Instruction::Load:
        dup = new Load_Instruction(transOperand(old2NewIns, ins->get_operand(0)),old2NewBB[ins->parent_]);
        break;
    case Instruction::Store:
        dup = new Store_Instruction(transOperand(old2NewIns, ins->get_operand(0)),
            transOperand(old2NewIns, ins->get_operand(1)),
            old2NewBB[ins->parent_]);
        break;
    case Instruction::GetElementPtr:
        for (int i = 1; i < ins->operands_.size(); i++) 
        {
            args.push_back(transOperand(old2NewIns, ins->get_operand(i)));
        }
        dup = new GetElementPtr_Instruction(transOperand(old2NewIns, ins->get_operand(0)),
            args,
            old2NewBB[ins->parent_]);
        break;
    case Instruction::ZExt:
        dup = new Zext_Instruction(ins->op_id_,
            transOperand(old2NewIns, ins->get_operand(0)),
            ins->type_,
            old2NewBB[ins->parent_]);
        break;
    case Instruction::FPtoSI:
        dup = new FpToSi_Instruction(ins->op_id_,
            transOperand(old2NewIns, ins->get_operand(0)),
            ins->type_,
            old2NewBB[ins->parent_]);
        break;
    case Instruction::SItoFP:
        dup = new SiToFp_Instruction(ins->op_id_,
            transOperand(old2NewIns, ins->get_operand(0)),
            ins->type_,
            old2NewBB[ins->parent_]);
        break;
    case Instruction::BitCast:
        dup = new Bitcast(ins->op_id_,
            transOperand(old2NewIns, ins->get_operand(0)),
            ins->type_,
            old2NewBB[ins->parent_]);
        break;
    case Instruction::ICmp:
        dup = new ICmp_Instruction(((ICmp_Instruction*)ins)->icmp_op_,
            transOperand(old2NewIns, ins->get_operand(0)),
            transOperand(old2NewIns, ins->get_operand(1)),
            old2NewBB[ins->parent_]);
        break;
    case Instruction::FCmp:
        dup = new FCmp_Instruction(((FCmp_Instruction*)ins)->fcmp_op_,
            transOperand(old2NewIns, ins->get_operand(0)),
            transOperand(old2NewIns, ins->get_operand(1)),
            old2NewBB[ins->parent_]);
        break;
    case Instruction::Call:
        for (int i = 0; i < ins->operands_.size() - 1; i++) 
        {
            args.push_back(transOperand(old2NewIns, ins->get_operand(i)));
        }
        dup = new Call_Instruction((Function*)ins->get_operand(ins->operands_.size() - 1),
            args,
            old2NewBB[ins->parent_]);
        break;
    case Instruction::Ret:
        std::cout << "ret��Ӧ�������" << std::endl;
        break;
    case Instruction::PHI:
        std::cout << "����Ӧ������phiָ��ǰִ��" << std::endl;
        break;
    case Instruction::FNeg:
    case Instruction::UDiv:
    case Instruction::URem:
    case Instruction::Shl:
    case Instruction::LShr:
    case Instruction::AShr:
    case Instruction::And:
    case Instruction::Or:
    case Instruction::Xor:
        std::cout << instr_id2string_[ins->op_id_] << "ָ���û���ɹ�" << std::endl;
        break;
    }
    return dup;
}

void InlineFun::inlineFunc(Call_Instruction* call) 
{
    auto callBB = call->parent_;
    auto callFunc = call->parent_->parent_;
    auto funcCalled = dynamic_cast<Function*>(call->get_operand(call->operands_.size() - 1));
    auto splitBB = new Basic_Block(module, std::to_string(id++), callFunc);
    Value* newRetVal = nullptr; //��������ֵ���滻����ԭ��call��䷵��ֵ��ʹ�õĵط������ǵĺ���Ӧ��ֻ��һ��ret��䣩
    bool moveTo = false;
    //call�����ָ��ȫ���Ƶ�splitBB
    for (auto iter = callBB->instr_list_.begin(); iter != callBB->instr_list_.end();) 
    {
        auto ins = *iter++;
        if (moveTo) 
        {
            callBB->remove_instr(ins);
            ins->parent_ = splitBB;
            splitBB->add_instruction(ins);
        }
        else if (ins == call) 
        {
            moveTo = true;
        }
    }
    //��ǰ������
    for (auto succBB : callBB->succ_bbs_) 
    {
        succBB->remove_pre_basic_block(callBB);
        succBB->add_pre_basic_block(splitBB);
        splitBB->add_succ_basic_block(succBB);
    }
    callBB->succ_bbs_.clear();

    std::map<Value*, Value*> old2NewIns; //����ָ�ʵ�Σ���ָ���Ӧ����Ϊָ��ͬʱҲ�ǲ�������
    std::map<Basic_Block*, Basic_Block*> old2NewBB;

    //�β�ʵ�ζ�Ӧ
    for (int i = 0; i < funcCalled->arguments_.size(); i++) 
    {
        old2NewIns[funcCalled->arguments_[i]] = call->get_operand(i);
    }
    //���ƻ�����
    for (auto oldBB : funcCalled->basic_blocks_) 
    {
        auto newBB = new Basic_Block(module, std::to_string(id++), callFunc);
        old2NewBB[oldBB] = newBB;
    }
    //����dfs��ָ��˳����������鸴��ָ���ȻǶ�������滻������ʱ�������
    std::list<Basic_Block*> stackOldBB;
    std::set<Basic_Block*> visitedOldBB; //ֻ�ø���һ�ξ͹���
    stackOldBB.push_back(funcCalled->basic_blocks_[0]);
    visitedOldBB.insert(funcCalled->basic_blocks_[0]);
    while (!stackOldBB.empty()) 
    {
        auto oldBB = stackOldBB.back();
        stackOldBB.pop_back();
        for (auto oldIns : oldBB->instr_list_) 
        {
            //����Ƿ��أ�ֱ������splitBB,ע�����÷���ֵ
            if (oldIns->is_ret()) 
            {
                auto newBB = old2NewBB[oldBB];
                if (newRetVal != nullptr) 
                {
                    std::cout << "���󣺴��ڶ��ret��䣡" << std::endl;
                    return;
                }
                new Branch_Instruction(splitBB, newBB); //ret��Ϊbr�������splitBB
                //���з���ֵ����Ҫ���÷���ֵ
                if (!oldIns->operands_.empty())
                {
                    newRetVal = old2NewIns[oldIns->operands_[0]];
                    call->replace_all_use_with(newRetVal); //ʹ��ԭ����call����ֵ�ĵط������·���ֵ���滻
                }
                callBB->delete_instr(call);
                new Branch_Instruction(old2NewBB[funcCalled->basic_blocks_[0]], callBB); //callBB��callָ��ȥ������Ϊ��ת����һ��������
                newBB->delete_instr(oldIns); //ɾ��ԭ��ret���
            }
            else 
            {
                Instruction* newIns = insFactory(oldIns, old2NewIns, old2NewBB);
                old2NewIns[oldIns] = newIns;
                //������תָ�����תָ���������ת�Ļ�������ջ
                if (oldIns->op_id_ == Instruction::Br) 
                {
                    if (oldIns->operands_.size() == 1) 
                    {
                        auto nextBB = (Basic_Block*)oldIns->get_operand(0);
                        if (!visitedOldBB.count(nextBB)) 
                        {
                            stackOldBB.push_back(nextBB);
                            visitedOldBB.insert(nextBB);
                        }
                    }
                    else if (oldIns->operands_.size() == 3) 
                    {
                        auto trueBB = (Basic_Block*)oldIns->get_operand(1);
                        auto falseBB = (Basic_Block*)oldIns->get_operand(2);
                        if (!visitedOldBB.count(trueBB)) 
                        {
                            stackOldBB.push_back(trueBB);
                            visitedOldBB.insert(trueBB);
                        }
                        if (!visitedOldBB.count(falseBB)) 
                        {
                            stackOldBB.push_back(falseBB);
                            visitedOldBB.insert(falseBB);
                        }
                    }
                }
            }
        }
    }
    //���ɴ���»�����ɾ��. ���ɴ���û�и���ָ���ɾ���ᱨ��
    for (auto& iter : old2NewBB) 
    {
        if (!visitedOldBB.count(iter.first)) 
        {
            callFunc->remove_bb(iter.second);
        }
    }
}

void InlineFun::delUselessJump() 
{
    for (auto func : module->function_list_) 
    {
        // �� �˳�
        if (func->basic_blocks_.empty()) 
            continue;
        auto retBB = func->getRetBB();
        int cnt = 0;
        std::list<Basic_Block*> stack;
        std::set<Basic_Block*> visited;
        stack.push_back(retBB);
        visited.insert(retBB);
        while (!stack.empty()) 
        {
            auto curBB = stack.back();
            stack.pop_back();
            //ֻ��һ����תָ�ֻ��һ��ǰ���飬��һ���Ż�
            if (curBB->instr_list_.size() == 1 && curBB->instr_list_.back()->is_br() && curBB->pre_bbs_.size() == 1) 
            {
                auto branch = curBB->instr_list_.back();
                auto preBB = curBB->pre_bbs_[0];
                //��ǰ����������ת
                if (curBB->succ_bbs_.size() == 1) 
                {
                    auto succBB = dynamic_cast<Basic_Block*>(branch->get_operand(0));
                    func->remove_bb(curBB); //�Ƴ�ǰ����̿����curBB�Ĺ�ϵ
                    preBB->add_succ_basic_block(succBB);
                    succBB->add_pre_basic_block(preBB);
                    auto preBranch = preBB->instr_list_.back();
                    //�޸�ǰ����תָ���Ӧ����ת��
                    for (int i = 0; i < preBranch->operands_.size(); i++) 
                    { //phi������
                        if (preBranch->get_operand(i) == curBB) 
                        {
                            preBranch->operands_[i]->remove_use(preBranch->use_pos_[i]); //ȡ��curBB�ڴ�ָ���use
                            preBranch->set_operand(i, succBB);
                        }
                    }
                    curBB->replace_all_use_with(preBB);
                    cnt++;
                }
                //��ǰ����������ת��ǰ��������������ת���滻ǰ����ָ��
                else if (preBB->succ_bbs_.size() == 1) 
                {
                    auto trueBB = dynamic_cast<Basic_Block*>(branch->get_operand(1));
                    auto falseBB = dynamic_cast<Basic_Block*>(branch->get_operand(2));
                    func->remove_bb(curBB); //�Ƴ�ǰ����̿����curBB�Ĺ�ϵ
                    trueBB->add_pre_basic_block(preBB);
                    falseBB->add_pre_basic_block(preBB);
                    preBB->add_succ_basic_block(trueBB);
                    preBB->add_succ_basic_block(falseBB);
                    curBB->remove_instr(branch);
                    preBB->delete_instr(preBB->instr_list_.back());
                    preBB->add_instruction(branch);
                    curBB->replace_all_use_with(preBB);
                    cnt++;
                }
            }
            for (auto BB : curBB->pre_bbs_) 
            {
                if (visited.insert(BB).second) 
                {
                    stack.push_back(BB);
                }
            }
        }
#ifdef DBG
        if (cnt)
            printf("����%-24sɾ��������ת�飺%d��\n", func->name_.c_str(), cnt);
#endif
    }
}
