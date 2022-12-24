#ifndef COMSUBEXPRELI_H
#define COMSUBEXPRELI_H

#include "Pass.h"
#include <map>
#include <set>
#include <unordered_map>

struct Expr
{
    int expr_id; //表达式标号
    Value *opr_0;
    Value *opr_1;
    std::vector<Value *> index; //用于GetElementPtr,保存数组下标
    Instruction::OpID op;
    int cmpop;

    bool operator==(const Expr &expr) const
    {
        // 运算符不同，直接返回false
        if (op != expr.op)  return false;
        
        // 二元运算指令
        if (op == Instruction::Add || op == Instruction::Sub || op == Instruction::Mul ||
            op == Instruction::SDiv || op == Instruction::SRem || op ==Instruction::FAdd ||
            op == Instruction::FSub || op == Instruction::FMul || op == Instruction::FDiv ||
            op == Instruction::ICmp || op == Instruction::FCmp)
        {
            if (op == Instruction::OpID::ICmp || op == Instruction::OpID::FCmp)
                if (cmpop != expr.cmpop)    return false;
            

            //加法和乘法左右操作数可交换
            bool reverse = false;
            if (op == Instruction::OpID::Add || op == Instruction::OpID::Mul
            || op == Instruction::OpID::FAdd || op == Instruction::OpID::FMul)
                reverse = true;

            //左右操作数分别相等
            if (opr_0 == expr.opr_0 && opr_1 == expr.opr_1) return true;
            if (reverse && opr_0 == expr.opr_1 && opr_1 == expr.opr_0)  return true;

            //可能出现常数
            auto opr_0_i1 = dynamic_cast<Constant_Int *>(opr_0);
            auto opr_1_i1 = dynamic_cast<Constant_Int *>(opr_1);
            auto opr_0_f1 = dynamic_cast<Constant_Float *>(opr_0);
            auto opr_1_f1 = dynamic_cast<Constant_Float *>(opr_1);

            auto opr_0_i2 = dynamic_cast<Constant_Int *>(expr.opr_0);
            auto opr_1_i2 = dynamic_cast<Constant_Int *>(expr.opr_1);
            auto opr_0_f2 = dynamic_cast<Constant_Float *>(expr.opr_0);
            auto opr_1_f2 = dynamic_cast<Constant_Float *>(expr.opr_1);

            //INT
            if (opr_0_i1 && opr_0_i2 && opr_1_i1 && opr_1_i2)
            {
                int const_exp1_opr0 = opr_0_i1->value_;
                int const_exp1_opr1 = opr_1_i1->value_;
                int const_exp2_opr0 = opr_0_i2->value_;
                int const_exp2_opr1 = opr_1_i2->value_;

                if (const_exp1_opr0 == const_exp2_opr0 && const_exp1_opr1 == const_exp2_opr1)
                    return true;
                else if (reverse && const_exp1_opr0 == const_exp2_opr1 && const_exp1_opr1 == const_exp2_opr0)
                    return true;
                else
                    return false;
            }
            else if (opr_0_i1 && opr_0_i2)
            {
                int const_opr0 = opr_0_i1->value_;
                int const_opr1 = opr_0_i2->value_;

                if (const_opr0 == const_opr1 && opr_1 == expr.opr_1)
                    return true;
                else
                    return false;
            }
            else if (opr_1_i1 && opr_1_i2)
            {
                int const_opr0 = opr_1_i1->value_;
                int const_opr1 = opr_1_i2->value_;

                if (const_opr0 == const_opr1 && opr_0 == expr.opr_0)
                    return true;
                else
                    return false;
            }
            else if (reverse && opr_0_i1 && opr_1_i2)
            {
                int const_opr0 = opr_0_i1->value_;
                int const_opr1 = opr_1_i2->value_;

                if (const_opr0 == const_opr1 && opr_1 == expr.opr_0)
                    return true;
                else
                    return false;
            }
            else if (reverse && opr_1_i1 && opr_0_i2)
            {
                int const_opr0 = opr_1_i1->value_;
                int const_opr1 = opr_0_i2->value_;

                if (const_opr0 == const_opr1 && opr_0 == expr.opr_1)
                    return true;
                else
                    return false;
            }

            //FLOAT
            if (opr_0_f1 && opr_0_f2 && opr_1_f1 && opr_1_f2)
            {
                float const_exp1_opr0 = opr_0_f1->value_;
                float const_exp1_opr1 = opr_1_f1->value_;
                float const_exp2_opr0 = opr_0_f2->value_;
                float const_exp2_opr1 = opr_1_f2->value_;

                if (const_exp1_opr0 == const_exp2_opr0 && const_exp1_opr1 == const_exp2_opr1)
                    return true;
                else if (reverse && const_exp1_opr0 == const_exp2_opr1 && const_exp1_opr1 == const_exp2_opr0)
                    return true;
                else
                    return false;
            }
            else if (opr_0_f1 && opr_0_f2)
            {
                float const_opr0 = opr_0_f1->value_;
                float const_opr1 = opr_0_f2->value_;

                if (const_opr0 == const_opr1 && opr_1 == expr.opr_1)
                    return true;
                else
                    return false;
            }
            else if (opr_1_f1 && opr_1_f2)
            {
                float const_opr0 = opr_1_f1->value_;
                float const_opr1 = opr_1_f2->value_;

                if (const_opr0 == const_opr1 && opr_0 == expr.opr_0)
                    return true;
                else
                    return false;
            }
            else if (reverse && opr_0_f1 && opr_1_f2)
            {
                float const_opr0 = opr_0_f1->value_;
                float const_opr1 = opr_1_f2->value_;

                if (const_opr0 == const_opr1 && opr_1 == expr.opr_0)
                    return true;
                else
                    return false;
            }
            else if (reverse && opr_1_f1 && opr_0_f2)
            {
                float const_opr0 = opr_1_f1->value_;
                float const_opr1 = opr_0_f2->value_;

                if (const_opr0 == const_opr1 && opr_0 == expr.opr_1)
                    return true;
                else
                    return false;
            }
            return false;
        }
        // gep指令
        else if (op == Instruction::GetElementPtr)
        {
            if (index.size() != expr.index.size())
                return false;
            
            for (int i = 0; i < index.size(); i++)
            {
                auto const_opr0 = dynamic_cast<Constant_Int *>(index[i]);
                auto const_opr1 = dynamic_cast<Constant_Int *>(expr.index[i]);
                if (const_opr0 && const_opr1)           //常量
                {
                    if (const_opr0->value_ != const_opr1->value_)
                        return false;
                }
                else if (!const_opr0 && !const_opr1)    //不是常量
                {
                    if ((index[i] != expr.index[i]))
                        return false;
                }
                else return false;
            }
            return true;
        }
        else if (op == Instruction::FPtoSI || op == Instruction::SItoFP)
        {
            if (op == Instruction::SItoFP) {
                auto const_opr0 = dynamic_cast<Constant_Int *>(opr_0);
                auto const_opr1 = dynamic_cast<Constant_Int *>(expr.opr_0);

                if (const_opr0 && const_opr1)           //常量
                {   
                    if (const_opr0->value_ != const_opr1->value_)
                        return false;
                    else return true;
                }
                else if (!const_opr0 && !const_opr1)  //不是常量
                {
                    if ((opr_0 != expr.opr_0))
                        return false;
                    else return true;
                }
                else return false;
            } 
            else    //op == Instruction::FPtoSI
            {
                auto const_opr0 = dynamic_cast<Constant_Float *>(opr_0);
                auto const_opr1 = dynamic_cast<Constant_Float *>(expr.opr_0);

                if (const_opr0 && const_opr1)           //常量
                {
                    if (const_opr0->value_ != const_opr1->value_)
                        return false;
                    else return true;
                }
                else if (!const_opr0 && !const_opr1)  //不是常量
                {
                    if ((opr_0 != expr.opr_0))
                        return false;
                    else return true;
                }
                else return false;
            }
            return false;
        }
        return false;
    }
};

