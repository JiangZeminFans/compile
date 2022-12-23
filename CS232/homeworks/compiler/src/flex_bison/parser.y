%define parse.error verbose
%locations

%{
    #include "../base.h"
    #include "../ast/all_ast.h"
    #include <memory>
    #include<stdarg.h>

    extern int yylineno;
    extern int yylex();
    extern void yyerror(const char* s);

    unique_ptr<CompUnitAST> root;
%}

%union{
    U_OP op;

    TYPE type;

    string* id;
    int int_t;
    float float_t;

    DecDefAST* decDef;
    CompUnitAST* compUnit;
    DecAST* decl;
    IdDefList* idDefList;
    IdDefAST* idDefAST;
    DimensionAST* dimensions;
    InitValAST* initVal;
    InitValList* initValList;
    FuncDefAST* funcDef;
    FuncParamList* funcL;
    FuncParamAST* funcP;
    BlockAST* block;
    Sentences* sentences;
    SentenceAST* sentence;
    StmtAST* stmt;
    ReturnStmtAST* returnStmt;
    IterationStmtAST* iteration;
    SelectStmtAST* select;
    LeftValAST* lVal;
    PrimaryExpAST* primary;
    NumberAST* number;
    UnaryAST* unary;
    CallAST* call;
    FuncRParamList* funcR;
    HighAST* mul;
    LowAST* add;
    RelExpAST* rel;
    EqExpAST* eq;
    AndAST* andExp;
    OrAST* orExp;
};

%type <compUnit> CompUnit
%type <decDef> DecDef
%type <decl> Decl
%type <idDefList> IdDefList
%type <idDefAST> IdDef
%type <type> BType VoidType
%type <dimensions> Dimensions
%type <initVal> InitVal
%type <initValList> InitValList
%type <funcDef>  FuncDef
%type <block> Block
%type <sentences> Sentences
%type <sentence> Sentence
%type <stmt> Stmt
%type <returnStmt> ReturnStmt
%type <iteration> IterationStmt
%type <select> SelectStmt
%type <lVal> LVal
%type <primary> PrimaryExp
%type <number> Number
%type <unary> UnaryExp
%type <call> Call
%type <funcL> FuncParamList
%type <funcP> FuncParam
%type <funcR> FuncRParamList
%type <mul> MulExp
%type <add> Expression AddExp
%type <rel> RelExp
%type <eq> EqExp
%type <andExp> AndExp
%type <orExp> Cond OrExp
%type <op> UnaryOp

//终结符
%token <int_t> NUM
%token <float_t> REAL
%token <id> ID

%token GT LT GE LE EQ NE
%token INT FLOAT VOID
%token CONST RET
%token IF ELSE WHILE BREAK CONTINUE
%token LS RS LM RM LB RB
//;
%token SEMI
//,
%token COMMA



%left ASSIGN
%left OR AND
%left EQ NE
%left GE LE GT LT
%left ADD MINUS
%left MOD MUL DIV
%right NOT POS NEG

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%start program

%%

program: CompUnit{  root=unique_ptr<CompUnitAST>($1);}
            ;

CompUnit: CompUnit DecDef   {
            $$=$1;
            $$->add($2);
            }
           |DecDef  {
                $$=new CompUnitAST();
                $$->add($1);
           }
            ;

DecDef:Decl{
            $$=new DecDefAST();
            $$->setDec($1);
        }|FuncDef {
            $$=new DecDefAST();
            $$->setDef($1);
        }
        ;
/*
类型定义
*/

BType: INT{
            $$=INT_T;
        }|FLOAT{
            $$=FLOAT_T;
        }
        ;
VoidType: VOID{
                $$=VOID_T;
          }
          ;

/*
*变量/常量 定义与声明
*/

//const int a,b,c,d;
Decl:CONST BType IdDefList SEMI{
            $$=new DecAST();
            $$->isConst=true;
            $$->bType=$2;
            $$->setIdDefList($3);
        }|BType IdDefList SEMI{
            $$=new DecAST();
            $$->bType=$1;
            $$->setIdDefList($2);

        }
        ;

//标识符定义列表 a,b,c
IdDefList:IdDef{
        $$=new DefListAST();
        $$->add($1);
    }|IdDefList COMMA IdDef{
        $$=$1;
        $$->add($3);
    }
    ;
//a[10]={1,2,3}
IdDef:  ID Dimensions ASSIGN InitVal{
            $$=new IdDefAST();
            $$->setId($1);
            $$->setDimensions($2);
            $$->setInitVal($4);
        }
        |ID Dimensions{
            $$=new IdDefAST();
            $$->setId($1);
            $$->setDimensions($2);
        }
        |ID ASSIGN InitVal{
            $$=new IdDefAST();
            $$->setId($1);
            $$->setInitVal($3);
        }|ID{
            $$=new IdDefAST();
            $$->setId($1);
        }
        ;
