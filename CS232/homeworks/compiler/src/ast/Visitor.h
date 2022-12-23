//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_VISITOR_H
#define COMPILER_VISITOR_H


#include "AST.h"
#include "all_ast.h"
class Visitor {
public:
    //重写该方法
    virtual void visit(AST& ast)=0;
    //TODO:增加相应的遍历方法
};


#endif //COMPILER_VISITOR_H
