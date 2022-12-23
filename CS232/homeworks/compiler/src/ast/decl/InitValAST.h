//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_INITVALAST_H
#define COMPILER_INITVALAST_H


#include "../AST.h"
#include "../operation/LowAST.h"
#include "InitValList.h"

class InitValAST : public AST{
public:
    unique_ptr<LowAST> exp;
    vector<unique_ptr<InitValAST>> initValList;
    void setExpression(LowAST* exp);
    void setInitValList(InitValList* list);
    void accept(Visitor &visitor) override;

};


#endif //COMPILER_INITVALAST_H
