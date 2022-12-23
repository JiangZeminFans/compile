//
// Created by 86180 on 2022/12/22.
//

#include "IterationStmtAST.h"

void IterationStmtAST::setStmt(StmtAST *stmt) {
    this->stmt=unique_ptr<StmtAST>(stmt);
}

void IterationStmtAST::setCondition(OrAST *cond) {
    this->cond=unique_ptr<OrAST>(cond);
}