//[10]
Dimensions:LM Expression RM {  //[10]
            $$=new DimensionAST();
            $$->add($2);
        }|Dimensions LM Expression RM{  //[10][20]
            $$=$1;
            $$->add($3);
        }
        ;
InitVal: Expression{
            $$=new InitValAST();
            $$->setExpression($1);
            }|LB RB{    //{}
                $$=new InitValAST();
            }|LB InitValList RB{    //{1,23,2}
                $$=new InitValAST();
                $$->setInitValList($2);
            }
            ;
//1,2,345,64
InitValList:InitVal{
                $$=new InitValList();
                $$->add($1)
            }|InitValList COMMA InitVal{
                $$=$1;
                $$->add($3);
            }
            ;

/*
函数定义
*/

FuncDef: BType ID LS FuncParamList RS Block{
            $$=new FuncDefAST();
            $$->setFuncType($1);
            $$->setId($2);
            $$->setFuncParamList($4);
            $$->setBlock($6);
        }|VoidType ID LS FuncParamList RS Block{
            $$=new FuncDefAST();
            $$->setFuncType($1);
            $$->setId($2);
            $$->setFuncParamList($4);
            $$->setBlock($6);
        }|BType ID LS RS Block{
            $$=new FuncDefAST();
            $$->setFuncType($1);
            $$->setId($2);
            $$->setBlock($5);
        }|VoidType ID LS RS Block{
            $$=new FuncDefAST();
            $$->setFuncType($1);
            $$->setId($2);
            $$->setBlock($5);
        }
        ;
FuncParamList:FuncParam{
            $$=new FuncParamList();
            $$->add($1);
         }|FuncParamList FuncParam{
            $$=$1;
            $$->add($2);
         }
         ;
FuncParam:      BType ID {
          				$$ = new FuncParamAST();
          				$$->bType = $1;
          				$$->id = unique_ptr<string>($2);
          				$$->isArray = false;
          			}|BType ID LM RM	{
          				$$ = new FuncFParamAST();
          				$$->bType = $1;
          				$$->id = unique_ptr<string>($2);
          				$$->isArray = true;
          			}|BType ID Dimensions {
          				$$ = new FuncFParamAST();
          				$$->bType = $1;
          				$$->id = unique_ptr<string>($2);
          				$$->isArray = true;
          				$$->arrays.swap($3->exps);
          			}
          			;
Block: LB RB{
            $$=new BlockAST();
        }|LB Sentences RB{
            $$=new BlockAST();
            $$->setSentences($2);
        }
        ;

Sentences:Sentence{
             $$=new Sentences();
             $$->add($1);
         }|Sentences Sentence{
            $$=$1;
            %%->add($1);
         }
         ;
Sentence:Decl{
            $$=new SentenceAST();
            $$->setDecl($1);
         }|Stmt{
            $$=new SentenceAST();
            $$->setStmt($1);
         }
         ;

/*
statement
*/
Stmt:SEMI{  //;
            $$=new StmtAST();
            $$->setStmtType(SEMI_T);
        }|CONTINUE SEMI{    //continue;
            $$=new StmtAST();
            $$->setStmtType(CONT_T);
        }|BREAK SEMI{   //break;
            $$=new StmtAST();
            $$->setStmtType(BRK_T);
        }|LVal ASSIGN Expression SEMI{  //a=10;
            $$=new StmtAST();
            $$->setExpression($3);
            $$->setLeftVal($1);//不可翻转
        }|Expression SEMI{
            $$=new StmtAST();
            $$->setExpression($1);
        }|Block{
 			$$ = new StmtAST();
 			$$->setBlock($1);
        }|ReturnStmt{
            $$ = new StmtAST();
            $$->setReturn($1);
        }|SelectStmt{
            $$=new StmtAST();
            $$=setSelect($1);
        }|IterationStmt{
            $$=new StmtAST();
            $$->setIteration($1);
        }
        ;
//返回
ReturnStmt: RET Expression SEMI{
                $$=new ReturnStmtAST();
                $$->setExpression($2);
            }|RET SEMI{
                $$=new ReturnStmtAST();
            }
            ;
//选择
SelectStmt: IF LS Cond RS Stmt %prec LOWER_THAN_ELSE{
                $$=new SelectStmtAST();
                $$->setCondition($3);
                $$->setIf($5);
            }|IF LS Cond RS Stmt ELSE Stmt{
                $$=new SelectStmtAST();
                $$->setCondition($3);
                $$->setIf($5);
                $$->setElse($7);
            }
            ;
