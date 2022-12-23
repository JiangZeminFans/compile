//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_FUNCPARAMLIST_H
#define COMPILER_FUNCPARAMLIST_H

#include "FuncParamAST.h"
class FuncParamList: public AST{
public:
    vector<unique_ptr<FuncParamAST>> list;
    void add(FuncParamAST* funcParamAst);
};


#endif //COMPILER_FUNCPARAMLIST_H
