//
// Created by 86180 on 2022/12/23.
//

#include "AndAST.h"

void AndAST::setExp(AndAST *low, EqExpAST *high) {
    this->low=unique_ptr<AndAST>(low);
    this->high=unique_ptr<EqExpAST>(high);
}
