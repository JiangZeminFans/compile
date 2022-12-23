//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_SELECTSTMTAST_H
#define COMPILER_SELECTSTMTAST_H


#include "../../AST.h"
#include "StmtAST.h"
#include "../../operation/logical/OrAST.h"

class SelectStmtAST: public AST{
public:
    unique_ptr<StmtAST> ifStmt;
    unique_ptr<StmtAST> elseStmt;

    unique_ptr<OrStmt> cond;

    void setIf(StmtAST* ifStmt);
    void setElse(StmtAST* elseStmt);
    void setCondition(OrAST* cond);
};


#endif //COMPILER_SELECTSTMTAST_H
