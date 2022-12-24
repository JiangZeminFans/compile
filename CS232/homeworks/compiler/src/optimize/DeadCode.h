#ifndef SYSYF_DEADCODE_H
#define SYSYF_DEADCODE_H

#include "Pass.h"
#include "RDTree.h"
// #include "Help.h"

class DeadCodeEli : public Pass
{
public:
    DeadCodeEli(Module* module) : Pass(module) {}
    void execute() final;
    const std::string getName() const override { return name; }
private:
    Function* func_;
    std::map<Function*, std::set<Value*>> funcPArg; //函数的指针参数
    std::map<Value*, std::list<Value*>> storeIns; //暂存使用了对应指针的store指令
    std::set<Value*> markedIns;
    std::set<Basic_Block*> markedBB;
    Basic_Block* retBB;

    const std::string name = "DeadCode";
    void initFuncPArg();
    void sweepIns();
    void mark();
    void initPre();
    bool isCrit(Instruction* ins);
};

#endif  // SYSYF_DEADCODE_H
