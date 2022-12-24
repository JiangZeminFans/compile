#ifndef SYSYCOMPILER_SIMPLIFYCFG_H
#define SYSYCOMPILER_SIMPLIFYCFG_H
#include "Pass.h"

class SimplifyCFG : public Pass
{
public:
    SimplifyCFG(Module *m) : Pass(m) {}

    void execute() override;

    const std::string get_name() const override {return name;};

private:
    const std::string name = "SimplifyCFG";

    Function *func_;

    void removeNoPreBB();
    void mergeSinglePreBB();
    void eliSinglePrePhi();
    void eliSingleJumpBB();
};


#endif //SYSYCOMPILER_SIMPLIFYCFG_H
