//
// Created by 86180 on 2022/12/23.
//

#include "EqExpAST.h"

void EqExpAST::setExp(EqExpAST *low, EQ_OP op, RelExpAST *high) {
    this->low=unique_ptr<EqExpAST>(low);
    this->high=unique_ptr<RelExpAST>(high);
    this->op=op;
}