struct myHashFuc
{
	std::size_t operator()(const Expr &expr) const
    {
        std::size_t hash_value = std::hash<int>()(expr.op);

        if (expr.op == Instruction::ICmp || expr.op == Instruction::FCmp)
            hash_value ^= std::hash<int>()(expr.cmpop);
        
        // if (expr.opr_0 && !expr.opr_1)      //FPtoSI or SItoFP
        //     hash_value = std::hash<unsigned long long>()((unsigned long long)expr.opr_0) ^ std::hash<int>()(expr.op);
        // else if (expr.opr_0 && expr.opr_1)  //Binary
        //     hash_value = std::hash<unsigned long long>()((unsigned long long)expr.opr_0) ^ std::hash<unsigned long long>()((unsigned long long)expr.opr_1)
        //                 ^ std::hash<int>()(expr.op) ^ std::hash<int>()(expr.cmpop);
        // else {                              //Gep
        //     for (int i = 0; i < expr.index.size(); i ++ ) {
        //         if (!i) hash_value = std::hash<unsigned long long>()((unsigned long long)expr.index[0]);
        //         else hash_value ^= std::hash<unsigned long long>()((unsigned long long)expr.index[i]);
        //     }
        // }

		return hash_value;
	}
};

class ComSubExprEli : public Pass
{
public:
    explicit ComSubExprEli(Module *module) : Pass(module) {}

    const std::string get_name() const override { return name; }

    void execute() override;

    void Get_AllExpr(Function *func);       //获取所有表达式
    void Get_AvailExpr(Function *func);     //可用表达式分析
    void Dlt_ComSubExpr(Function *func);    //删除公共子表达式
    void Dlt_Unused_Inst(Function *func);   //删除无用指令

private:
    const std::string name = "ComSubExprEli";

    int expr_cnt = 0;                       //记录当前处理的表达式个数

    std::unordered_map<struct Expr, int, myHashFuc> ALL;         //所有的表达式

    //使用位向量表示每个BB的gen,IN,OUT
    std::map<Basic_Block *, std::vector<bool>> gen, IN, OUT;

    //待删除指令
    std::map<Instruction *, Basic_Block *> dlt_instr;
    
    //从bb开始寻找前驱基本块中计算了表达式target的,返回替换的值
    Value *Find_Expr(Basic_Block *bb, struct Expr target);
};

#endif // COMSUBEXPRELI_H