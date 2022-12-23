/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 4 "parser.y"

    #include "parser.hpp"

    extern int yylineno;
    extern int yylex();
    extern void yyerror(const char* s);

    unique_ptr<CompUnitAST> root;

#line 83 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUM = 258,
    REAL = 259,
    ID = 260,
    GT = 261,
    LT = 262,
    GE = 263,
    LE = 264,
    EQ = 265,
    NE = 266,
    INT = 267,
    FLOAT = 268,
    VOID = 269,
    CONST = 270,
    RET = 271,
    IF = 272,
    ELSE = 273,
    WHILE = 274,
    BREAK = 275,
    CONTINUE = 276,
    LS = 277,
    RS = 278,
    LM = 279,
    RM = 280,
    LB = 281,
    RB = 282,
    SEMI = 283,
    COMMA = 284,
    ASSIGN = 285,
    OR = 286,
    AND = 287,
    ADD = 288,
    MINUS = 289,
    MOD = 290,
    MUL = 291,
    DIV = 292,
    NOT = 293,
    POS = 294,
    NEG = 295,
    LOWER_THAN_ELSE = 296
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "parser.y"

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

#line 218 "parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   233

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  160

#define YYUNDEFTOK  2
#define YYMAXUTOK   296


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   123,   123,   126,   130,   136,   139,   148,   150,   154,
     164,   169,   178,   181,   187,   193,   198,   202,   208,   211,
     216,   219,   221,   227,   230,   240,   246,   252,   257,   264,
     267,   272,   277,   282,   290,   292,   298,   301,   306,   309,
     318,   321,   324,   327,   331,   334,   337,   340,   343,   349,
     352,   357,   361,   369,   381,   386,   391,   394,   401,   404,
     407,   413,   417,   426,   429,   432,   439,   442,   447,   449,
     451,   456,   459,   465,   468,   472,   476,   483,   486,   489,
     494,   497,   500,   503,   506,   511,   514,   517,   523,   526,
     531,   534
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "REAL", "ID", "GT", "LT", "GE",
  "LE", "EQ", "NE", "INT", "FLOAT", "VOID", "CONST", "RET", "IF", "ELSE",
  "WHILE", "BREAK", "CONTINUE", "LS", "RS", "LM", "RM", "LB", "RB", "SEMI",
  "COMMA", "ASSIGN", "OR", "AND", "ADD", "MINUS", "MOD", "MUL", "DIV",
  "NOT", "POS", "NEG", "LOWER_THAN_ELSE", "$accept", "program", "CompUnit",
  "DecDef", "BType", "VoidType", "Decl", "IdDefList", "IdDef",
  "Dimensions", "InitVal", "InitValList", "FuncDef", "FuncParamList",
  "FuncParam", "Block", "Sentences", "Sentence", "Stmt", "ReturnStmt",
  "SelectStmt", "IterationStmt", "Expression", "Cond", "LVal",
  "PrimaryExp", "Number", "UnaryExp", "Call", "UnaryOp", "FuncRParamList",
  "MulExp", "AddExp", "RelExp", "EqExp", "AndExp", "OrExp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296
};
# endif

