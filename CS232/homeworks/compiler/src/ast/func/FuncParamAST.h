//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_FUNCPARAMAST_H
#define COMPILER_FUNCPARAMAST_H


#include "../AST.h"
#include "../operation/LowAST.h"

class FuncParamAST: public AST{
public:
    TYPE bType;
    unique_ptr<string> id;
    bool isArray = false;
    vector<unique_ptr<LowAST>> arrays;
    void accept(Visitor &visitor) override;

};


#endif //COMPILER_FUNCPARAMAST_H
