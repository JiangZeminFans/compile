%option noyywrap yylineno

%{
    #include "string.h"
    #include "parser.hpp"
%}

%x COMMENT

UCN (\\U[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
EXP ([Ee][-+]?[0-9]+)

%%

/*整数*/
0[0-7]*     {yylval.int_t=strtol(yytext,nullptr,0);return NUM;}
[1-9][0-9]* {yylval.int_t=strtol(yytext,nullptr,0);return NUM;}
0[Xx][0-9a-fA-F]+   {yylval.int_t=strtol(yytext,nullptr,0);return NUM;}

/*十进制小数*/
([0-9]*\.[0-9]+|[0-9]+\.){EXP}? {yylval.float_t=strtof(yytext,nullptr);return REAL;}
[0-9]+{EXP} {yylval.float_t=strtof(yytext,nullptr);return REAL;}
/*十六进制小数*/
0[Xx]([0-9a-fA-F]*\.[0-9a-fA-F]+)|[0-9a-fA-F]+|([0-9a-fA-F]+\.?)[Pp][-+]?[0-9]+ {yylval.float_t=strtof(yytext,nullptr);return REAL;}

/*注释*/
"/*"    {BEGIN(COMMENT);}
<COMMENT>"*/" {BEGIN(INITIAL);}
<COMMENT>([^*]|\n)+|.
<COMMENT><<EOF>>    {printf("%d:unterminated comment",yylineno);return 0;}

/*单行注释*/
”//“.*\n    {yylineno++;}

/*关键字*/
"int"  {  return INT;  }

"float" { return FLOAT; }

"const" { return CONST; }

"void" { return VOID; }

"return"  {  return RET;  }

"continue"  {  return CONTINUE; }

"break" { return BREAK; }

"if"  {  return IF;  }

"else"  {  return ELSE;  }

"while"  {  return WHILE;  }

/*操作符*/
"&&" {  return AND;}

"||" { return OR;}

"!"  { return NOT;}

"=="  {  return EQ;  }

"!="  {  return NE;  }

"<="  {  return LE;  }

"<"  {  return LT;  }

">="  {  return GE;  }

">"  {  return GT;  }

"+"  {  return ADD; }

"-"  {  return SUB; }

"*"  {  return MUL; }

"/"  {  return DIV; }

";"  {  return SEMI; }

"%"  {  return MOD; }

"="  {  return ASSIGN; }

","  {  return COMMA; }

"("  {  return LS; }

")"  {  return RS; }

"["  {  return LM; }

"]"  {  return RM; }

"{"  {  return LB; }

"}"  {  return RB; }


/*标识符*/
([_a-zA-Z]|{UCN})([_a-zA-Z0-9]|{UCN})*  {yylval.id = new string(yytext);return ID;}

/*换行*/
[\n] {yylineno++;}

/*空白字符*/
[ \t]+

/*非法*/
.   {printf("行%d:非法输入：%s",yylineno,yytext);exit(0)}

%%