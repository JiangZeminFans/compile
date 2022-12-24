#include "Pass.h"
#include "ComSubExprEli.h"

#include <string.h>
#include <set>
#include <algorithm>

//执行函数
void ComSubExprEli::execute()
{
    for(auto func: module->function_list_)
        func->set_instr_name();
    
    for (auto fun : module->function_list_)
    {
        if (fun->basic_blocks_.empty()) continue;

        ALL.clear();
        gen.clear();
        IN.clear();
        OUT.clear();
        dlt_instr.clear();

        expr_cnt = 0;

        //std::cout << "Get_AllExpr" << std::endl;
        Get_AllExpr(fun);

        //std::cout << "Get_AvailExpr" << std::endl;
        Get_AvailExpr(fun);

        //std::cout << "Dlt_ComSubExpr" << std::endl;
        Dlt_ComSubExpr(fun);

        //std::cout << "Dlt_Unused_Inst" << std::endl;
        Dlt_Unused_Inst(fun);
    }
}

void ComSubExprEli::Get_AllExpr(Function *func)
{
    //求出所有表达式，存在ALL里
    for (auto bb : func->basic_blocks_)
    {
        //遍历每一条指令
        for (auto instr : bb->instr_list_)
        {
            int op_id = instr->op_id_;

            if(op_id == Instruction::Add || op_id == Instruction::Sub || op_id == Instruction::Mul ||
            op_id == Instruction::SDiv || op_id == Instruction::SRem || op_id ==Instruction::FAdd ||
            op_id == Instruction::FSub || op_id == Instruction::FMul || op_id == Instruction::FDiv ||
            op_id == Instruction::ICmp || op_id == Instruction::FCmp)
            {
                struct Expr tmp_expr;

                tmp_expr.opr_0 = instr->get_operand(0);
                tmp_expr.opr_1 = instr->get_operand(1);
                tmp_expr.op = instr->op_id_;

                if (op_id == Instruction::ICmp)
                    tmp_expr.cmpop = dynamic_cast<ICmp_Instruction *>(instr)->icmp_op_;
                if (op_id == Instruction::FCmp)
                    tmp_expr.cmpop = dynamic_cast<FCmp_Instruction *>(instr)->fcmp_op_;

                // 第一次出现，加入ALL
                if (ALL.find(tmp_expr) == ALL.end())
                {
                    tmp_expr.expr_id = expr_cnt;
                    ALL.insert({tmp_expr, expr_cnt});
                    expr_cnt++;
                }
            }
            else if (op_id == Instruction::GetElementPtr)
            {
                std::vector<Value *> idx;

                for (int i = 0; i < instr->num_ops_; i++)
                    idx.push_back(instr->get_operand(i));
                
                struct Expr tmp_expr;

                tmp_expr.opr_0 = NULL;
                tmp_expr.opr_1 = NULL;
                tmp_expr.index = idx;
                tmp_expr.op = instr->op_id_;

                // 第一次出现，加入ALL
                if (ALL.find(tmp_expr) == ALL.end())
                {
                    tmp_expr.expr_id = expr_cnt;
                    ALL.insert({tmp_expr, expr_cnt});
                    expr_cnt++;
                }
            }
            else if(op_id == Instruction::FPtoSI || op_id == Instruction::SItoFP)
            {
                struct Expr tmp_expr;

                tmp_expr.opr_0 = instr->get_operand(0);
                tmp_expr.opr_1 = NULL;
                tmp_expr.op = instr->op_id_;

                // 第一次出现，加入ALL
                if (ALL.find(tmp_expr) == ALL.end())
                {
                    tmp_expr.expr_id = expr_cnt;
                    ALL.insert({tmp_expr, expr_cnt});
                    expr_cnt++;
                }
            }
        }
    }
    return;
}

