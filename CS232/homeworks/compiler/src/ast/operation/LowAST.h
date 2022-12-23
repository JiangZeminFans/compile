//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_LOWAST_H
#define COMPILER_LOWAST_H

#include "../AST.h"
#include "HighAST.h"

//+,-
class LowAST: public AST{
public:
    unique_ptr<LowAST> low;
    unique_ptr<HighAST> high;
    LOW_OP op;
    void accept(Visitor &visitor) override;
    void setExpr(LowAST* low,LOW_OP op,HighAST *high);
    void setHighExp(HighAST* high);
};


#endif //COMPILER_LOWAST_H