#define YYPACT_NINF (-126)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     178,  -126,  -126,  -126,    82,    10,   178,  -126,     8,    20,
    -126,  -126,    36,  -126,  -126,    -2,   101,  -126,    22,    -6,
     168,    49,   167,    51,    -1,  -126,    36,    67,  -126,    32,
      58,   105,  -126,  -126,  -126,    77,   167,  -126,  -126,  -126,
      46,  -126,  -126,  -126,  -126,  -126,   167,   124,   169,    12,
    -126,  -126,   167,    51,  -126,    32,   144,    93,  -126,    54,
      32,  -126,   140,    78,    84,  -126,  -126,   167,   167,   167,
     167,   167,  -126,  -126,   150,    86,  -126,  -126,    32,   147,
      94,   100,   106,   110,  -126,  -126,    36,  -126,  -126,   120,
    -126,  -126,  -126,  -126,  -126,   121,   125,   161,    78,  -126,
    -126,  -126,    64,  -126,  -126,  -126,  -126,   124,   124,  -126,
      51,  -126,  -126,  -126,   148,   167,   167,  -126,  -126,  -126,
    -126,  -126,   167,  -126,  -126,   167,  -126,  -126,   145,   169,
     200,   208,   152,   173,   159,   170,  -126,    48,   167,   167,
     167,   167,   167,   167,   167,   167,    48,  -126,   202,   169,
     169,   169,   169,   200,   200,   208,   152,  -126,    48,  -126
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     7,     8,     9,     0,     0,     2,     4,     0,     0,
       5,     6,     0,     1,     3,    17,     0,    12,     0,    17,
       0,     0,     0,     0,    15,    11,     0,     0,    10,     0,
       0,     0,    29,    61,    62,    56,     0,    68,    69,    70,
       0,    59,    63,    60,    73,    64,     0,    77,    54,     0,
      16,    20,     0,     0,    13,     0,     0,     0,    27,    31,
       0,    30,     0,    57,     0,    18,    65,     0,     0,     0,
       0,     0,    21,    23,     0,     0,    14,    28,     0,     0,
       0,     0,     0,     0,    34,    40,     0,    38,    45,     0,
      36,    39,    46,    47,    48,     0,    59,     0,    33,    25,
      66,    71,     0,    58,    76,    74,    75,    78,    79,    22,
       0,    19,    26,    50,     0,     0,     0,    42,    41,    35,
      37,    44,     0,    32,    67,     0,    24,    49,     0,    80,
      85,    88,    90,    55,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    51,    83,
      84,    81,    82,    86,    87,    89,    91,    53,     0,    52
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -126,  -126,  -126,   219,     2,  -126,   -53,   214,   201,   -16,
     -44,  -126,  -126,   203,   -24,   -18,  -126,   139,  -125,  -126,
    -126,  -126,   -22,   113,   -54,  -126,  -126,   -20,  -126,  -126,
    -126,   151,    72,    81,    87,    88,  -126
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,    30,     9,    10,    16,    17,    24,
      50,    74,    11,    31,    32,    88,    89,    90,    91,    92,
      93,    94,    95,   128,    41,    42,    43,    44,    45,    46,
     102,    47,    48,   130,   131,   132,   133
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      40,    51,     8,    96,    87,    73,    12,    61,     8,    76,
      13,    58,   148,    15,    64,    33,    34,    35,    22,    63,
      21,   157,    22,    52,    23,    18,    66,    51,    23,    53,
      75,    51,    61,   159,    36,    96,    87,    77,    49,    72,
     101,    19,    99,    98,    27,    37,    38,   104,   105,   106,
      39,    33,    34,    35,    33,    34,    35,   114,    57,    86,
     112,     1,     2,    59,    79,    80,   126,    81,    82,    83,
      36,    65,    29,    36,    57,    40,    85,    49,    97,     1,
       2,    37,    38,    96,    37,    38,    39,   124,    51,    39,
      55,    86,    96,   125,     1,     2,    33,    34,    35,    62,
     135,    22,    52,   136,    96,     1,     2,   103,     4,    79,
      80,   111,    81,    82,    83,    36,   115,     1,     2,    57,
      84,    85,   116,    33,    34,    35,    37,    38,    60,    25,
      26,    39,     1,     2,   117,     4,    79,    80,   118,    81,
      82,    83,    36,    33,    34,    35,    57,   119,    85,   121,
      33,    34,    35,    37,    38,   122,     1,     2,    39,    67,
      68,    69,    36,   100,    33,    34,    35,    78,   137,    36,
      33,    34,    35,    37,    38,   113,   127,   109,    39,   110,
      37,    38,   146,    36,   144,    39,   123,   129,   129,    36,
       1,     2,     3,     4,    37,    38,    28,    26,   147,    39,
      37,    38,    70,    71,   145,    39,   138,   139,   140,   141,
     149,   150,   151,   152,   129,   129,   129,   129,   142,   143,
     158,   107,   108,   153,   154,    14,    20,    54,   120,   134,
      56,   155,     0,   156
};