void ComSubExprEli::Get_AvailExpr(Function *func)
{
    int num = ALL.size();
    //对每一个基本块，求出gen[](SSA中kill始终为空集)
    for (auto bb : func->basic_blocks_)
    {
        std::vector<bool> v1(num);

        //遍历每一条指令
        for (auto instr : bb->instr_list_)
        {
            int op_id = instr->op_id_;

            if(op_id == Instruction::Add || op_id == Instruction::Sub || op_id == Instruction::Mul ||
            op_id == Instruction::SDiv || op_id == Instruction::SRem || op_id ==Instruction::FAdd ||
            op_id == Instruction::FSub || op_id == Instruction::FMul || op_id == Instruction::FDiv ||
            op_id == Instruction::ICmp || op_id == Instruction::FCmp)
            {
                struct Expr tmp_expr;

                tmp_expr.opr_0 = instr->get_operand(0);
                tmp_expr.opr_1 = instr->get_operand(1);
                tmp_expr.op = instr->op_id_;

                if (op_id == Instruction::ICmp)
                    tmp_expr.cmpop = dynamic_cast<ICmp_Instruction *>(instr)->icmp_op_;
                if (op_id == Instruction::FCmp)
                    tmp_expr.cmpop = dynamic_cast<FCmp_Instruction *>(instr)->fcmp_op_;

                // 如果ALL中存在tmp_expr，则此BB中gen了对应表达式
                if (ALL.count(tmp_expr))
                    v1[ALL[tmp_expr]] = true;
                
            }
            else if (op_id == Instruction::GetElementPtr)
            {
                std::vector<Value *> idx;
                
                for (int i = 0; i < instr->num_ops_; i++)
                    idx.push_back(instr->get_operand(i));
                
                struct Expr tmp_expr;

                tmp_expr.opr_0 = NULL;
                tmp_expr.opr_1 = NULL;
                tmp_expr.index = idx;
                tmp_expr.op = instr->op_id_;

                // 如果ALL中存在tmp_expr，则此BB中gen了对应表达式
                if (ALL.count(tmp_expr))
                    v1[ALL[tmp_expr]] = true;
                
            }
            else if (op_id == Instruction::FPtoSI || op_id == Instruction::SItoFP)
            {
                struct Expr tmp_expr;

                tmp_expr.opr_0 = instr->get_operand(0);
                tmp_expr.opr_1 = NULL;
                tmp_expr.op = instr->op_id_;

                // 如果ALL中存在tmp_expr，则此BB中gen了对应表达式
                if (ALL.count(tmp_expr))
                    v1[ALL[tmp_expr]] = true;
                
            }
        }
        gen.insert({bb, v1});
        // printf("DEBUG\n");
        // //输出gen
        // printf("Gen:%s: \t", bb->name_.c_str());
        // for (int i = 0; i < num; i++)
        // {
        //     if (gen[bb][i])
        //         printf("%d", 1);
        //     else
        //         printf("%d", 0);
        // }
        // printf("\n");

        std::vector<bool> v2(num);

        //非label_entry的IN[]初始化为全1
        if (strcmp(bb->name_.c_str(), "label_entry"))
            for (int i = 0; i < num; i++)
                v2[i] = true;
        
        IN.insert({bb, v2});

        std::vector<bool> v3(num);

        //非label_entry的OUT[]初始化为全1
        if (strcmp(bb->name_.c_str(), "label_entry"))
            for (int i = 0; i < num; i++)
                v3[i] = true;

        OUT.insert({bb, v3});
    }

    //迭代计算IN和OUT
    bool OUT_change = true;
    while (OUT_change)
    {
        OUT_change = false;
        for (auto bb : func->basic_blocks_)
        {
            //bb的所有前驱的OUT取交集
            for (auto prevbb : bb->pre_bbs_)
                for (int i = 0; i < num; i++)
                    IN[bb][i] = IN[bb][i] & OUT[prevbb][i];

            //gen与IN求并集(kill始终为空)
            for (int i = 0; i < num; i++)
            {
                auto temp = gen[bb][i] | IN[bb][i];

                //OUT发生变化
                if (temp != OUT[bb][i])
                {
                    OUT_change = true;
                    OUT[bb][i] = temp;
                }
            }
        }
    }
    // printf("DEBUG\n");
    // printf("\nIN:\n");
    // for (auto bb : func->basic_blocks_) {
    //     printf("%s: \t", bb->name_.c_str());
    //     for (int i = 0; i < num; i ++ ) {
    //         if (IN[bb][i])
    //             printf("%d", 1);
    //         else
    //             printf("%d", 0);
    //     }
    //     printf("\n");
    // }
    // printf("\nOUT:\n");
    // for (auto bb : func->basic_blocks_) {
    //     printf("%s: \t", bb->name_.c_str());
    //     for (int i = 0; i < num; i ++ ) {
    //         if (OUT[bb][i])
    //             printf("%d", 1);
    //         else
    //             printf("%d", 0);
    //     }
    //     printf("\n");
    // }
}

