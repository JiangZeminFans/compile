//
// Created by 86180 on 2022/12/23.
//

#include "FuncRParamList.h"

void FuncRParamList::add(LowAST *realParams) {
    this->realParams.push_back(unique_ptr<LowAST>(realParams));
}
