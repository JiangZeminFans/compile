//
// Created by 86180 on 2022/12/22.
//

#include "StmtAST.h"

void StmtAST::accept(Visitor &visitor) {

}

void StmtAST::setStmtType(STMT_TYPE stmtType) {
    StmtAST::stmtType = stmtType;
}

void StmtAST::setLeftVal(LeftValAST* leftVal) {
    setStmtType(ASS_T);
    this->leftVal=unique_ptr<LeftValAST>(leftVal);
}

void StmtAST::setExpression(LowAST* expression) {
    setStmtType(EXP_T);
    this->exp=unique_ptr<LowAST>(expression);
}

void StmtAST::setBlock(BlockAST *block) {
    setStmtType(BLK_T);
    this->block=unique_ptr<BlockAST>(block);
}

void StmtAST::setReturn(ReturnStmtAST *returnStmt) {
    setStmtType(RET_T);
    this->ret=unique_ptr<ReturnStmtAST>(returnStmt);
}

void StmtAST::setSelect(SelectStmtAST *select) {
    setStmtType(SEL_T);
    this->sel=unique_ptr<SelectStmtAST>(select);
}

void StmtAST::setIteration(IterationStmtAST *iteration) {
    setStmtType(ITR_T);
    this->itr=unique_ptr<IterationStmtAST>(iteration);
}

