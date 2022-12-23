//
// Created by 86180 on 2022/12/22.
//

#include "DecAST.h"

void DecAST::accept(Visitor &visitor) {

}

void DecAST::setIdDefList(IdDefList* defList) {
    this->defList.swap(defList->defList);
}
