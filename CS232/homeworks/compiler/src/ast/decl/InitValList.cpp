//
// Created by 86180 on 2022/12/22.
//

#include "InitValList.h"


void InitValList::add(InitValAST* initVal) {
    this->initValList.push_back(unique_ptr<InitValAST>(initVal));
}
