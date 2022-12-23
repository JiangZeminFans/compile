//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_NUMBERAST_H
#define COMPILER_NUMBERAST_H


#include "../AST.h"

class NumberAST: public AST{
public:
    bool isInt;
    union{
        int int_t;
        float float_t;
    };
    void accept(Visitor &visitor) override;
};


#endif //COMPILER_NUMBERAST_H
