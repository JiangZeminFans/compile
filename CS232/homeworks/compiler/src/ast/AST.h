//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_AST_H
#define COMPILER_AST_H

#include <iostream>
#include <vector>
#include <memory>
#include "../base.h"
using namespace std;
class AST;
class AndAST;
class Visitor;
class EqExpAST;
class SentenceAST;

class AST{
public:
    void accept(Visitor& visitor);
    AST()=default;
    virtual ~AST()=default;
};

class AndAST : public AST {
public:
    unique_ptr<AndAST> low;
    unique_ptr<EqExpAST> high;

    void setExp(AndAST* low, EqExpAST* high);
};

class Sentences;

class BlockAST : public AST {
public:
    vector<unique_ptr<SentenceAST>> sentences;
    void setSentences(Sentences* sentences);
};

class LowAST;

//函数引用
class CallAST : public AST {
public:
    unique_ptr<string> id;
    vector<unique_ptr<LowAST>> funcParamList;

    void setId(string* id);
    // void setFuncParams();
};

class DecDefAST;

class CompUnitAST : public AST {
public:
    vector<unique_ptr<DecDefAST>> decDefList;
    void add(DecDefAST* decDef);
};

class IdDefAST;

class IdDefList;

class DecAST : public AST {
public:
    TYPE bType;
    bool isConst;
    vector<unique_ptr<IdDefAST>> defList;//声明表
    void setIdDefList(IdDefList* defList);
};

class FuncDefAST;

class DecDefAST : public AST {
public:
    unique_ptr<DecAST> Decl;
    unique_ptr<FuncDefAST> funcDef;

    void setDecl(DecAST* decl);
    void setDef(FuncDefAST* def);
};

class DimensionAST : public AST {
public:
    vector<unique_ptr<LowAST>> exps;

    void add(LowAST* exp);
};

class RelExpAST;

class EqExpAST : public AST {
public:
    unique_ptr<RelExpAST> high;
    unique_ptr<EqExpAST> low;
    EQ_OP op;
    void setExp(EqExpAST* low, EQ_OP op, RelExpAST* high);
};

class FuncParamAST : public AST {
public:
    TYPE bType;
    unique_ptr<string> id;
    bool isArray = false;
    vector<unique_ptr<LowAST>> arrays;

};

class FuncParamList;

class FuncDefAST : public AST {
public:
    TYPE funcType;
    unique_ptr<string> id;
    vector<unique_ptr<FuncParamAST>> funcParamList;
    unique_ptr<BlockAST> block = nullptr;

    void setId(string* id);

    void setFuncParamList(FuncParamList funcParamList);

    void setFuncType(TYPE funcType);

    void setBlock(BlockAST* block);

};

/*class FuncParamAST : public AST {
public:
    TYPE bType;
    unique_ptr<string> id;
    bool isArray = false;
    vector<unique_ptr<LowAST>> arrays;

};*/

class FuncParamList : public AST {
public:
    vector<unique_ptr<FuncParamAST>> list;
    void add(FuncParamAST* funcParamAst);
};

class FuncRParamList : public AST {
public:
    vector<unique_ptr<LowAST>> realParams;
    void add(LowAST* realParams);
};

class UnaryAST;

//*,/,%
class HighAST :public AST {
public:
    HIGH_OP op;
    unique_ptr<UnaryAST> unary;
    unique_ptr<HighAST> high;

    void setOp(HIGH_OP op);
    void setModExp(HighAST* factor);
    void setMulExp(HighAST* factor);
    void setDivExp(HighAST* factor);
    void setUnaryExp(UnaryAST* unary);

};

class InitValAST;

class IdDefAST : public AST {
public:
    unique_ptr<string> id;
    unique_ptr<InitValAST> initVal;
    vector<unique_ptr<LowAST>> dimensions;

    void setId(string* id);

    void setInitVal(InitValAST* initVal);

    void setDimensions(DimensionAST* dimensions);

};

class IdDefList {
public:
    vector<unique_ptr<IdDefAST>> defList;
    IdDefList() = default;
    void add(IdDefAST* idDefAst);
};

class InitValList;

