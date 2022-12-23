//
// Created by 86180 on 2022/12/22.
//

#include "CompUnitAST.h"

void CompUnitAST::accept(Visitor &visitor) {
}

void CompUnitAST::add(DecDefAST* decDef) {
    this->decDefList.push_back(unique_ptr<DecDefAST>(decDef));
}
