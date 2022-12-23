//
// Created by 86180 on 2022/12/23.
//

#ifndef COMPILER_EQEXPAST_H
#define COMPILER_EQEXPAST_H


#include "../AST.h"
#include "RelExpAST.h"

class EqExpAST: public AST{
public:
    unique_ptr<RelExpAST> high;
    unique_ptr<EqExpAST> low;
    EQ_OP op;
    void setExp(EqExpAST* low,EQ_OP op,RelExpAST* high);
};


#endif //COMPILER_EQEXPAST_H