//循环
IterationStmt:  WHILE LS Cond RS Stmt {
					$$ = new IterationStmtAST();
					$$->setCondition($3);
					$$->setStmt($5);
				}
                ;

/*
表达式
*/

//普通
Expression: AddExp{
                $$=$1;
            }
            ;
//逻辑
Cond:       OrExp{
                $$=$1;
            }
            ;
//a[0]/x
LVal:   ID{
            $$=new LeftValAST();
            $$->setId($1);
           }|ID Dimensions{
             $$=new LeftValAST();
             $$->setId($1);
             $$->setDimensions($2);
           }
           ;
//基本表达式
PrimaryExp: LS Expression RS{
                $$ = new PrimaryExpAST();
                $$->setExpression($2);
            }|LVal{
                $$ = new PrimaryExpAST();
                $$->setLeftVal($1);
            }|Number{
                $$ = new PrimaryExpAST();
                $$->setNumber($1);
            }

//123548
Number: NUM{
            $$=new NumberAST();
			$$->isInt = true;
			$$->int_t = $1;
        }|REAL{
			$$ = new NumberAST();
			$$->isInt = false;
			$$->float_t = $1;
        }
        ;


//单目
UnaryExp:   PrimaryExp{
				$$ = new UnaryExpAST();
				$$->setPrimaryExp($1)
            }|Call{
				$$ = new UnaryExpAST();
				$$->setCall($1)
            }|UnaryOp UnaryExp{
				$$ = new UnaryExpAST();
				$$->setOp($1)
				$$->setUnary($2)
            }
            ;

Call:   ID LS RS{
                $$ = new CallAST();
                $$->setId($1);
        }|ID LS FuncRParamList RS{
		        $$ = new CallAST();
        }
        ;

UnaryOp:    ADD{
                $$=U_POS;
            }|MINUS{
                $$=U_NEG;
            }|NOT{
                $$=U_NOT;
            }
            ;

FuncRParamList: Expression{
                    $$=new FuncRParamList();
                    $$->add($1);
                }|FuncRParamList COMMA Expression{
                    $$=$1;
                    $$->add($3);
                }
                ;

MulExp: UnaryExp{
            $$=new HighAST();
            $$->setUnaryAST($1);
        }|MulExp MUL UnaryExp{
             $$=new HighAST();
             $$->setMulExp($1);
             $$->setUnaryExp($3);
        }|MulExp DIV UnaryExp {
             $$=new HighAST();
             $$->setDivExp($1);
             $$->setUnaryExp($3);
        }|MulExp MOD UnaryExp {
             $$=new HighAST();
             $$->setModExp($1);
             $$->setUnaryExp($3);
        }
        ;

AddExp: MulExp{
			$$ = new LowAST();
			$$->setHighExp($1);
        }|AddExp ADD MulExp {
			$$ = new LowAST();
			$$->setExpr($1,ADD_OP,$3);
        }|AddExp MINUS MulExp {
			$$ = new LowAST();
			$$->setExpr($1,MINUS_OP,$3);
        }
        ;
RelExp: AddExp{
            $$ = new RelExpAST();
            $$->high=unique_ptr<LowAST>($1);
        }|RelExp GE AddExp{
 			$$ = new RelExpAST();
 			$$->setExp($1,GE_REL,$3)
        }|RelExp LE AddExp{
			$$ = new RelExpAST();
			$$->setExp($1,LE_REL,$3)
        }|RelExp GT AddExp {
			$$ = new RelExpAST();
			$$->setExp($1,GT_REL,$3)
        }|RelExp LT AddExp {
			$$ = new RelExpAST();
            $$->setExp($1,LT_REL,$3)
        }
        ;
EqExp: RelExp{
            $$=new EqExpAST();
            $$_>high=unique_ptr<RelExpAST>($1);
        }|EqExp EQ RelExp{
            $$=new EqExpAST();
            $$->setExp($1,EQ_REL,$3);
        }|EqExp NE RelExp{
            $$=new EqExpAST();
            $$->setExp($1,NE_REL,$3);
        }
        ;

AndExp: EqExp {
            $$ = new AndAST();
            $$->high = unique_ptr<EqExpAST>($1);
        }|AndExp AND EqExp {
            $$ = new AndAST();
            $$->setExp($1,$3);
        }
        ;
OrExp:  AndExp {
            $$ = new OrAST();
            $$->high = unique_ptr<AndAST>($1);
        }|OrExp OR AndExp {
            $$ = new OrAST();
            $$->setExp($1,$3);
        }
%%

void yyerror(const char* fmt) {
    printf("line:%d\tcolumn:%d\n", yylloc.first_line,yylloc.first_column);
    printf("ERROR: %s\n", fmt);
}




