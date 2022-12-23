//
// Created by 86180 on 2022/12/23.
//

#ifndef COMPILER_RELEXPAST_H
#define COMPILER_RELEXPAST_H


#include "../AST.h"
#include "LowAST.h"

class RelExpAST: public AST{
public:
    unique_ptr<RelExpAST> low;
    unique_ptr<LowAST> high;
    REL_OP op;
    void setExp(RelExpAST* low,REL_OP op,LowAST* high);
};


#endif //COMPILER_RELEXPAST_H
