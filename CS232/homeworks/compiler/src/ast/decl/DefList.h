//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_DEFLIST_H
#define COMPILER_DEFLIST_H
#include "../AST.h"
#include "IdDefAST.h"

class IdDefList {
public:
    vector<unique_ptr<IdDefAST>> defList;
    IdDefList()=default;
    void add(IdDefAST* idDefAst);
};


#endif //COMPILER_DEFLIST_H
