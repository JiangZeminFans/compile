//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_OPERATION_H
#define COMPILER_OPERATION_H

enum LOW_OP{
    ADD_OP,MINUS_OP
};
enum HIGH_OP{
    MUL_OP,DIV_OP,MOD_OP
};
enum REL_OP{
    GE_REL,
    LE_REL,
    GT_REL,
    LT_REL
};
enum EQ_OP{
    EQ_REL,
    NE_REL
};
enum U_OP{
    U_POS,
    U_NEG,
    U_NOT
};
#endif //COMPILER_OPERATION_H
