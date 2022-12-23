//
// Created by 86180 on 2022/12/22.
//

#include "LeftValAST.h"

void LeftValAST::setId(string* id) {
    this->id=unique_ptr<string>(id);
}

void LeftValAST::setDimensions(DimensionAST* dimension) {
    this->dimensions.swap(dimension->exps);
}
