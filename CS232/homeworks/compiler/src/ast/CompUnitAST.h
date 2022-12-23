//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_COMPUNITAST_H
#define COMPILER_COMPUNITAST_H


#include "AST.h"
#include "DecDefAST.h"

class CompUnitAST: public AST{
public:
    vector<unique_ptr<DecDefAST>> decDefList;
    void add(DecDefAST* decDef);
};


#endif //COMPILER_COMPUNITAST_H
