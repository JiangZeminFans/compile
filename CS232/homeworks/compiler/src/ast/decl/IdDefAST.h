//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_IDDEFAST_H
#define COMPILER_IDDEFAST_H


#include "../AST.h"
#include "InitValAST.h"
#include "DimensionAST.h"

class IdDefAST: public AST{
public:
    unique_ptr<string> id;
    unique_ptr<InitValAST> initVal;
    vector<unique_ptr<LowAST>> dimensions;

    void setId(string* id);

    void setInitVal(InitValAST* initVal);

    void setDimensions(DimensionAST* dimensions);

    void accept(Visitor &visitor) override;
};


#endif //COMPILER_IDDEFAST_H
