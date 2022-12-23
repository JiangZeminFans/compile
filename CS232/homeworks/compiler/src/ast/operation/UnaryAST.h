//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_UNARYAST_H
#define COMPILER_UNARYAST_H


#include "PrimaryExpAST.h"
#include "CallAST.h"

class UnaryAST: public AST {
public:
    unique_ptr<PrimaryExpAST> primaryExp;
    unique_ptr<CallAST> call;
    unique_ptr<UnaryAST> unary;
    U_OP op;

    void setOp(U_OP op);

    void setPrimaryExp(PrimaryExpAST* primaryExp);
    void setCall(CallAST* call);
    void setUnary(UnaryAST* unaryAst);

};


#endif //COMPILER_UNARYAST_H
