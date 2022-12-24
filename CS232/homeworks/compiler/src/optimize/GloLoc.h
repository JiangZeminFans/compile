#ifndef SYSYCOMPILER_GLOLOC_H
#define SYSYCOMPILER_GLOLOC_H
#include "Pass.h"

class GloLoc :public Pass {
public:
    explicit GloLoc(Module* module) : Pass(module) {}
    void execute() final;
    const std::string getName() const override { return name; };
private:
    std::map<Global_Variable*, std::set<Function*>> funcVar;
    std::map<Function*, std::set<Function*>> funcSuc;
    std::set<Function*> funcRec;
    Function* funcEntry;
    const std::string name = "GloLoc";

    void initfuncRec(Function* cur, std::set<Function*>& visited);
    void initfuncSuc();
};


#endif //SYSYCOMPILER_GLOLOC_H
