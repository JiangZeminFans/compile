//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_ITERATIONSTMTAST_H
#define COMPILER_ITERATIONSTMTAST_H


#include "../../AST.h"
#include "StmtAST.h"

class IterationStmtAST: public AST{
public:
    unique_ptr<StmtAST> stmt;
    unique_ptr<OrAST> cond;

    void setStmt(StmtAST* stmt);
    void setCondition(OrAST* condition);
};


#endif //COMPILER_ITERATIONSTMTAST_H
