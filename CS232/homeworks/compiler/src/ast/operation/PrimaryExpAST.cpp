//
// Created by 86180 on 2022/12/23.
//

#include "PrimaryExpAST.h"

void PrimaryExpAST::setLeftVal(LeftValAST * leftVal) {
    this->leftVal=unique_ptr<LeftValAST>(leftVal);
}

void PrimaryExpAST::setExpression(LowAST * exp) {
    this->exp=unique_ptr<LowAST>(exp);
}

void PrimaryExpAST::setNumber(NumberAST * num) {
    this->number=unique_ptr<NumberAST>(num);
}
