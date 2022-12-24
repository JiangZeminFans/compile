#ifndef SYSYCOMPILER_DELETEUNUSEDRETURN_H
#define SYSYCOMPILER_DELETEUNUSEDRETURN_H
#include "Pass.h"
/// <summary>
/// 删除未使用的返回指令块中的指令
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
