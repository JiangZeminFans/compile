//
// Created by 86180 on 2022/12/22.
//

#include "AST.h"

void AST::accept(Visitor &visitor) {
    visitor.visit(this);
}

void AndAST::setExp(AndAST* low, EqExpAST* high) {
    this->low = unique_ptr<AndAST>(low);
    this->high = unique_ptr<EqExpAST>(high);
}

void BlockAST::setSentences(Sentences* sentences) {
    this->sentences.swap(sentences->sentences);
}

void CallAST::setId(string* id) {
    this->id = unique_ptr<string>(id);
}

void CompUnitAST::add(DecDefAST* decDef) {
    this->decDefList.push_back(unique_ptr<DecDefAST>(decDef));
}

void DecAST::setIdDefList(IdDefList* defList) {
    this->defList.swap(defList->defList);
}

void DecDefAST::setDecl(DecAST* decl) {
    this->Decl = unique_ptr<DecAST>(decl);
}

void DecDefAST::setDef(FuncDefAST* def) {
    this->funcDef = unique_ptr<FuncDefAST>(def);
}

void DimensionAST::add(LowAST* exp) {
    this->exps.push_back(unique_ptr<LowAST>(exp));
}

void EqExpAST::setExp(EqExpAST* low, EQ_OP op, RelExpAST* high) {
    this->low = unique_ptr<EqExpAST>(low);
    this->high = unique_ptr<RelExpAST>(high);
    this->op = op;
}

void FuncDefAST::setFuncParamList(FuncParamList funcParamList) {
    this->funcParamList.swap(funcParamList.list);
}

void FuncDefAST::setFuncType(TYPE funcType) {
    FuncDefAST::funcType = funcType;
}

void FuncDefAST::setBlock(BlockAST *block) {
    this->block=unique_ptr<BlockAST>(block);
}

void FuncDefAST::setId(string *id) {
    this->id=unique_ptr<string>(id);
}

void FuncParamList::add(FuncParamAST* funcParamAst) {
    this->list.push_back(unique_ptr<FuncParamAST>(funcParamAst));
}

void FuncRParamList::add(LowAST* realParams) {
    this->realParams.push_back(unique_ptr<LowAST>(realParams));
}

void HighAST::setOp(HIGH_OP op) {
    HighAST::op = op;
}

void HighAST::setModExp(HighAST* factor) {
    setOp(MOD_OP);
    this->high = unique_ptr<HighAST>(factor);
}

void HighAST::setMulExp(HighAST* factor) {
    setOp(MUL_OP);
    this->high = unique_ptr<HighAST>(factor);
}

void HighAST::setDivExp(HighAST* factor) {
    setOp(DIV_OP);
    this->high = unique_ptr<HighAST>(factor);
}

void HighAST::setUnaryExp(UnaryAST* unary) {
    this->unary = unique_ptr<UnaryAST>(unary);
}

void IdDefAST::setId(string* id) {
    this->id = unique_ptr<string>(id);
}

void IdDefAST::setInitVal(InitValAST* initVal) {
    this->initVal = unique_ptr<InitValAST>(initVal);
}

void IdDefAST::setDimensions(DimensionAST* dimensions) {
    this->dimensions.swap(dimensions->exps);
}

void IdDefList::add(IdDefAST* idDefAst) {
    this->defList.push_back(unique_ptr<IdDefAST>(idDefAst));
}

void InitValAST::setExpression(LowAST* expression) {
    this->exp = unique_ptr<LowAST>(expression);
}

void InitValAST::setInitValList(InitValList* list) {
    this->initValList.swap(list->initValList);
}

void IterationStmtAST::setStmt(StmtAST* stmt) {
    this->stmt = unique_ptr<StmtAST>(stmt);
}

void IterationStmtAST::setCondition(OrAST* cond) {
    this->cond = unique_ptr<OrAST>(cond);
}

void InitValList::add(InitValAST* initVal) {
    this->initValList.push_back(unique_ptr<InitValAST>(initVal));
}

