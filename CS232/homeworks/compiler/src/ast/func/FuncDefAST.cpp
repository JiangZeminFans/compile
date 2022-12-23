//
// Created by 86180 on 2022/12/22.
//

#include "FuncDefAST.h"

void FuncDefAST::accept(Visitor &visitor) {

}

void FuncDefAST::setFuncParamList(FuncParamList funcParamList) {
    this->funcParamList.swap(funcParamList.list);
}

void FuncDefAST::setFuncType(TYPE funcType) {
    FuncDefAST::funcType = funcType;
}

void FuncDefAST::setBlock(BlockAST *block) {
    this->block=unique_ptr<BlockAST>(block);
}

void FuncDefAST:: setId(string *id) {
    this->id=unique_ptr<string>(id);
}

