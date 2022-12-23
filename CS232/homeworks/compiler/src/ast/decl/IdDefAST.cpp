//
// Created by 86180 on 2022/12/22.
//

#include "IdDefAST.h"

void IdDefAST::accept(Visitor &visitor) {

}

void IdDefAST::setId(string *id) {
    this->id=unique_ptr<string>(id);
}

void IdDefAST::setInitVal(InitValAST *initVal) {
    this->initVal=unique_ptr<InitValAST>(initVal);
}

void IdDefAST::setDimensions(DimensionAST *dimensions) {
    this->dimensions.swap(dimensions->exps);
}

