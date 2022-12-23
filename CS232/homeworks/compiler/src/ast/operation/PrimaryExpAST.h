//
// Created by 86180 on 2022/12/23.
//

#ifndef COMPILER_PRIMARYEXPAST_H
#define COMPILER_PRIMARYEXPAST_H


#include "../AST.h"
#include "../func/stmt/LeftValAST.h"
#include "NumberAST.h"

class PrimaryExpAST: public AST {
public:
    unique_ptr<LeftValAST> leftVal;
    unique_ptr<LowAST> exp;
    unique_ptr<NumberAST> number;
    void setLeftVal(LeftValAST*);
    void setExpression(LowAST*);
    void setNumber(NumberAST*);
};


#endif //COMPILER_PRIMARYEXPAST_H