std::vector<Basic_Block *> _visited;
void ComSubExprEli::Dlt_ComSubExpr(Function *func)
{
    //遍历所有基本块
    for (auto bb : func->basic_blocks_)
    {
        std::map<int, Value *> expr_list;       //处理基本块内的公共子表达式删除
        std::set<Instruction*> visited_instr;   //一个BB中已经遍历过的指令结合

        //遍历每一条指令
        for (auto instr : bb->instr_list_)
        {
            //判断相同指令是否已经遍历过，防止插入phi指令后重复遍历
            if (visited_instr.count(instr)) continue;
            visited_instr.insert(instr);

            int op_id = instr->op_id_;
            if (op_id == Instruction::Add || op_id == Instruction::Sub || op_id == Instruction::Mul ||
            op_id == Instruction::SDiv || op_id == Instruction::SRem || op_id ==Instruction::FAdd ||
            op_id == Instruction::FSub || op_id == Instruction::FMul || op_id == Instruction::FDiv ||
            op_id == Instruction::ICmp || op_id == Instruction::FCmp)
            {
                struct Expr tmp_expr;

                tmp_expr.opr_0 = instr->get_operand(0);
                tmp_expr.opr_1 = instr->get_operand(1);
                tmp_expr.op = instr->op_id_;

                if (op_id == Instruction::ICmp)
                    tmp_expr.cmpop = dynamic_cast<ICmp_Instruction *>(instr)->icmp_op_;
                if (op_id == Instruction::FCmp)
                    tmp_expr.cmpop = dynamic_cast<FCmp_Instruction *>(instr)->fcmp_op_;

                int i = expr_cnt; //i=expr_cnt表明未找到

                tmp_expr.expr_id = expr_cnt;

                if (ALL.count(tmp_expr))
                {
                    i = ALL[tmp_expr];
                    tmp_expr.expr_id = i;
                }

                //如果在该点此表达式可用
                if (i != expr_cnt && IN[bb][i])
                {
                    _visited.clear();
                    Value *target = Find_Expr(bb, tmp_expr);
                    instr->replace_all_use_with(target);
                    dlt_instr.insert({instr, bb});
                }
                else
                {
                    auto iter = expr_list.find(i);

                    //在该基本块前面出现过此表达式
                    if (iter != expr_list.end())
                    {
                        instr->replace_all_use_with(iter->second);
                        dlt_instr.insert({instr, bb});
                    }
                    else
                    {
                        expr_list.insert({i, instr});

                        if (i == expr_cnt) //新增一个表达式
                            expr_cnt++;
                    }
                }
            }
            else if (op_id == Instruction::GetElementPtr)
            {
                std::vector<Value *> idx;

                for (int i = 0; i < instr->num_ops_; i++)
                    idx.push_back(instr->get_operand(i));
                
                struct Expr tmp_expr;

                tmp_expr.opr_0 = NULL;
                tmp_expr.opr_1 = NULL;
                tmp_expr.index = idx;
                tmp_expr.op = instr->op_id_;

                int i = expr_cnt; //i=expr_cnt表明未找到

                tmp_expr.expr_id = expr_cnt;

                if (ALL.count(tmp_expr))
                {
                    i = ALL[tmp_expr];
                    tmp_expr.expr_id = i;
                }

                //如果在该点此表达式可用
                if (i != expr_cnt && IN[bb][i])
                {
                    _visited.clear();
                    Value *target = Find_Expr(bb, tmp_expr);
                    instr->replace_all_use_with(target);
                    dlt_instr.insert({instr, bb});
                }
                else
                {
                    auto iter = expr_list.find(i);
                    //在该基本块前面出现过此表达式
                    if (iter != expr_list.end())
                    {
                        instr->replace_all_use_with(iter->second);
                        dlt_instr.insert({instr, bb});
                    }
                    else
                    {
                        expr_list.insert({i, instr});
                        if (i == expr_cnt) //新增一个表达式
                            expr_cnt++;
                    }
                }
            }
            else if (op_id == Instruction::FPtoSI || op_id == Instruction::SItoFP)
            {
                struct Expr tmp_expr;

                tmp_expr.opr_0 = instr->get_operand(0);
                tmp_expr.opr_1 = NULL;
                tmp_expr.op = instr->op_id_;

                int i = expr_cnt; //i=expr_cnt表明未找到

                tmp_expr.expr_id = expr_cnt;

                if (ALL.count(tmp_expr))
                {
                    i = ALL[tmp_expr];
                    tmp_expr.expr_id = i;
                }
                //如果在该点此表达式可用
                if (i != expr_cnt && IN[bb][i])
                {
                    _visited.clear();
                    Value *target = Find_Expr(bb, tmp_expr);
                    instr->replace_all_use_with(target);
                    dlt_instr.insert({instr, bb});
                }
                else
                {
                    auto iter = expr_list.find(i);
                    //在该基本块前面出现过此表达式
                    if (iter != expr_list.end())
                    {
                        instr->replace_all_use_with(iter->second);
                        dlt_instr.insert({instr, bb});
                    }
                    else
                    {
                        expr_list.insert({i, instr});
                        if (i == expr_cnt) //新增一个表达式
                            expr_cnt++;
                    }
                }
            }
        }
    }
}

