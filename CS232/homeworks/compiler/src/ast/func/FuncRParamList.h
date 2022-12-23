//
// Created by 86180 on 2022/12/23.
//

#ifndef COMPILER_FUNCRPARAMLIST_H
#define COMPILER_FUNCRPARAMLIST_H


#include "../AST.h"
#include "../operation/LowAST.h"

class FuncRParamList: public AST{
public:
    vector<unique_ptr<LowAST>> realParams;
    void add(LowAST* realParams);
};


#endif //COMPILER_FUNCRPARAMLIST_H
