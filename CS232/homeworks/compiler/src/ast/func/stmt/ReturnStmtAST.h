//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_RETURNSTMTAST_H
#define COMPILER_RETURNSTMTAST_H


#include "../../AST.h"
#include "../../operation/LowAST.h"

class ReturnStmtAST: public AST{
public:
    unique_ptr<LowAST> exp;
    void setExpression(LowAST* expression);
};


#endif //COMPILER_RETURNSTMTAST_H
