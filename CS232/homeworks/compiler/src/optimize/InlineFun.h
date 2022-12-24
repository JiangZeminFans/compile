#ifndef SYSYCOMPILER_INLINEFUN_H
#define SYSYCOMPILER_INLINEFUN_H
#include "Pass.h"

class InlineFun : public Pass 
{
public:
    InlineFun(Module* module) : Pass(module) {}
    void execute() override;
    const std::string getName() const override { return name; }

private:
    std::map<Function*, std::set<Function*>> funSuc;
    std::set<Function*> recFunc; // µÝ¹éº¯Êý
    const std::string name = "InlineFun";

    void initfunSuc();
    void initrecFunc();
    void initrecFunc(Function* cur, std::set<Function*>& visited, std::set<Function*>& judged);
    void inlineFunc(Call_Instruction* call);
    void delUselessJump();
};

#endif //SYSYCOMPILER_INLINEFUN_H
