//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_FUNCDEFAST_H
#define COMPILER_FUNCDEFAST_H


#include "../AST.h"
#include "BlockAST.h"
#include "FuncParamAST.h"
#include "FuncParamList.h"

class FuncDefAST: public AST{
public:
    TYPE funcType;
    unique_ptr<string> id;
    vector<unique_ptr<FuncParamAST>> funcParamList;
    unique_ptr<BlockAST> block = nullptr;

    void setId(string *id);

    void setFuncParamList(FuncParamList funcParamList);

    void setFuncType(TYPE funcType);

    void setBlock(BlockAST* block);
    void accept(Visitor &visitor) override;

};


#endif //COMPILER_FUNCDEFAST_H
