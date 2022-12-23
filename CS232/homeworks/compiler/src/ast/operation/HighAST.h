//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_HIGHAST_H
#define COMPILER_HIGHAST_H

#include "../AST.h"
#include "UnaryAST.h"

//*,/,%
class HighAST:public AST{
public:
    HIGH_OP op;
    unique_ptr<UnaryAST> unary;
    unique_ptr<HighAST> high;

    void setOp(HIGH_OP op);
    void setModExp(HighAST* factor);
    void setMulExp(HighAST* factor);
    void setDivExp(HighAST* factor);
    void setUnaryExp(UnaryAST* unary);

};


#endif //COMPILER_HIGHAST_H