void LeftValAST::setId(string* id) {
    this->id = unique_ptr<string>(id);
}

void LeftValAST::setDimensions(DimensionAST* dimension) {
    this->dimensions.swap(dimension->exps);
}

void LowAST::setExpr(LowAST* low, LOW_OP op, HighAST* high) {
    this->low = unique_ptr<LowAST>(low);
    this->op = op;
    this->high = unique_ptr<HighAST>(high);
}

void LowAST::setHighExp(HighAST* high) {
    this->high = unique_ptr<HighAST>(high);
}

void OrAST::setExp(OrAST* low, AndAST* high) {
    this->low = unique_ptr<OrAST>(low);
    this->high = unique_ptr<AndAST>(high);
}

void PrimaryExpAST::setLeftVal(LeftValAST* leftVal) {
    this->leftVal = unique_ptr<LeftValAST>(leftVal);
}

void PrimaryExpAST::setExpression(LowAST* exp) {
    this->exp = unique_ptr<LowAST>(exp);
}

void PrimaryExpAST::setNumber(NumberAST* num) {
    this->number = unique_ptr<NumberAST>(num);
}

void RelExpAST::setExp(RelExpAST* low, REL_OP op, LowAST* high) {
    this->low = unique_ptr<RelExpAST>(low);
    this->op = op;
    this->high = unique_ptr<LowAST>(high);
}

void ReturnStmtAST::setExpression(LowAST* expression) {
    this->exp = unique_ptr<LowAST>(expression);
}

void SelectStmtAST::setIf(StmtAST* ifStmt) {
    this->ifStmt = unique_ptr<StmtAST>(ifStmt);
}

void SelectStmtAST::setElse(StmtAST* elseStmt) {
    this->elseStmt = unique_ptr<StmtAST>(elseStmt);
}

void SelectStmtAST::setCondition(OrAST* cond) {
    this->cond = unique_ptr<OrAST>(cond);
}

void SentenceAST::setDecl(DecAST* decl) {
    this->decl = unique_ptr<DecAST>(decl);
}

void SentenceAST::setStmt(StmtAST* stmt) {
    this->stmt = unique_ptr<StmtAST>(stmt);
}

void Sentences::add(SentenceAST* sentence) {
    sentences.push_back(unique_ptr<SentenceAST>(sentence));
}

void StmtAST::setStmtType(STMT_TYPE stmtType) {
    StmtAST::stmtType = stmtType;
}

void StmtAST::setLeftVal(LeftValAST* leftVal) {
    setStmtType(ASS_T);
    this->leftVal = unique_ptr<LeftValAST>(leftVal);
}

void StmtAST::setExpression(LowAST* expression) {
    setStmtType(EXP_T);
    this->exp = unique_ptr<LowAST>(expression);
}

void StmtAST::setBlock(BlockAST* block) {
    setStmtType(BLK_T);
    this->block = unique_ptr<BlockAST>(block);
}

void StmtAST::setReturn(ReturnStmtAST* returnStmt) {
    setStmtType(RET_T);
    this->ret = unique_ptr<ReturnStmtAST>(returnStmt);
}

void StmtAST::setSelect(SelectStmtAST* select) {
    setStmtType(SEL_T);
    this->sel = unique_ptr<SelectStmtAST>(select);
}

void StmtAST::setIteration(IterationStmtAST* iteration) {
    setStmtType(ITR_T);
    this->itr = unique_ptr<IterationStmtAST>(iteration);
}

void UnaryAST::setPrimaryExp(PrimaryExpAST* primaryExp) {
    UnaryAST::primaryExp = unique_ptr<PrimaryExpAST>(primaryExp);
}

void UnaryAST::setOp(U_OP op) {
    UnaryAST::op = op;
}

void UnaryAST::setCall(CallAST* call) {
    this->call = unique_ptr<CallAST>(call);
}

void UnaryAST::setUnary(UnaryAST* unaryAst) {
    this->unary = unique_ptr<UnaryAST>(unaryAst);
}
