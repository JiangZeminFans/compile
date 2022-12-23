//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_STMTAST_H
#define COMPILER_STMTAST_H


#include "../../AST.h"
#include "ReturnStmtAST.h"
#include "../../operation/LowAST.h"
#include "SelectStmtAST.h"
#include "IterationStmtAST.h"
#include "../BlockAST.h"
#include "LeftValAST.h"

class StmtAST: public AST{
public:
    STMT_TYPE  stmtType;



    unique_ptr<ReturnStmtAST> ret= nullptr;
    unique_ptr<SelectStmtAST> sel= nullptr;
    unique_ptr<IterationStmtAST> itr= nullptr;

    unique_ptr<BlockAST> block = nullptr;
    unique_ptr<LeftValAST> leftVal=nullptr;
    unique_ptr<LowAST> exp= nullptr;

    void setLeftVal(LeftValAST* leftVal);
    void setStmtType(STMT_TYPE stmtType);
    void setExpression(LowAST* exp);
    void setBlock(BlockAST* block);
    void setReturn(ReturnStmtAST* returnStmt);
    void setSelect(SelectStmtAST* select);
    void setIteration(IterationStmtAST *iteration);
    void accept(Visitor &visitor) override;

};


#endif //COMPILER_STMTAST_H
