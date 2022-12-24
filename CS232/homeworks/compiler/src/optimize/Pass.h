#ifndef PASS_H
#define PASS_H


#include <string>
#include <list>
#include <algorithm>
#include "../IR/IR.h"


class Pass{
public:
    explicit Pass(Module* m){module = m;}
    virtual void execute() = 0;
    virtual const std::string get_name() const = 0;
    Module* module;
};

template<typename T>
using PassList = std::list<T*>;

class PassMgr{
public:
    explicit PassMgr(Module* m){module = m;pass_list = PassList<Pass>();}
    template <typename PassTy> void addPass(){pass_list.push_back(new PassTy(module));}
    void execute() {
        for (auto pass : pass_list) {
            pass->execute();
        }
    }
    Module* module;
    PassList<Pass> pass_list;
};


#endif // PASS_H
