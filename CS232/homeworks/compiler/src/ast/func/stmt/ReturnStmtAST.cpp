//
// Created by 86180 on 2022/12/22.
//

#include "ReturnStmtAST.h"

void ReturnStmtAST::setExpression(LowAST *expression) {
    this->exp=unique_ptr<LowAST>(expression);
}
