#include "ConstPro.h"
/*�����۵��ж�*/

// ����int32�ĳ����۵�
Constant_Int* ConstFolder::computeINT(Instruction::OpID op, Constant_Int* value1, Constant_Int* value2) {
    int const_value1 = value1->value_ , const_value2 = value2->value_;
    switch (op) 
    {
        case Instruction::Add:
            return new Constant_Int(module_->int32_ty_, const_value1 + const_value2);
        case Instruction::Sub:
            return new Constant_Int(module_->int32_ty_, const_value1 - const_value2);
        case Instruction::Mul:
            return new Constant_Int(module_->int32_ty_, const_value1 * const_value2);
        case Instruction::SDiv:
            return new Constant_Int(module_->int32_ty_, const_value1 / const_value2);
        case Instruction::SRem:
            return new Constant_Int(module_->int32_ty_, const_value1 % const_value2);
        default:
            return nullptr;
    }
}

// int32�ĳ����۵�
Constant_Int* ConstFolder::computeCMP(ICmp_Instruction::ICmp_Op op, Constant_Int* value1, Constant_Int* value2)
{
    int lhs = value1->value_, rhs = value2->value_;
    switch (op)
    {
    case ICmp_Instruction::ICMP_EQ:
        return new Constant_Int(module_->int1_ty_, lhs == rhs);
    case ICmp_Instruction::ICMP_NE:
        return new Constant_Int(module_->int1_ty_, lhs != rhs);
    case ICmp_Instruction::ICMP_SGT:
        return new Constant_Int(module_->int1_ty_, lhs > rhs);
    case ICmp_Instruction::ICMP_SGE:
        return new Constant_Int(module_->int1_ty_, lhs >= rhs);
    case ICmp_Instruction::ICMP_SLE:
        return new Constant_Int(module_->int1_ty_, lhs <= rhs);
    case ICmp_Instruction::ICMP_SLT:
        return new Constant_Int(module_->int1_ty_, lhs < rhs);
    default:
        return nullptr;
    }
}

// �ж�int�����Ƿ�Ϊ������������Ƿ���nullptr
Constant_Int* judgeConstInt(Value* value)
{
    auto const_int_ptr = dynamic_cast<Constant_Int*>(value);
    if (const_int_ptr)
    {
        return const_int_ptr;
    }
    else
    {
        return nullptr;
    }
}


// ����float�ĳ����۵�
Constant_Float* ConstFolder::computeFLOAT(Instruction::OpID op, Constant_Float* value1, Constant_Float* value2) 
{
    float const_value1 = value1->value_ , const_value2 = value2->value_;;
    switch (op) 
    {
        case Instruction::FAdd:
            return new Constant_Float(module_->float32_ty_, const_value1 + const_value2);
        case Instruction::FSub:
            return new Constant_Float(module_->float32_ty_, const_value1 - const_value2);
        case Instruction::FMul:
            return new Constant_Float(module_->float32_ty_, const_value1 * const_value2);
        case Instruction::FDiv:
            return new Constant_Float(module_->float32_ty_, const_value1 / const_value2);
        default:
            return nullptr;
    }
}


// float�ĳ����۵�
Constant_Int* ConstFolder::computeFCMP(FCmp_Instruction::FCmp_Op op, Constant_Float* value1, Constant_Float* value2) 
{
    float lhs = value1->value_;
    float rhs = value2->value_;
    switch (op)
    {
        case FCmp_Instruction::FCMP_UEQ:
            return new Constant_Int(module_->int1_ty_, lhs == rhs);
        case FCmp_Instruction::FCMP_UNE:
            return new Constant_Int(module_->int1_ty_, lhs != rhs);
        case FCmp_Instruction::FCMP_UGT:
            return new Constant_Int(module_->int1_ty_, lhs > rhs);
        case FCmp_Instruction::FCMP_UGE:
            return new Constant_Int(module_->int1_ty_, lhs >= rhs);
        case FCmp_Instruction::FCMP_ULE:
            return new Constant_Int(module_->int1_ty_, lhs <= rhs);
        case FCmp_Instruction::FCMP_ULT:
            return new Constant_Int(module_->int1_ty_, lhs < rhs);
        default:
            return nullptr;
    }
}

// �ж�float�����Ƿ�Ϊ������������Ƿ���nullptr
Constant_Float* judgeConstFloat(Value* value) 
{
    auto const_float_ptr = dynamic_cast<Constant_Float*>(value);
    if (const_float_ptr) 
    {
        return const_float_ptr;
    }
    else 
    {
        return nullptr;
    }
}




/*�ж�bb���Ĳ�����*/
bool changed = false;

