//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_INITVALLIST_H
#define COMPILER_INITVALLIST_H

#include "InitValAST.h"

class InitValList {
public:
    vector<unique_ptr<InitValAST>> initValList;
    InitValList()=default;
    void add(InitValAST* initVal);
};


#endif //COMPILER_INITVALLIST_H
