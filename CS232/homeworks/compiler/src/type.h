//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H
enum TYPE{
    VOID_T,INT_T,FLOAT_T
};

enum STMT_TYPE {
    SEMI_T, //;
    ASS_T, //assign
    EXP_T, //expression
    CONT_T, //continue
    BRK_T, //break
    RET_T, //return
    BLK_T, //block
    SEL_T,//IF-ELSE
    ITR_T //while，for
};

#endif //COMPILER_TYPE_H