class InitValAST : public AST {
public:
    unique_ptr<LowAST> exp;
    vector<unique_ptr<InitValAST>> initValList;
    void setExpression(LowAST* exp);
    void setInitValList(InitValList* list);

};

class OrAST;

class StmtAST;

class IterationStmtAST : public AST {
public:
    unique_ptr<StmtAST> stmt;
    unique_ptr<OrAST> cond;

    void setStmt(StmtAST* stmt);
    void setCondition(OrAST* condition);
};

class InitValList {
public:
    vector<unique_ptr<InitValAST>> initValList;
    InitValList() = default;
    void add(InitValAST* initVal);
};

class LeftValAST : public AST {
public:
    unique_ptr<string> id;
    vector<unique_ptr<LowAST>> dimensions;
    void setId(string* id);
    void setDimensions(DimensionAST* dimensions);
};

class NumberAST : public AST {
public:
    bool isInt;
    union {
        int int_t;
        float float_t;
    };
};

//+,-
class LowAST : public AST {
public:
    unique_ptr<LowAST> low;
    unique_ptr<HighAST> high;
    LOW_OP op;
    void setExpr(LowAST* low, LOW_OP op, HighAST* high);
    void setHighExp(HighAST* high);
};

class OrAST : public AST {
public:
    unique_ptr<OrAST> low;
    unique_ptr<AndAST> high;

    void setExp(OrAST* low, AndAST* high);
};

class PrimaryExpAST : public AST {
public:
    unique_ptr<LeftValAST> leftVal;
    unique_ptr<LowAST> exp;
    unique_ptr<NumberAST> number;
    void setLeftVal(LeftValAST*);
    void setExpression(LowAST*);
    void setNumber(NumberAST*);
};

class RelExpAST : public AST {
public:
    unique_ptr<RelExpAST> low;
    unique_ptr<LowAST> high;
    REL_OP op;
    void setExp(RelExpAST* low, REL_OP op, LowAST* high);
};

class ReturnStmtAST : public AST {
public:
    unique_ptr<LowAST> exp;
    void setExpression(LowAST* expression);
};

class SelectStmtAST : public AST {
public:
    unique_ptr<StmtAST> ifStmt;
    unique_ptr<StmtAST> elseStmt;

    unique_ptr<OrAST> cond;

    void setIf(StmtAST* ifStmt);
    void setElse(StmtAST* elseStmt);
    void setCondition(OrAST* cond);
};

class SentenceAST : public AST {
public:
    unique_ptr<DecAST> decl;
    unique_ptr<StmtAST> stmt;
    void setDecl(DecAST* decl);
    void setStmt(StmtAST* stmt);
};

class Sentences : public AST {
public:
    vector<unique_ptr<SentenceAST>> sentences;
    void add(SentenceAST* sentence);
};

class StmtAST : public AST {
public:
    STMT_TYPE  stmtType;

    unique_ptr<ReturnStmtAST> ret = nullptr;
    unique_ptr<SelectStmtAST> sel = nullptr;
    unique_ptr<IterationStmtAST> itr = nullptr;

    unique_ptr<BlockAST> block = nullptr;
    unique_ptr<LeftValAST> leftVal = nullptr;
    unique_ptr<LowAST> exp = nullptr;

    void setLeftVal(LeftValAST* leftVal);
    void setStmtType(STMT_TYPE stmtType);
    void setExpression(LowAST* exp);
    void setBlock(BlockAST* block);
    void setReturn(ReturnStmtAST* returnStmt);
    void setSelect(SelectStmtAST* select);
    void setIteration(IterationStmtAST* iteration);

};

class UnaryAST : public AST {
public:
    unique_ptr<PrimaryExpAST> primaryExp;
    unique_ptr<CallAST> call;
    unique_ptr<UnaryAST> unary;
    U_OP op;

    void setOp(U_OP op);

    void setPrimaryExp(PrimaryExpAST* primaryExp);
    void setCall(CallAST* call);
    void setUnary(UnaryAST* unaryAst);

};

class Visitor {
public:
    //重写该方法
    virtual void visit(AST& ast) = 0;
    //TODO:增加相应的遍历方法
};


#endif //COMPILER_AST_H
