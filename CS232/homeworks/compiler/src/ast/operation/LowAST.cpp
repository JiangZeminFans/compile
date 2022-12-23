//
// Created by 86180 on 2022/12/22.
//

#include "LowAST.h"

void LowAST::accept(Visitor &visitor) {

}

void LowAST::setExpr(LowAST *low, LOW_OP op, HighAST *high) {
    this->low=unique_ptr<LowAST>(low);
    this->op=op;
    this->high=unique_ptr<HighAST>(high);
}

void LowAST::setHighExp(HighAST* high) {
    this->high=unique_ptr<HighAST>(high);
}
