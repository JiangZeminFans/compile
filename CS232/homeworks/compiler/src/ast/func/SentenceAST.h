//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_SENTENCEAST_H
#define COMPILER_SENTENCEAST_H


#include "../AST.h"
#include "../decl/DecAST.h"
#include "stmt/StmtAST.h"

class SentenceAST: public AST{
public:
    unique_ptr<DecAST> decl;
    unique_ptr<StmtAST> stmt;
    void setDecl(DecAST* decl);
    void setStmt(StmtAST* stmt);
};


#endif //COMPILER_SENTENCEAST_H
