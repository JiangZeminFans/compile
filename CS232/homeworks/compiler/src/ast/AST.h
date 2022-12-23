//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_AST_H
#define COMPILER_AST_H

#include <iostream>
#include <vector>
#include "memory"
#include "Visitor.h"
#include "../base.h"
using namespace std;

class AST {
public:
    virtual void accept(Visitor& visitor)=0;
    AST()=default;
    virtual ~AST()=default;

};


#endif //COMPILER_AST_H
