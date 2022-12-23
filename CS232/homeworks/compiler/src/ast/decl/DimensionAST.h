//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_DIMENSIONAST_H
#define COMPILER_DIMENSIONAST_H


#include "../AST.h"
#include "../operation/LowAST.h"

class DimensionAST: public AST{
public:
    vector<unique_ptr<LowAST>> exps;

    void add(LowAST* exp);
};


#endif //COMPILER_DIMENSIONAST_H
