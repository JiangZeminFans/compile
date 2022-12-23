//
// Created by 86180 on 2022/12/22.
//

#include "SelectStmtAST.h"


void SelectStmtAST::setIf(StmtAST *ifStmt) {
    this->ifStmt=unique_ptr<StmtAST>(ifStmt);
}

void SelectStmtAST::setElse(StmtAST *elseStmt) {
    this->elseStmt=unique_ptr<StmtAST>(elseStmt);
}

void SelectStmtAST::setCondition(OrAST *cond) {
    this->cond=unique_ptr<OrAST>(cond);
}

