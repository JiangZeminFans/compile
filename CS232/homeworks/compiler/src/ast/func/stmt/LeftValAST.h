//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_LEFTVALAST_H
#define COMPILER_LEFTVALAST_H


#include "../../AST.h"
#include "../../operation/LowAST.h"
#include "../../decl/DimensionAST.h"

class LeftValAST: public AST{
public:
        unique_ptr<string> id;
        vector<unique_ptr<LowAST>> dimensions;
        void setId(string* id);
        void setDimensions(DimensionAST* dimensions);
};


#endif //COMPILER_LEFTVALAST_H
