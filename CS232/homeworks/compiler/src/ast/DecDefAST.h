//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_DECDEFAST_H
#define COMPILER_DECDEFAST_H


#include "AST.h"
#include "decl/DecAST.h"
#include "func/FuncDefAST.h"


class DecDefAST: public AST{
public:
    unique_ptr<DecAST> Decl;
    unique_ptr<FuncDefAST> funcDef;

    void setDecl(DecAST* decl);
    void setDef(FuncDefAST* def);
};


#endif //COMPILER_DECDEFAST_H
