//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_DECAST_H
#define COMPILER_DECAST_H


#include "../AST.h"
#include "../func/FuncDefAST.h"
#include "IdDefAST.h"
#include "IdDefList.h"

class DecAST : public AST{
public:
    TYPE bType;
    bool isConst;
    vector<unique_ptr<IdDefAST>> defList;//声明表
    void accept(Visitor &visitor) override;
    void setIdDefList(IdDefList* defList);
};


#endif //COMPILER_DECAST_H
