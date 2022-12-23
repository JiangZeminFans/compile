//
// Created by 86180 on 2022/12/22.
//

#include "FuncParamList.h"


void FuncParamList::add(FuncParamAST *funcParamAst) {
    this->list.push_back(unique_ptr<FuncParamAST>(funcParamAst));
}
