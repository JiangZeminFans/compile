//
// Created by 86180 on 2022/12/22.
//

#include "UnaryAST.h"

void UnaryAST::setPrimaryExp(PrimaryExpAST* primaryExp) {
    UnaryAST::primaryExp = unique_ptr<PrimaryExpAST>(primaryExp);
}

void UnaryAST::setOp(U_OP op) {
    UnaryAST::op = op;
}

void UnaryAST::setCall(CallAST *call) {
    this->call=unique_ptr<CallAST>(call);
}

void UnaryAST::setUnary(UnaryAST *unaryAst) {
    this->unary=unique_ptr<UnaryAST>(unaryAst);
}

