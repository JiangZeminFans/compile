//
// Created by 86180 on 2022/12/22.
//

#include "HighAST.h"

void HighAST::setOp(HIGH_OP op) {
    HighAST::op = op;
}

void HighAST::setModExp(HighAST *factor) {
    setOp(MOD_OP);
    this->high=unique_ptr<HighAST>(factor);
}

void HighAST::setMulExp(HighAST *factor) {
    setOp(MUL_OP);
    this->high=unique_ptr<HighAST>(factor);
}

void HighAST::setDivExp(HighAST *factor) {
    setOp(DIV_OP);
    this->high=unique_ptr<HighAST>(factor);
}

void HighAST::setUnaryExp(UnaryAST *unary) {
    this->unary=unique_ptr<UnaryAST>(unary);
}

