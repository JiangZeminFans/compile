//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_CALLAST_H
#define COMPILER_CALLAST_H


#include "../AST.h"
#include "LowAST.h"
//函数引用
class CallAST: public AST{
public:
    unique_ptr<string> id;
    vector<unique_ptr<LowAST>> funcParamList;

    void setId(string* id);
    void setFuncParams();
};


#endif //COMPILER_CALLAST_H