Value *ComSubExprEli::Find_Expr(Basic_Block *bb, struct Expr target)
{
    std::map<Value *, Basic_Block *> expr_list;
    _visited.push_back(bb);         //标记当前bb为已经遍历过

    //遍历所有prebb
    for (auto prevbb : bb->pre_bbs_)
    {
        //避免循环
        if (find(_visited.begin(), _visited.end(), prevbb) != _visited.end())   continue;

        //若该基本块首次计算target
        if (gen[prevbb][target.expr_id] && !IN[prevbb][target.expr_id])
        {
            //寻找计算target的指令
            for (auto instr : prevbb->instr_list_)
            {
                if (target.op == instr->op_id_)
                {
                    int op_id = instr->op_id_;

                    if (op_id == Instruction::Add || op_id == Instruction::Sub || op_id == Instruction::Mul ||
                    op_id == Instruction::SDiv || op_id == Instruction::SRem || op_id ==Instruction::FAdd ||
                    op_id == Instruction::FSub || op_id == Instruction::FMul || op_id == Instruction::FDiv ||
                    op_id == Instruction::ICmp || op_id == Instruction::FCmp)
                    {
                        struct Expr tmp_expr;

                        tmp_expr.opr_0 = instr->get_operand(0);
                        tmp_expr.opr_1 = instr->get_operand(1);
                        tmp_expr.op = instr->op_id_;

                        if (op_id == Instruction::ICmp)
                            tmp_expr.cmpop = dynamic_cast<ICmp_Instruction *>(instr)->icmp_op_;
                        if (op_id == Instruction::FCmp)
                            tmp_expr.cmpop = dynamic_cast<FCmp_Instruction *>(instr)->fcmp_op_;

                        if (target == tmp_expr)
                        {
                            expr_list.insert({instr, prevbb});
                            break;
                        }
                    }
                    else if (op_id == Instruction::GetElementPtr)
                    {
                        std::vector<Value *> idx;
                        for (int i = 0; i < instr->num_ops_; i++)
                            idx.push_back(instr->get_operand(i));

                        struct Expr tmp_expr;

                        tmp_expr.opr_0 = NULL;
                        tmp_expr.opr_1 = NULL;
                        tmp_expr.index = idx;
                        tmp_expr.op = instr->op_id_;

                        if (target == tmp_expr)
                        {
                            expr_list.insert({instr, prevbb});
                            break;
                        }
                    }
                    else if (op_id == Instruction::FPtoSI || op_id == Instruction::SItoFP)
                    {
                        struct Expr tmp_expr;
                        tmp_expr.opr_0 = instr->get_operand(0);
                        tmp_expr.opr_1 = NULL;
                        tmp_expr.op = instr->op_id_;

                        if (target == tmp_expr)
                        {
                            expr_list.insert({instr, prevbb});
                            break;
                        }
                    }
                }
            }
            _visited.push_back(prevbb); //标记为已经遍历过
        }
        //否则，继续向上寻找
        else
        {
            Value *res_instr = Find_Expr(prevbb, target);
            if (res_instr)
                expr_list.insert({res_instr, prevbb});
        }
    }
    // for (auto &[k, v] : expr_list) {
    //     cout << (dynamic_cast<Instruction*>(k))->print() << ' ' << v->name_ << endl;
    // }

    if (expr_list.size() == 0)
        return nullptr;
    else if (expr_list.size() == 1)
        return expr_list.begin()->first;
    else    //需要创建phi指令
    {
        Type *ty = expr_list.begin()->first->type_;
        auto newphi = PhiInst::create_phi(ty, bb);

        for (auto iter = expr_list.begin(); iter != expr_list.end(); iter++)
            newphi->add_phi_pair_operand(iter->first, iter->second);
        
        bb->add_instruction_front(newphi);

        return newphi;
    }
}

void ComSubExprEli::Dlt_Unused_Inst(Function *func)
{
    for (auto iter = dlt_instr.begin(); iter != dlt_instr.end(); iter++)
        iter->second->delete_instr(iter->first);
    
    return;
}
