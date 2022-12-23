//
// Created by 86180 on 2022/12/22.
//

#include "DimensionAST.h"

void DimensionAST::add(LowAST *exp) {
    this->exps.push_back(unique_ptr<LowAST>(exp));
}
