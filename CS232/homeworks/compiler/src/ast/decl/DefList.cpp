//
// Created by 86180 on 2022/12/22.
//

#include "DefList.h"

void IdDefList::add(IdDefAST *idDefAst) {
    this->defList.push_back(unique_ptr<IdDefAST>(idDefAst));
}
