//
// Created by 86180 on 2022/12/23.
//

#include "OrAST.h"

void OrAST::setExp(OrAST *low, AndAST *high) {
    this->low=unique_ptr<OrAST>(low);
    this->high=unique_ptr<AndAST>(high);
}
