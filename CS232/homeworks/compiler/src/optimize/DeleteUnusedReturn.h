#ifndef SYSYCOMPILER_DELETEUNUSEDRETURN_H
#define SYSYCOMPILER_DELETEUNUSEDRETURN_H
#include "Pass.h"
/// <summary>
/// ɾ��δʹ�õķ���ָ����е�ָ��
/// </summary>
class DeleteUnusedReturn :public Pass{
public:
    explicit DeleteUnusedReturn(Module* module): Pass(module){}
    void execute() final;
    const std::string get_name() const override {return name;};
private:
    const std::string name = "DeleteUnusedReturn";

};


#endif //SYSYCOMPILER_UNUSEDRETELI_H