void ConstPro::execute() 
{
    for (auto func : module->function_list_)
        func->set_instr_name();
    folder = new ConstFolder(module);
    for (auto fun : module->function_list_) 
    {
        if (fun->basic_blocks_.empty())
            continue;
        changed = true;
        while (changed) 
        {
            changed = false;
            Const_Fold(fun);
            Handle_Br(fun);
            // TODO Help.h Dlt_Unused_BB(fun); //ɾ������BB
        }
    }
}

void ConstPro::Const_Fold(Function* func) 
{
    dltList.clear();
    for (auto bb : func->basic_blocks_) 
    {
        globalInt.clear();
        globalFloat.clear();
        for (auto instr : bb->instr_list_) 
        {
            if (instr->op_id_ == Instruction::Add || instr->op_id_ == Instruction::Sub ||
                instr->op_id_ == Instruction::Mul || instr->op_id_ == Instruction::SDiv ||
                instr->op_id_ == Instruction::SRem)
            {
                auto v1 = judgeConstInt(instr->get_operand(0));
                auto v2 = judgeConstInt(instr->get_operand(1));
                if (v1 && v2) 
                {
                    auto fold_const = folder->computeINT(instr->op_id_, v1, v2);
                    instr->replace_all_use_with(fold_const);
                    dltList.insert({ instr, bb });
                }
            }
            else if (instr->op_id_ == Instruction::FAdd || instr->op_id_ == Instruction::FSub ||
                instr->op_id_ == Instruction::FMul || instr->op_id_ == Instruction::FDiv)
            {
                auto f1 = judgeConstFloat(instr->get_operand(0));
                auto f2 = judgeConstFloat(instr->get_operand(1));
                if (f1 && f2) 
                {
                    auto fold_const = folder->computeFLOAT(instr->op_id_, f1, f2);
                    instr->replace_all_use_with(fold_const);
                    dltList.insert({ instr, bb });
                }
            }
            else if (instr->op_id_ == Instruction::FPtoSI) 
            {
                auto tmp = judgeConstFloat(instr->get_operand(0));
                if (tmp) 
                {
                    auto fold_const = new Constant_Int(module->int32_ty_, (int)(tmp->value_));
                    instr->replace_all_use_with(fold_const);
                    dltList.insert({ instr, bb });
                }
            }
            else if (instr->op_id_ == Instruction::SItoFP) 
            {
                auto tmp = judgeConstInt(instr->get_operand(0));
                if (tmp) 
                {
                    auto fold_const = new Constant_Float(module->float32_ty_, (float)(tmp->value_));
                    instr->replace_all_use_with(fold_const);
                    dltList.insert({ instr, bb });
                }
            }
            else if (instr->op_id_ == Instruction::ICmp) 
            {
                auto v1 = judgeConstInt(instr->get_operand(0));
                auto v2 = judgeConstInt(instr->get_operand(1));
                if (v1 && v2) 
                {
                    auto fold_const = folder->computeCMP(dynamic_cast<ICmp_Instruction*>(instr)->icmp_op_, v1, v2);
                    instr->replace_all_use_with(fold_const);
                    dltList.insert({ instr, bb });
                }
            }
            else if (instr->op_id_ == Instruction::FCmp) 
            {
                auto f1 = judgeConstFloat(instr->get_operand(0));
                auto f2 = judgeConstFloat(instr->get_operand(1));
                if (f1 && f2) 
                {
                    auto fold_const = folder->computeFCMP(dynamic_cast<FCmp_Instruction*>(instr)->fcmp_op_, f1, f2);
                    instr->replace_all_use_with(fold_const);
                    dltList.insert({ instr, bb });
                }
            }
            else if (instr->op_id_ == Instruction::ZExt) 
            {
                auto tmp = judgeConstInt(instr->get_operand(0));
                if (tmp) 
                {
                    auto fold_const = new Constant_Int(module->int32_ty_, (int)(tmp->value_));
                    instr->replace_all_use_with(fold_const);
                    dltList.insert({ instr, bb });
                }
            }
            else if (instr->op_id_ == Instruction::Call) 
            {
                globalInt.clear();
                globalFloat.clear();
            }
            else if (instr->op_id_ == Instruction::Load) 
            {
                //����ȫ�ֱ���Ϊ����ʱ���۵�
                auto global = dynamic_cast<Global_Variable*>(instr->get_operand(0));
                if (global) 
                {
                    auto iterINT = globalInt.find(global);
                    auto iterFloat = globalFloat.find(global);

                    if (iterINT != globalInt.end()) 
                    {
                        instr->replace_all_use_with(iterINT->second);
                        dltList.insert({ instr, bb });
                    }
                    else if (iterFloat != globalFloat.end()) 
                    {
                        instr->replace_all_use_with(iterFloat->second);
                        dltList.insert({ instr, bb });
                    }
                }
                else if (dynamic_cast<Alloca_Instruction*>(instr->get_operand(0))) 
                {
                    auto adrs = dynamic_cast<Alloca_Instruction*>(instr->get_operand(0));
                    if (adrs->alloca_ty_->tid_ == Type::IntegerTyID) 
                    {
                        auto iterINT = globalInt.find(adrs);
                        if (iterINT != globalInt.end()) 
                        {
                            instr->replace_all_use_with(iterINT->second);
                            dltList.insert({ instr, bb });
                        }
                    }
                    else if (adrs->alloca_ty_->tid_ == Type::FloatTyID) 
                    {
                        auto iterFloat = globalFloat.find(adrs);
                        if (iterFloat != globalFloat.end()) 
                        {
                            instr->replace_all_use_with(iterFloat->second);
                            dltList.insert({ instr, bb });
                        }
                    }
                }
            }
            else if (instr->op_id_ == Instruction::Store) 
            {
                auto lval = instr->get_operand(1);
                auto rval_i = judgeConstInt(instr->get_operand(0));
                auto rval_f = judgeConstFloat(instr->get_operand(0));

                // �洢��int����
                if (rval_i) 
                {
                    auto iter = globalInt.find(lval);
                    if (iter != globalInt.end()) 
                    {
                        //�����ֵһ��������ɾ��
                        if (iter->second->value_ == rval_i->value_)
                            dltList.insert({ instr, bb });
                        else
                            iter->second = rval_i;
                    }
                    else
                        globalInt.insert({ lval, rval_i });
                }
                //�洢��float����
                else if (rval_f) 
                {
                    auto iter = globalFloat.find(lval);
                    if (iter != globalFloat.end()) 
                    {
                        //�����ֵһ��������ɾ��
                        if (iter->second->value_ == rval_f->value_)
                            dltList.insert({ instr, bb });
                        else
                            iter->second = rval_f;
                    }
                    else
                        globalFloat.insert({ lval, rval_f });
                }
                //�洢�˱���
                else 
                {
                    auto iterINT = globalInt.find(lval);
                    auto iterFloat = globalFloat.find(lval);
                    if (iterINT != globalInt.end())
                        globalInt.erase(iterINT);

                    if (iterFloat != globalFloat.end())
                        globalFloat.erase(iterFloat);
                }
            }
        }
    }
    // ɾ��dltList��ָ��
    for (auto iter = dltList.begin(); iter != dltList.end(); iter++) 
    {
        changed = true;
        iter->second->delete_instr(iter->first);
    }
}


