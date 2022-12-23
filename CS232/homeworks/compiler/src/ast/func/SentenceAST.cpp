//
// Created by 86180 on 2022/12/22.
//

#include "SentenceAST.h"

void SentenceAST::setDecl(DecAST *decl) {
    this->decl=unique_ptr<DecAST>(decl);
}

void SentenceAST::setStmt(StmtAST *stmt) {
    this->stmt=unique_ptr<StmtAST>(stmt);
}

