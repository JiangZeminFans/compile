#ifndef CONSTPRO_H
#define CONSTPRO_H
#include "Pass.h"
#include "../IR/IR.h"

// #include "Help.h"

// �жϱ����Ƿ�Ϊ��ֵ
Constant_Int* judgeConstInt(Value* value);
Constant_Float* judgeConstFloat(Value* value);

class ConstFolder
{
public:
    ConstFolder(Module* module) : module_(module) {}

    Module* module_;
    Constant_Int* computeINT(Instruction::OpID op, Constant_Int* value1, Constant_Int* value2);
    Constant_Float* computeFLOAT(Instruction::OpID op, Constant_Float* value1, Constant_Float* value2);
    Constant_Int* computeCMP(ICmp_Instruction::ICmp_Op op, Constant_Int* value1, Constant_Int* value2);
    Constant_Int* computeFCMP(FCmp_Instruction::FCmp_Op op, Constant_Float* value1, Constant_Float* value2);   
};

class ConstPro : public Pass
{
public:
    const std::string name = "ConstPro";
    ConstFolder* folder;
    std::map<Instruction*, Basic_Block*> dltList;    //��ɾ��ָ��
    std::map<Value*, Constant_Int*> globalInt;        //int32������
    std::map<Value*, Constant_Float*> globalFloat;    //float������

    ConstPro(Module* module) : Pass(module) {}
    void execute() final;
    const std::string getName() const override { return name; }
    void Const_Fold(Function* func);                        //��������
    void Handle_Br(Function* func);                         //����Br
};

#endif  // CONSTPRO_H
