//
// Created by 86180 on 2022/12/23.
//

#ifndef COMPILER_ORAST_H
#define COMPILER_ORAST_H


#include "../../AST.h"
#include "AndAST.h"

class OrAST: public AST{
public:
    unique_ptr<OrAST> low;
    unique_ptr<AndAST> high;

    void setExp(OrAST* low,AndAST* high);
};


#endif //COMPILER_ORAST_H