// ����ָ��
void ConstPro::Handle_Br(Function* func) 
{
    for (auto bb : func->basic_blocks_) 
    {
        //��ȡ����������һ��ָ��
        auto br = bb->get_terminator();
        if (!br) 
            continue;
        if (br->op_id_ == Instruction::Br && dynamic_cast<Branch_Instruction*>(br)->num_ops_ == 3) 
        {
            auto cond = judgeConstInt(br->get_operand(0));
            auto truebb = br->get_operand(1);
            auto falsebb = br->get_operand(2);
            if (!cond)
                continue; //�������ʽ���ǳ���������һ��������
            changed = true;
            //����false��֧��ɾ��true��֧
            if (cond->value_ == 0) 
            {
                bb->delete_instr(br);
                for (auto succ_bb : bb->succ_bbs_) 
                {
                    //���к�̿�������bb����ϵ
                    succ_bb->remove_pre_basic_block(bb);

                    if (succ_bb != falsebb) 
                    {
                        //�����̿���phiָ�����bb�����
                        // TODO ����phiָ�� Handle_Phi(bb, succ_bb);
                    }
                }
                //���bb�ĺ��
                bb->succ_bbs_.clear();
                //�����µķ�ָ֧��������̻Ὣfalsebb��bb���Ӳ��һὫָ����ӵ�ָ���б���
                new Branch_Instruction(dynamic_cast<Basic_Block*>(falsebb), bb);
            }
            //����true��֧��ɾ��false��֧
            else {
                bb->delete_instr(br);
                for (auto succ_bb : bb->succ_bbs_) 
                {
                    //���к�̿�������bb����ϵ
                    succ_bb->remove_pre_basic_block(bb);

                    if (succ_bb != truebb) 
                    {
                        //�����̿���phiָ�����bb�����
                        //TODO Handle_Phi(bb, succ_bb); //����phiָ��
                    }
                }
                //���bb�ĺ��
                bb->succ_bbs_.clear();
                //�����µķ�ָ֧��������̻Ὣfalsebb��bb���Ӳ��һὫָ����ӵ�ָ���б���
                //TODO new BranchInst(dynamic_cast<Basic_Block*>(truebb), bb); 
         
            }
        }
    }
    return;
}