static const yytype_int16 yycheck[] =
{
      22,    23,     0,    57,    57,    49,     4,    31,     6,    53,
       0,    29,   137,     5,    36,     3,     4,     5,    24,    35,
      22,   146,    24,    24,    30,     5,    46,    49,    30,    30,
      52,    53,    56,   158,    22,    89,    89,    55,    26,    27,
      62,     5,    60,    59,    22,    33,    34,    67,    68,    69,
      38,     3,     4,     5,     3,     4,     5,    79,    26,    57,
      78,    12,    13,     5,    16,    17,   110,    19,    20,    21,
      22,    25,    23,    22,    26,    97,    28,    26,    24,    12,
      13,    33,    34,   137,    33,    34,    38,    23,   110,    38,
      23,    89,   146,    29,    12,    13,     3,     4,     5,    22,
     122,    24,    24,   125,   158,    12,    13,    23,    15,    16,
      17,    25,    19,    20,    21,    22,    22,    12,    13,    26,
      27,    28,    22,     3,     4,     5,    33,    34,    23,    28,
      29,    38,    12,    13,    28,    15,    16,    17,    28,    19,
      20,    21,    22,     3,     4,     5,    26,    27,    28,    28,
       3,     4,     5,    33,    34,    30,    12,    13,    38,    35,
      36,    37,    22,    23,     3,     4,     5,    23,    23,    22,
       3,     4,     5,    33,    34,    28,    28,    27,    38,    29,
      33,    34,    23,    22,    32,    38,    25,   115,   116,    22,
      12,    13,    14,    15,    33,    34,    28,    29,    28,    38,
      33,    34,    33,    34,    31,    38,     6,     7,     8,     9,
     138,   139,   140,   141,   142,   143,   144,   145,    10,    11,
      18,    70,    71,   142,   143,     6,    12,    26,    89,   116,
      27,   144,    -1,   145
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    12,    13,    14,    15,    43,    44,    45,    46,    47,
      48,    54,    46,     0,    45,     5,    49,    50,     5,     5,
      49,    22,    24,    30,    51,    28,    29,    22,    28,    23,
      46,    55,    56,     3,     4,     5,    22,    33,    34,    38,
      64,    66,    67,    68,    69,    70,    71,    73,    74,    26,
      52,    64,    24,    30,    50,    23,    55,    26,    57,     5,
      23,    56,    22,    51,    64,    25,    69,    35,    36,    37,
      33,    34,    27,    52,    53,    64,    52,    57,    23,    16,
      17,    19,    20,    21,    27,    28,    46,    48,    57,    58,
      59,    60,    61,    62,    63,    64,    66,    24,    51,    57,
      23,    64,    72,    23,    69,    69,    69,    73,    73,    27,
      29,    25,    57,    28,    64,    22,    22,    28,    28,    27,
      59,    28,    30,    25,    23,    29,    52,    28,    65,    74,
      75,    76,    77,    78,    65,    64,    64,    23,     6,     7,
       8,     9,    10,    11,    32,    31,    23,    28,    60,    74,
      74,    74,    74,    75,    75,    76,    77,    60,    18,    60
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    45,    46,    46,    47,
      48,    48,    49,    49,    50,    50,    50,    50,    51,    51,
      52,    52,    52,    53,    53,    54,    54,    54,    54,    55,
      55,    56,    56,    56,    57,    57,    58,    58,    59,    59,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    61,
      61,    62,    62,    63,    64,    65,    66,    66,    67,    67,
      67,    68,    68,    69,    69,    69,    70,    70,    71,    71,
      71,    72,    72,    73,    73,    73,    73,    74,    74,    74,
      75,    75,    75,    75,    75,    76,    76,    76,    77,    77,
      78,    78
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       4,     3,     1,     3,     4,     2,     3,     1,     3,     4,
       1,     2,     3,     1,     3,     6,     6,     5,     5,     1,
       2,     2,     4,     3,     2,     3,     1,     2,     1,     1,
       1,     2,     2,     4,     2,     1,     1,     1,     1,     3,
       2,     5,     7,     5,     1,     1,     1,     2,     3,     1,
       1,     1,     1,     1,     1,     2,     3,     4,     1,     1,
       1,     1,     3,     1,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 123 "parser.y"
                 {  root=unique_ptr<CompUnitAST>((yyvsp[0].compUnit));}
#line 1633 "parser.cpp"
    break;

  case 3:
#line 126 "parser.y"
                            {
            (yyval.compUnit)=(yyvsp[-1].compUnit);
            (yyval.compUnit)->add((yyvsp[0].decDef));
            }
#line 1642 "parser.cpp"
    break;

  case 4:
#line 130 "parser.y"
                    {
                (yyval.compUnit)=new CompUnitAST();
                (yyval.compUnit)->add((yyvsp[0].decDef));
           }
#line 1651 "parser.cpp"
    break;

  case 5:
#line 136 "parser.y"
           {
            (yyval.decDef)=new DecDefAST();
            (yyval.decDef)->setDec((yyvsp[0].decl));
        }
#line 1660 "parser.cpp"
    break;

  case 6:
#line 139 "parser.y"
                  {
            (yyval.decDef)=new DecDefAST();
            (yyval.decDef)->setDef((yyvsp[0].funcDef));
        }
#line 1669 "parser.cpp"
    break;

  case 7:
#line 148 "parser.y"
          {
            (yyval.type)=INT_T;
        }
#line 1677 "parser.cpp"
    break;

  case 8:
#line 150 "parser.y"
               {
            (yyval.type)=FLOAT_T;
        }
#line 1685 "parser.cpp"
    break;

  case 9:
#line 154 "parser.y"
              {
                (yyval.type)=VOID_T;
          }
#line 1693 "parser.cpp"
    break;

  case 10:
#line 164 "parser.y"
                               {
            (yyval.decl)=new DecAST();
            (yyval.decl)->isConst=true;
            (yyval.decl)->bType=(yyvsp[-2].type);
            (yyval.decl)->setIdDefList((yyvsp[-1].idDefList));
        }
#line 1704 "parser.cpp"
    break;

  case 11:
#line 169 "parser.y"
                              {
            (yyval.decl)=new DecAST();
            (yyval.decl)->bType=(yyvsp[-2].type);
            (yyval.decl)->setIdDefList((yyvsp[-1].idDefList));

        }
#line 1715 "parser.cpp"
    break;

  case 12:
#line 178 "parser.y"
               {
        (yyval.idDefList)=new DefListAST();
        (yyval.idDefList)->add((yyvsp[0].idDefAST));
    }
#line 1724 "parser.cpp"
    break;

  case 13:
#line 181 "parser.y"
                           {
        (yyval.idDefList)=(yyvsp[-2].idDefList);
        (yyval.idDefList)->add((yyvsp[0].idDefAST));
    }
#line 1733 "parser.cpp"
    break;

  case 14:
#line 187 "parser.y"
                                    {
            (yyval.idDefAST)=new IdDefAST();
            (yyval.idDefAST)->setId((yyvsp[-3].id));
            (yyval.idDefAST)->setDimensions((yyvsp[-2].dimensions));
            (yyval.idDefAST)->setInitVal((yyvsp[0].initVal));
        }
#line 1744 "parser.cpp"
    break;

  case 15:
#line 193 "parser.y"
                      {
            (yyval.idDefAST)=new IdDefAST();
            (yyval.idDefAST)->setId((yyvsp[-1].id));
            (yyval.idDefAST)->setDimensions((yyvsp[0].dimensions));
        }
#line 1754 "parser.cpp"
    break;

  case 16:
#line 198 "parser.y"
                          {
            (yyval.idDefAST)=new IdDefAST();
            (yyval.idDefAST)->setId((yyvsp[-2].id));
            (yyval.idDefAST)->setInitVal((yyvsp[0].initVal));
        }
#line 1764 "parser.cpp"
    break;

  case 17:
#line 202 "parser.y"
            {
            (yyval.idDefAST)=new IdDefAST();
            (yyval.idDefAST)->setId((yyvsp[0].id));
        }
#line 1773 "parser.cpp"
    break;

  case 18:
#line 208 "parser.y"
                            {  //[10]
            (yyval.dimensions)=new DimensionAST();
            (yyval.dimensions)->add((yyvsp[-1].add));
        }
#line 1782 "parser.cpp"
    break;

  case 19:
#line 211 "parser.y"
                                     {  //[10][20]
            (yyval.dimensions)=(yyvsp[-3].dimensions);
            (yyval.dimensions)->add((yyvsp[-1].add));
        }
#line 1791 "parser.cpp"
    break;

  case 20:
#line 216 "parser.y"
                   {
            (yyval.initVal)=new InitValAST();
            (yyval.initVal)->setExpression((yyvsp[0].add));
            }
#line 1800 "parser.cpp"
    break;

  case 21:
#line 219 "parser.y"
                   {    //{}
                (yyval.initVal)=new InitValAST();
            }
#line 1808 "parser.cpp"
    break;

  case 22:
#line 221 "parser.y"
                               {    //{1,23,2}
                (yyval.initVal)=new InitValAST();
                (yyval.initVal)->setInitValList((yyvsp[-1].initValList));
            }
#line 1817 "parser.cpp"
    break;

  case 23:
#line 227 "parser.y"
                   {
                (yyval.initValList)=new InitValList();
                (yyval.initValList)->add((yyvsp[0].initVal))
            }
#line 1826 "parser.cpp"
    break;

  case 24:
#line 230 "parser.y"
                                       {
                (yyval.initValList)=(yyvsp[-2].initValList);
                (yyval.initValList)->add((yyvsp[0].initVal));
            }
#line 1835 "parser.cpp"
    break;

  case 25:
#line 240 "parser.y"
                                           {
            (yyval.funcDef)=new FuncDefAST();
            (yyval.funcDef)->setFuncType((yyvsp[-5].type));
            (yyval.funcDef)->setId((yyvsp[-4].id));
            (yyval.funcDef)->setFuncParamList((yyvsp[-2].funcL));
            (yyval.funcDef)->setBlock((yyvsp[0].block));
        }
#line 1847 "parser.cpp"
    break;

  case 26:
#line 246 "parser.y"
                                               {
            (yyval.funcDef)=new FuncDefAST();
            (yyval.funcDef)->setFuncType((yyvsp[-5].type));
            (yyval.funcDef)->setId((yyvsp[-4].id));
            (yyval.funcDef)->setFuncParamList((yyvsp[-2].funcL));
            (yyval.funcDef)->setBlock((yyvsp[0].block));
        }
#line 1859 "parser.cpp"
    break;

  case 27:
#line 252 "parser.y"
                              {
            (yyval.funcDef)=new FuncDefAST();
            (yyval.funcDef)->setFuncType((yyvsp[-4].type));
            (yyval.funcDef)->setId((yyvsp[-3].id));
            (yyval.funcDef)->setBlock((yyvsp[0].block));
        }
#line 1870 "parser.cpp"
    break;

  case 28:
#line 257 "parser.y"
                                 {
            (yyval.funcDef)=new FuncDefAST();
            (yyval.funcDef)->setFuncType((yyvsp[-4].type));
            (yyval.funcDef)->setId((yyvsp[-3].id));
            (yyval.funcDef)->setBlock((yyvsp[0].block));
        }
#line 1881 "parser.cpp"
    break;

  case 29:
#line 264 "parser.y"
                       {
            (yyval.funcL)=new FuncParamList();
            (yyval.funcL)->add((yyvsp[0].funcP));
         }
#line 1890 "parser.cpp"
    break;

  case 30:
#line 267 "parser.y"
                                  {
            (yyval.funcL)=(yyvsp[-1].funcL);
            (yyval.funcL)->add((yyvsp[0].funcP));
         }
#line 1899 "parser.cpp"
    break;

  case 31:
#line 272 "parser.y"
                         {
          				(yyval.funcP) = new FuncParamAST();
          				(yyval.funcP)->bType = (yyvsp[-1].type);
          				(yyval.funcP)->id = unique_ptr<string>((yyvsp[0].id));
          				(yyval.funcP)->isArray = false;
          			}
#line 1910 "parser.cpp"
    break;

  case 32:
#line 277 "parser.y"
                                                        {
          				(yyval.funcP) = new FuncFParamAST();
          				(yyval.funcP)->bType = (yyvsp[-3].type);
          				(yyval.funcP)->id = unique_ptr<string>((yyvsp[-2].id));
          				(yyval.funcP)->isArray = true;
          			}
#line 1921 "parser.cpp"
    break;

  case 33:
#line 282 "parser.y"
                                                      {
          				(yyval.funcP) = new FuncFParamAST();
          				(yyval.funcP)->bType = (yyvsp[-2].type);
          				(yyval.funcP)->id = unique_ptr<string>((yyvsp[-1].id));
          				(yyval.funcP)->isArray = true;
          				(yyval.funcP)->arrays.swap((yyvsp[0].dimensions)->exps);
          			}
#line 1933 "parser.cpp"
    break;

  case 34:
#line 290 "parser.y"
            {
            (yyval.block)=new BlockAST();
        }
#line 1941 "parser.cpp"
    break;

  case 35:
#line 292 "parser.y"
                         {
            (yyval.block)=new BlockAST();
            (yyval.block)->setSentences((yyvsp[-1].sentences));
        }
#line 1950 "parser.cpp"
    break;

  case 36:
#line 298 "parser.y"
                  {
             (yyval.sentences)=new Sentences();
             (yyval.sentences)->add((yyvsp[0].sentence));
         }
#line 1959 "parser.cpp"
    break;

  case 37:
#line 301 "parser.y"
                             {
            (yyval.sentences)=(yyvsp[-1].sentences);
            %%->add((yyvsp[-1].sentences));
         }
#line 1968 "parser.cpp"
    break;

  case 38:
#line 306 "parser.y"
             {
            (yyval.sentence)=new SentenceAST();
            (yyval.sentence)->setDecl((yyvsp[0].decl));
         }
#line 1977 "parser.cpp"
    break;

  case 39:
#line 309 "parser.y"
               {
            (yyval.sentence)=new SentenceAST();
            (yyval.sentence)->setStmt((yyvsp[0].stmt));
         }
#line 1986 "parser.cpp"
    break;

  case 40:
#line 318 "parser.y"
         {  //;
            (yyval.stmt)=new StmtAST();
            (yyval.stmt)->setStmtType(SEMI_T);
        }
#line 1995 "parser.cpp"
    break;

  case 41:
#line 321 "parser.y"
                       {    //continue;
            (yyval.stmt)=new StmtAST();
            (yyval.stmt)->setStmtType(CONT_T);
        }
#line 2004 "parser.cpp"
    break;

  case 42:
#line 324 "parser.y"
                    {   //break;
            (yyval.stmt)=new StmtAST();
            (yyval.stmt)->setStmtType(BRK_T);
        }
#line 2013 "parser.cpp"
    break;

  case 43:
#line 327 "parser.y"
                                     {  //a=10;
            (yyval.stmt)=new StmtAST();
            (yyval.stmt)->setExpression((yyvsp[-1].add));
            (yyval.stmt)->setLeftVal((yyvsp[-3].lVal));//不可翻转
        }
#line 2023 "parser.cpp"
    break;

  case 44:
#line 331 "parser.y"
                         {
            (yyval.stmt)=new StmtAST();
            (yyval.stmt)->setExpression((yyvsp[-1].add));
        }
#line 2032 "parser.cpp"
    break;

  case 45:
#line 334 "parser.y"
               {
 			(yyval.stmt) = new StmtAST();
 			(yyval.stmt)->setBlock((yyvsp[0].block));
        }
#line 2041 "parser.cpp"
    break;

  case 46:
#line 337 "parser.y"
                    {
            (yyval.stmt) = new StmtAST();
            (yyval.stmt)->setReturn((yyvsp[0].returnStmt));
        }
#line 2050 "parser.cpp"
    break;

  case 47:
#line 340 "parser.y"
                    {
            (yyval.stmt)=new StmtAST();
            (yyval.stmt)=setSelect((yyvsp[0].select));
        }
#line 2059 "parser.cpp"
    break;

  case 48:
#line 343 "parser.y"
                       {
            (yyval.stmt)=new StmtAST();
            (yyval.stmt)->setIteration((yyvsp[0].iteration));
        }
#line 2068 "parser.cpp"
    break;

  case 49:
#line 349 "parser.y"
                               {
                (yyval.returnStmt)=new ReturnStmtAST();
                (yyval.returnStmt)->setExpression((yyvsp[-1].add));
            }
#line 2077 "parser.cpp"
    break;

  case 50:
#line 352 "parser.y"
                      {
                (yyval.returnStmt)=new ReturnStmtAST();
            }
#line 2085 "parser.cpp"
    break;

  case 51:
#line 357 "parser.y"
                                                    {
                (yyval.select)=new SelectStmtAST();
                (yyval.select)->setCondition((yyvsp[-2].orExp));
                (yyval.select)->setIf((yyvsp[0].stmt));
            }
#line 2095 "parser.cpp"
    break;

  case 52:
#line 361 "parser.y"
                                          {
                (yyval.select)=new SelectStmtAST();
                (yyval.select)->setCondition((yyvsp[-4].orExp));
                (yyval.select)->setIf((yyvsp[-2].stmt));
                (yyval.select)->setElse((yyvsp[0].stmt));
            }
#line 2106 "parser.cpp"
    break;

  case 53:
#line 369 "parser.y"
                                      {
					(yyval.iteration) = new IterationStmtAST();
					(yyval.iteration)->setCondition((yyvsp[-2].orExp));
					(yyval.iteration)->setStmt((yyvsp[0].stmt));
				}
#line 2116 "parser.cpp"
    break;

  case 54:
#line 381 "parser.y"
                  {
                (yyval.add)=(yyvsp[0].add);
            }
#line 2124 "parser.cpp"
    break;

  case 55:
#line 386 "parser.y"
                 {
                (yyval.orExp)=(yyvsp[0].orExp);
            }
#line 2132 "parser.cpp"
    break;

  case 56:
#line 391 "parser.y"
          {
            (yyval.lVal)=new LeftValAST();
            (yyval.lVal)->setId((yyvsp[0].id));
           }
#line 2141 "parser.cpp"
    break;

  case 57:
#line 394 "parser.y"
                          {
             (yyval.lVal)=new LeftValAST();
             (yyval.lVal)->setId((yyvsp[-1].id));
             (yyval.lVal)->setDimensions((yyvsp[0].dimensions));
           }
#line 2151 "parser.cpp"
    break;

  case 58:
#line 401 "parser.y"
                            {
                (yyval.primary) = new PrimaryExpAST();
                (yyval.primary)->setExpression((yyvsp[-1].add));
            }
#line 2160 "parser.cpp"
    break;

  case 59:
#line 404 "parser.y"
                  {
                (yyval.primary) = new PrimaryExpAST();
                (yyval.primary)->setLeftVal((yyvsp[0].lVal));
            }
#line 2169 "parser.cpp"
    break;

  case 60:
#line 407 "parser.y"
                    {
                (yyval.primary) = new PrimaryExpAST();
                (yyval.primary)->setNumber((yyvsp[0].number));
            }
#line 2178 "parser.cpp"
    break;

  case 61:
#line 413 "parser.y"
           {
            (yyval.number)=new NumberAST();
			(yyval.number)->isInt = true;
			(yyval.number)->int_t = (yyvsp[0].int_t);
        }
#line 2188 "parser.cpp"
    break;

  case 62:
#line 417 "parser.y"
              {
			(yyval.number) = new NumberAST();
			(yyval.number)->isInt = false;
			(yyval.number)->float_t = (yyvsp[0].float_t);
        }
#line 2198 "parser.cpp"
    break;

  case 63:
#line 426 "parser.y"
                      {
				(yyval.unary) = new UnaryExpAST();
				(yyval.unary)->setPrimaryExp((yyvsp[0].primary))
            }
#line 2207 "parser.cpp"
    break;

  case 64:
#line 429 "parser.y"
                  {
				(yyval.unary) = new UnaryExpAST();
				(yyval.unary)->setCall((yyvsp[0].call))
            }
#line 2216 "parser.cpp"
    break;

  case 65:
#line 432 "parser.y"
                              {
				(yyval.unary) = new UnaryExpAST();
				(yyval.unary)->setOp((yyvsp[-1].op))
				(yyval.unary)->setUnary((yyvsp[0].unary))
            }
#line 2226 "parser.cpp"
    break;

  case 66:
#line 439 "parser.y"
                {
                (yyval.call) = new CallAST();
                (yyval.call)->setId((yyvsp[-2].id));
        }
#line 2235 "parser.cpp"
    break;

  case 67:
#line 442 "parser.y"
                                 {
		        (yyval.call) = new CallAST();
        }
#line 2243 "parser.cpp"
    break;

  case 68:
#line 447 "parser.y"
               {
                (yyval.op)=U_POS;
            }
#line 2251 "parser.cpp"
    break;

  case 69:
#line 449 "parser.y"
                   {
                (yyval.op)=U_NEG;
            }
#line 2259 "parser.cpp"
    break;

  case 70:
#line 451 "parser.y"
                 {
                (yyval.op)=U_NOT;
            }
#line 2267 "parser.cpp"
    break;

  case 71:
#line 456 "parser.y"
                          {
                    (yyval.funcR)=new FuncRParamList();
                    (yyval.funcR)->add((yyvsp[0].add));
                }
#line 2276 "parser.cpp"
    break;

  case 72:
#line 459 "parser.y"
                                                 {
                    (yyval.funcR)=(yyvsp[-2].funcR);
                    (yyval.funcR)->add((yyvsp[0].add));
                }
#line 2285 "parser.cpp"
    break;

  case 73:
#line 465 "parser.y"
                {
            (yyval.mul)=new HighAST();
            (yyval.mul)->setUnaryAST((yyvsp[0].unary));
        }
#line 2294 "parser.cpp"
    break;

  case 74:
#line 468 "parser.y"
                             {
             (yyval.mul)=new HighAST();
             (yyval.mul)->setMulExp((yyvsp[-2].mul));
             (yyval.mul)->setUnaryExp((yyvsp[0].unary));
        }
#line 2304 "parser.cpp"
    break;

  case 75:
#line 472 "parser.y"
                              {
             (yyval.mul)=new HighAST();
             (yyval.mul)->setDivExp((yyvsp[-2].mul));
             (yyval.mul)->setUnaryExp((yyvsp[0].unary));
        }
#line 2314 "parser.cpp"
    break;

  case 76:
#line 476 "parser.y"
                              {
             (yyval.mul)=new HighAST();
             (yyval.mul)->setModExp((yyvsp[-2].mul));
             (yyval.mul)->setUnaryExp((yyvsp[0].unary));
        }
#line 2324 "parser.cpp"
    break;

  case 77:
#line 483 "parser.y"
              {
			(yyval.add) = new LowAST();
			(yyval.add)->setHighExp((yyvsp[0].mul));
        }
#line 2333 "parser.cpp"
    break;

  case 78:
#line 486 "parser.y"
                            {
			(yyval.add) = new LowAST();
			(yyval.add)->setExpr((yyvsp[-2].add),ADD_OP,(yyvsp[0].mul));
        }
#line 2342 "parser.cpp"
    break;

  case 79:
#line 489 "parser.y"
                              {
			(yyval.add) = new LowAST();
			(yyval.add)->setExpr((yyvsp[-2].add),MINUS_OP,(yyvsp[0].mul));
        }
#line 2351 "parser.cpp"
    break;

  case 80:
#line 494 "parser.y"
              {
            (yyval.rel) = new RelExpAST();
            (yyval.rel)->high=unique_ptr<LowAST>((yyvsp[0].add));
        }
#line 2360 "parser.cpp"
    break;

  case 81:
#line 497 "parser.y"
                          {
 			(yyval.rel) = new RelExpAST();
 			(yyval.rel)->setExp((yyvsp[-2].rel),GE_REL,(yyvsp[0].add))
        }
#line 2369 "parser.cpp"
    break;

  case 82:
#line 500 "parser.y"
                          {
			(yyval.rel) = new RelExpAST();
			(yyval.rel)->setExp((yyvsp[-2].rel),LE_REL,(yyvsp[0].add))
        }
#line 2378 "parser.cpp"
    break;

  case 83:
#line 503 "parser.y"
                           {
			(yyval.rel) = new RelExpAST();
			(yyval.rel)->setExp((yyvsp[-2].rel),GT_REL,(yyvsp[0].add))
        }
#line 2387 "parser.cpp"
    break;

  case 84:
#line 506 "parser.y"
                           {
			(yyval.rel) = new RelExpAST();
            (yyval.rel)->setExp((yyvsp[-2].rel),LT_REL,(yyvsp[0].add))
        }
#line 2396 "parser.cpp"
    break;

  case 85:
#line 511 "parser.y"
             {
            (yyval.eq)=new EqExpAST();
            (yyval.eq)_>high=unique_ptr<RelExpAST>((yyvsp[0].rel));
        }
#line 2405 "parser.cpp"
    break;

  case 86:
#line 514 "parser.y"
                         {
            (yyval.eq)=new EqExpAST();
            (yyval.eq)->setExp((yyvsp[-2].eq),EQ_REL,(yyvsp[0].rel));
        }
#line 2414 "parser.cpp"
    break;

  case 87:
#line 517 "parser.y"
                         {
            (yyval.eq)=new EqExpAST();
            (yyval.eq)->setExp((yyvsp[-2].eq),NE_REL,(yyvsp[0].rel));
        }
#line 2423 "parser.cpp"
    break;

  case 88:
#line 523 "parser.y"
              {
            (yyval.andExp) = new AndAST();
            (yyval.andExp)->high = unique_ptr<EqExpAST>((yyvsp[0].eq));
        }
#line 2432 "parser.cpp"
    break;

  case 89:
#line 526 "parser.y"
                           {
            (yyval.andExp) = new AndAST();
            (yyval.andExp)->setExp((yyvsp[-2].andExp),(yyvsp[0].eq));
        }
#line 2441 "parser.cpp"
    break;

  case 90:
#line 531 "parser.y"
               {
            (yyval.orExp) = new OrAST();
            (yyval.orExp)->high = unique_ptr<AndAST>((yyvsp[0].andExp));
        }
#line 2450 "parser.cpp"
    break;

  case 91:
#line 534 "parser.y"
                          {
            (yyval.orExp) = new OrAST();
            (yyval.orExp)->setExp((yyvsp[-2].orExp),(yyvsp[0].andExp));
        }
#line 2459 "parser.cpp"
    break;


#line 2463 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 538 "parser.y"


void yyerror(const char* fmt) {
    printf("line:%d\tcolumn:%d\n", yylloc.first_line,yylloc.first_column);
    printf("ERROR: %s\n", fmt);
}




