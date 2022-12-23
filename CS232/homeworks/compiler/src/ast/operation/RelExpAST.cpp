//
// Created by 86180 on 2022/12/23.
//

#include "RelExpAST.h"

void RelExpAST::setExp(RelExpAST *low, REL_OP op, LowAST* high) {
    this->low=unique_ptr<RelExpAST>(low);
    this->op=op;
    this->high=unique_ptr<LowAST>(high);
}
