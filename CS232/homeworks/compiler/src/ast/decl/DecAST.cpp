//
// Created by 86180 on 2022/12/22.
//

#include "DecAST.h"

void DecAST::accept(Visitor &visitor) {

}

void DecAST::setIdDefList(vector<unique_ptr<IdDefAST>> defList) {
    this->defList.swap(defList);
}
