#ifndef SYSYCOMPILER_UNUSEDRETURNELI_H
#define SYSYCOMPILER_UNUSEDRETURNELI_H
#include "Pass.h"
/// <summary>
/// ����δʹ�õķ���ָ����е�ָ��
/// </summary>
class UnusedReturnEli :public Pass{
public:
    explicit UnusedReturnEli(Module* module): Pass(module){}
    void execute() final;
    const std::string get_name() const override {return name;};
private:
    const std::string name = "UnusedReturnEli";

};


#endif //SYSYCOMPILER_UNUSEDRETURNELI_H
