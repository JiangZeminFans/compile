//
// Created by 86180 on 2022/12/23.
//

#ifndef COMPILER_ANDAST_H
#define COMPILER_ANDAST_H


#include "../../AST.h"
#include "../EqExpAST.h"

class AndAST: public AST{
public:
    unique_ptr<AndAST> low;
    unique_ptr<EqExpAST> high;

    void setExp(AndAST* low,EqExpAST* high);
};


#endif //COMPILER_ANDAST_H
