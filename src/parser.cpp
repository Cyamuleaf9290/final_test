/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 1 "src/parser.y"

    #include <iostream>
    #include <assert.h>
    #include "parser.h"
    #include <cstring>
    #include <stack>
    extern Ast ast;

    // for typecheck
    Type *latestType;   //由于vardef看不到type，我们保留一下当前最后一次词法分析出的type
    std::string lastid;
    // for array
    ArrayType* arrayType;
    int arrayIdx;
    int* arrayValue;
    int leftbrace = 0;
    std::stack<InitValExpr*> stk;
    InitValExpr* top;
    // for while stmt
    int is_in_while = 0;
    std::stack<StmtNode*> whileStack;
    // for function
    int glob_param_cnt = 0;

    int yylex();
    int yyerror( char const * );
    Type* getMaxType(ExprNode *expr1, ExprNode *expr2)
    {
        Type* t1;
        Type* t2;
        t1 = expr1->getSymPtr()->getType(true);
        t2 = expr2->getSymPtr()->getType(true);
        if(t1->isVoid() || t2->isVoid()) {
            fprintf(stderr,"operator can not be void type.\n");
            exit(EXIT_FAILURE);
        }
        if(t1->isFloatLike()|| t2->isFloatLike()) return TypeSystem::floatType;
        return TypeSystem::intType;
    }

#line 109 "src/parser.cpp"




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

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_INTEGER = 4,                    /* INTEGER  */
  YYSYMBOL_FLOAT = 5,                      /* FLOAT  */
  YYSYMBOL_FLOATS = 6,                     /* FLOATS  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_INT = 10,                       /* INT  */
  YYSYMBOL_VOID = 11,                      /* VOID  */
  YYSYMBOL_LPAREN = 12,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 13,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 14,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 15,                    /* RBRACE  */
  YYSYMBOL_SEMICOLON = 16,                 /* SEMICOLON  */
  YYSYMBOL_LBRACKET = 17,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 18,                  /* RBRACKET  */
  YYSYMBOL_COMMA = 19,                     /* COMMA  */
  YYSYMBOL_ADD = 20,                       /* ADD  */
  YYSYMBOL_SUB = 21,                       /* SUB  */
  YYSYMBOL_MUL = 22,                       /* MUL  */
  YYSYMBOL_DIV = 23,                       /* DIV  */
  YYSYMBOL_MOD = 24,                       /* MOD  */
  YYSYMBOL_OR = 25,                        /* OR  */
  YYSYMBOL_AND = 26,                       /* AND  */
  YYSYMBOL_LT = 27,                        /* LT  */
  YYSYMBOL_LTE = 28,                       /* LTE  */
  YYSYMBOL_GT = 29,                        /* GT  */
  YYSYMBOL_GTE = 30,                       /* GTE  */
  YYSYMBOL_ASSIGN = 31,                    /* ASSIGN  */
  YYSYMBOL_EQUAL = 32,                     /* EQUAL  */
  YYSYMBOL_NEQUAL = 33,                    /* NEQUAL  */
  YYSYMBOL_NOT = 34,                       /* NOT  */
  YYSYMBOL_UNKNOWN = 35,                   /* UNKNOWN  */
  YYSYMBOL_CONST = 36,                     /* CONST  */
  YYSYMBOL_RETURN = 37,                    /* RETURN  */
  YYSYMBOL_BREAK = 38,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 39,                  /* CONTINUE  */
  YYSYMBOL_THEN = 40,                      /* THEN  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_Program = 42,                   /* Program  */
  YYSYMBOL_Stmts = 43,                     /* Stmts  */
  YYSYMBOL_Stmt = 44,                      /* Stmt  */
  YYSYMBOL_LVal = 45,                      /* LVal  */
  YYSYMBOL_AssignStmt = 46,                /* AssignStmt  */
  YYSYMBOL_BlockStmt = 47,                 /* BlockStmt  */
  YYSYMBOL_48_1 = 48,                      /* $@1  */
  YYSYMBOL_IfStmt = 49,                    /* IfStmt  */
  YYSYMBOL_WhileStmt = 50,                 /* WhileStmt  */
  YYSYMBOL_51_2 = 51,                      /* @2  */
  YYSYMBOL_BreakStmt = 52,                 /* BreakStmt  */
  YYSYMBOL_ContinueStmt = 53,              /* ContinueStmt  */
  YYSYMBOL_ReturnStmt = 54,                /* ReturnStmt  */
  YYSYMBOL_ExprStmt = 55,                  /* ExprStmt  */
  YYSYMBOL_BlankStmt = 56,                 /* BlankStmt  */
  YYSYMBOL_Exp = 57,                       /* Exp  */
  YYSYMBOL_Cond = 58,                      /* Cond  */
  YYSYMBOL_PrimaryExp = 59,                /* PrimaryExp  */
  YYSYMBOL_AddExp = 60,                    /* AddExp  */
  YYSYMBOL_MulExp = 61,                    /* MulExp  */
  YYSYMBOL_UnaryExp = 62,                  /* UnaryExp  */
  YYSYMBOL_FuncRParams = 63,               /* FuncRParams  */
  YYSYMBOL_RelExp = 64,                    /* RelExp  */
  YYSYMBOL_EqExp = 65,                     /* EqExp  */
  YYSYMBOL_LAndExp = 66,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 67,                    /* LOrExp  */
  YYSYMBOL_Type = 68,                      /* Type  */
  YYSYMBOL_DeclStmt = 69,                  /* DeclStmt  */
  YYSYMBOL_VarDefList = 70,                /* VarDefList  */
  YYSYMBOL_VarDef = 71,                    /* VarDef  */
  YYSYMBOL_72_3 = 72,                      /* @3  */
  YYSYMBOL_ArrayIndices = 73,              /* ArrayIndices  */
  YYSYMBOL_InitVal = 74,                   /* InitVal  */
  YYSYMBOL_75_4 = 75,                      /* @4  */
  YYSYMBOL_InitValList = 76,               /* InitValList  */
  YYSYMBOL_FuncDef = 77,                   /* FuncDef  */
  YYSYMBOL_78_5 = 78,                      /* $@5  */
  YYSYMBOL_79_6 = 79,                      /* $@6  */
  YYSYMBOL_FuncFParams = 80,               /* FuncFParams  */
  YYSYMBOL_FuncFParam = 81,                /* FuncFParam  */
  YYSYMBOL_FuncArrayIndices = 82           /* FuncArrayIndices  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  58
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   215

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  95
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  155

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    77,    77,    82,    83,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,   101,   117,   133,   170,
     169,   178,   183,   186,   191,   191,   205,   215,   226,   230,
     236,   240,   246,   253,   261,   265,   273,   280,   286,   291,
     302,   314,   318,   328,   336,   347,   357,   394,   417,   442,
     460,   461,   465,   468,   471,   475,   479,   483,   489,   490,
     494,   501,   505,   513,   517,   524,   528,   532,   536,   540,
     546,   552,   557,   564,   575,   616,   655,   655,   704,   717,
     730,   764,   789,   789,   831,   834,   843,   857,   843,   915,
     919,   924,   928,   939,   966,   970
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "INTEGER",
  "FLOAT", "FLOATS", "IF", "ELSE", "WHILE", "INT", "VOID", "LPAREN",
  "RPAREN", "LBRACE", "RBRACE", "SEMICOLON", "LBRACKET", "RBRACKET",
  "COMMA", "ADD", "SUB", "MUL", "DIV", "MOD", "OR", "AND", "LT", "LTE",
  "GT", "GTE", "ASSIGN", "EQUAL", "NEQUAL", "NOT", "UNKNOWN", "CONST",
  "RETURN", "BREAK", "CONTINUE", "THEN", "$accept", "Program", "Stmts",
  "Stmt", "LVal", "AssignStmt", "BlockStmt", "$@1", "IfStmt", "WhileStmt",
  "@2", "BreakStmt", "ContinueStmt", "ReturnStmt", "ExprStmt", "BlankStmt",
  "Exp", "Cond", "PrimaryExp", "AddExp", "MulExp", "UnaryExp",
  "FuncRParams", "RelExp", "EqExp", "LAndExp", "LOrExp", "Type",
  "DeclStmt", "VarDefList", "VarDef", "@3", "ArrayIndices", "InitVal",
  "@4", "InitValList", "FuncDef", "$@5", "$@6", "FuncFParams",
  "FuncFParam", "FuncArrayIndices", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-104)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     176,    -4,  -104,  -104,  -104,    -7,     8,  -104,  -104,    74,
     -12,  -104,    74,    74,    74,    32,    13,    27,    34,    11,
     176,  -104,   -16,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,    41,  -104,    52,    30,  -104,    45,  -104,  -104,
      74,    74,  -104,    74,    74,  -104,    46,  -104,   176,  -104,
    -104,  -104,  -104,  -104,  -104,    84,  -104,  -104,  -104,  -104,
      74,  -104,    74,    74,    74,    74,    74,    29,    85,    83,
      86,    90,    88,    91,    52,    61,    60,    81,    87,    97,
    -104,   139,  -104,    95,    30,    30,  -104,  -104,  -104,  -104,
      24,    82,  -104,   111,    74,  -104,    98,   176,    74,    74,
      74,    74,    74,    74,    74,    74,  -104,  -104,  -104,    51,
     102,  -104,  -104,  -104,    -5,  -104,  -104,  -104,   110,    52,
      52,    52,    52,    61,    61,    60,    81,   176,   116,  -104,
     101,  -104,    24,    24,   176,  -104,   104,   109,    51,  -104,
      -1,  -104,  -104,   107,   112,   113,  -104,  -104,    24,  -104,
      74,  -104,  -104,   108,  -104
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    16,    35,    67,    36,     0,     0,    65,    66,     0,
      19,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     3,    34,     5,     6,     7,     8,     9,    10,    11,
      13,    14,     0,    45,    32,    38,    41,     0,    12,    15,
      52,     0,    17,     0,     0,    34,     0,    21,     0,    47,
      48,    49,    69,    68,    29,     0,    26,    27,     1,     4,
       0,    30,     0,     0,     0,     0,     0,    73,     0,    72,
      50,     0,     0,     0,    53,    58,    61,    63,    33,     0,
      37,     0,    28,     0,    39,    40,    42,    43,    44,    86,
       0,    75,    70,     0,    52,    46,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,    20,    18,    91,
      82,    80,    74,    76,    73,    71,    51,    79,    22,    54,
      55,    56,    57,    59,    60,    62,    64,     0,     0,    87,
      89,    81,     0,     0,     0,    25,    92,     0,    91,    84,
       0,    77,    23,     0,    93,     0,    90,    83,     0,    94,
       0,    88,    85,     0,    95
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -104,  -104,    89,   -18,     1,  -104,   -15,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,    -9,    92,  -104,   -34,
      21,    10,    37,    -6,    28,    33,  -104,  -103,  -104,    40,
    -104,  -104,     3,   -93,  -104,  -104,  -104,  -104,  -104,     2,
    -104,  -104
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    19,    20,    21,    45,    23,    24,    48,    25,    26,
     127,    27,    28,    29,    30,    31,    32,    73,    33,    34,
      35,    36,    71,    75,    76,    77,    78,    37,    38,    68,
      69,   133,    91,   112,   132,   140,    39,   109,   137,   129,
     130,   144
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      46,    22,    59,    47,    42,    43,   128,    55,    40,    74,
      74,    58,    41,    41,   147,    60,     1,     2,   148,     4,
      44,    22,    49,    50,    51,     9,    90,     1,     2,    54,
       4,    70,    72,    12,    13,   128,     9,    52,   110,   139,
     141,    89,    53,    56,    12,    13,    41,    14,    67,    22,
      57,    83,    64,    65,    66,   152,     3,    61,    14,    80,
      90,     7,     8,    59,   119,   120,   121,   122,    74,    74,
      74,    74,    62,    63,    86,    87,    88,     1,     2,   118,
       4,   111,    22,    84,    85,    70,     9,    15,    98,    99,
     100,   101,   102,   103,    12,    13,   123,   124,    22,   117,
      82,    92,    93,    95,    97,    94,    96,   104,    14,   135,
     106,   108,   105,   113,   114,    41,   142,   131,   134,   136,
     138,   143,   145,   111,   111,   149,   154,    10,    22,   150,
     151,   116,   125,   115,     0,    22,    79,    81,   126,   111,
     146,   153,     1,     2,     3,     4,     5,     0,     6,     7,
       8,     9,     0,    10,   107,    11,     0,     0,     0,    12,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,     0,    15,    16,    17,    18,     1,
       2,     3,     4,     5,     0,     6,     7,     8,     9,     0,
      10,     0,    11,     0,     0,     0,    12,    13,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,     0,    15,    16,    17,    18
};

static const yytype_int16 yycheck[] =
{
       9,     0,    20,    15,     1,    12,   109,    16,    12,    43,
      44,     0,    17,    17,    15,    31,     3,     4,    19,     6,
      12,    20,    12,    13,    14,    12,    31,     3,     4,    16,
       6,    40,    41,    20,    21,   138,    12,     5,    14,   132,
     133,    12,    10,    16,    20,    21,    17,    34,     3,    48,
      16,    60,    22,    23,    24,   148,     5,    16,    34,    13,
      31,    10,    11,    81,    98,    99,   100,   101,   102,   103,
     104,   105,    20,    21,    64,    65,    66,     3,     4,    97,
       6,    90,    81,    62,    63,    94,    12,    36,    27,    28,
      29,    30,    32,    33,    20,    21,   102,   103,    97,    96,
      16,    16,    19,    13,    13,    19,    18,    26,    34,   127,
      13,    16,    25,    31,     3,    17,   134,    15,     8,     3,
      19,    17,    13,   132,   133,    18,    18,    14,   127,    17,
     145,    94,   104,    93,    -1,   134,    44,    48,   105,   148,
     138,   150,     3,     4,     5,     6,     7,    -1,     9,    10,
      11,    12,    -1,    14,    15,    16,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    37,    38,    39,     3,
       4,     5,     6,     7,    -1,     9,    10,    11,    12,    -1,
      14,    -1,    16,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    37,    38,    39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     9,    10,    11,    12,
      14,    16,    20,    21,    34,    36,    37,    38,    39,    42,
      43,    44,    45,    46,    47,    49,    50,    52,    53,    54,
      55,    56,    57,    59,    60,    61,    62,    68,    69,    77,
      12,    17,    73,    12,    12,    45,    57,    15,    48,    62,
      62,    62,     5,    10,    16,    57,    16,    16,     0,    44,
      31,    16,    20,    21,    22,    23,    24,     3,    70,    71,
      57,    63,    57,    58,    60,    64,    65,    66,    67,    58,
      13,    43,    16,    57,    61,    61,    62,    62,    62,    12,
      31,    73,    16,    19,    19,    13,    18,    13,    27,    28,
      29,    30,    32,    33,    26,    25,    13,    15,    16,    78,
      14,    57,    74,    31,     3,    70,    63,    73,    44,    60,
      60,    60,    60,    64,    64,    65,    66,    51,    68,    80,
      81,    15,    75,    72,     8,    44,     3,    79,    19,    74,
      76,    74,    44,    17,    82,    13,    80,    15,    19,    18,
      17,    47,    74,    57,    18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    45,    45,    46,    48,
      47,    47,    49,    49,    51,    50,    52,    53,    54,    54,
      55,    56,    57,    58,    59,    59,    59,    59,    60,    60,
      60,    61,    61,    61,    61,    62,    62,    62,    62,    62,
      63,    63,    63,    64,    64,    64,    64,    64,    65,    65,
      65,    66,    66,    67,    67,    68,    68,    68,    68,    68,
      69,    70,    70,    71,    71,    71,    72,    71,    73,    73,
      74,    74,    75,    74,    76,    76,    78,    79,    77,    80,
      80,    80,    81,    81,    82,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     0,
       4,     2,     5,     7,     0,     6,     2,     2,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     4,     2,     2,     2,
       1,     3,     0,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     1,     1,     2,     2,
       3,     3,     1,     1,     3,     2,     0,     5,     3,     4,
       1,     2,     0,     4,     1,     3,     0,     0,     8,     1,
       3,     0,     2,     3,     2,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: Stmts  */
#line 77 "src/parser.y"
            {
        ast.setRoot((yyvsp[0].stmttype));
    }
#line 1306 "src/parser.cpp"
    break;

  case 3: /* Stmts: Stmt  */
#line 82 "src/parser.y"
           {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1312 "src/parser.cpp"
    break;

  case 4: /* Stmts: Stmts Stmt  */
#line 83 "src/parser.y"
                {
        (yyval.stmttype) = new SeqNode((yyvsp[-1].stmttype), (yyvsp[0].stmttype));
    }
#line 1320 "src/parser.cpp"
    break;

  case 5: /* Stmt: AssignStmt  */
#line 88 "src/parser.y"
                 {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1326 "src/parser.cpp"
    break;

  case 6: /* Stmt: BlockStmt  */
#line 89 "src/parser.y"
                {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1332 "src/parser.cpp"
    break;

  case 7: /* Stmt: IfStmt  */
#line 90 "src/parser.y"
             {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1338 "src/parser.cpp"
    break;

  case 8: /* Stmt: WhileStmt  */
#line 91 "src/parser.y"
                {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1344 "src/parser.cpp"
    break;

  case 9: /* Stmt: BreakStmt  */
#line 92 "src/parser.y"
                {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1350 "src/parser.cpp"
    break;

  case 10: /* Stmt: ContinueStmt  */
#line 93 "src/parser.y"
                   {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1356 "src/parser.cpp"
    break;

  case 11: /* Stmt: ReturnStmt  */
#line 94 "src/parser.y"
                 {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1362 "src/parser.cpp"
    break;

  case 12: /* Stmt: DeclStmt  */
#line 95 "src/parser.y"
               {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1368 "src/parser.cpp"
    break;

  case 13: /* Stmt: ExprStmt  */
#line 96 "src/parser.y"
               {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1374 "src/parser.cpp"
    break;

  case 14: /* Stmt: BlankStmt  */
#line 97 "src/parser.y"
                {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1380 "src/parser.cpp"
    break;

  case 15: /* Stmt: FuncDef  */
#line 98 "src/parser.y"
              {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1386 "src/parser.cpp"
    break;

  case 16: /* LVal: ID  */
#line 101 "src/parser.y"
         {
        // fprintf(stderr,"LVal => ID\n");
        SymbolEntry *se;
        se = identifiers->lookup((yyvsp[0].strtype));
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)(yyvsp[0].strtype));
            delete [](char*)(yyvsp[0].strtype);
            exit(EXIT_FAILURE);
        }
        (yyval.exprtype) = new Id(se);
        // const int N = 3, a[N+1][2]={}，需要修改lval表达式的值
        if(se->getType()->isConstInt())
            (yyval.exprtype)->setIntValue(se->getIntValue());
        delete [](yyvsp[0].strtype);
    }
#line 1407 "src/parser.cpp"
    break;

  case 17: /* LVal: ID ArrayIndices  */
#line 117 "src/parser.y"
                      { //数组左值
        SymbolEntry *se;
        se = identifiers->lookup((yyvsp[-1].strtype));
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)(yyvsp[-1].strtype));
            delete [](char*)(yyvsp[-1].strtype);
            exit(EXIT_FAILURE);
        }
        (yyval.exprtype) = new Id(se, (yyvsp[0].exprtype));
        (yyval.exprtype)->setIntValue(se->getIntValue());
        delete [](yyvsp[-1].strtype);
    }
#line 1425 "src/parser.cpp"
    break;

  case 18: /* AssignStmt: LVal ASSIGN Exp SEMICOLON  */
#line 133 "src/parser.y"
                              {
        // fprintf(stderr,"AssignStmt : LVal ASSIGN Exp SEMICOLON\n");
        if((yyvsp[-3].exprtype)->getSymPtr()->getType()->isConstInt() || (yyvsp[-3].exprtype)->getSymPtr()->getType()->isConstFloat()) {
            fprintf(stderr,"You can't assign a constant.\n");
            exit(EXIT_FAILURE);
        }
        // 判断exp是否为void函数
        if((yyvsp[-1].exprtype)->getSymPtr()->isVariable() && (dynamic_cast<IdentifierSymbolEntry*>((yyvsp[-1].exprtype)->getSymPtr()))->isFunc()){
            Type *retType = dynamic_cast<FunctionType*>(dynamic_cast<IdentifierSymbolEntry *>((yyvsp[-1].exprtype)->getSymPtr())->getType(true));
            if(retType->isVoid()) {
                fprintf(stderr,"right val can not be a void func call\n");
                exit(EXIT_FAILURE);
            } else  {
                // 给左值赋予函数调用的返回值，这是在运行时才能决定的。TODO
            }
        } /*else {    
            // exp为标识符或数值或数组，数组暂不考虑
            if ($1->getSymPtr()->getType()->isInt()) {
                if($3->getSymPtr()->getType()->isConstFloat() || $3->getSymPtr()->getType()->isFloat()) {
                    int val = $3->getSymPtr()->getFloatValue();
                    (dynamic_cast<IdentifierSymbolEntry*>($1->getSymPtr()))->setIntValue(val);
                } else (dynamic_cast<IdentifierSymbolEntry*>($1->getSymPtr()))->setIntValue($3->getSymPtr()->getIntValue());
            } else {
                if($3->getSymPtr()->getType()->isConstFloat() || $3->getSymPtr()->getType()->isFloat()) {
                    //(dynamic_cast<IdentifierSymbolEntry*>($1->getSymPtr()))->setFloatValue($3->getSymPtr()->getFloatValue());
                } else {
                    //float val = (float)($3->getSymPtr()->getIntValue());
                    //(dynamic_cast<IdentifierSymbolEntry*>($1->getSymPtr()))->setFloatValue(val);
                }
            }  
        }*/
        (yyvsp[-3].exprtype)->setIntValue((yyvsp[-1].exprtype)->getIntValue());
        (yyval.stmttype) = new AssignStmt((yyvsp[-3].exprtype), (yyvsp[-1].exprtype));
    }
#line 1464 "src/parser.cpp"
    break;

  case 19: /* $@1: %empty  */
#line 170 "src/parser.y"
        {identifiers = new SymbolTable(identifiers);}
#line 1470 "src/parser.cpp"
    break;

  case 20: /* BlockStmt: LBRACE $@1 Stmts RBRACE  */
#line 172 "src/parser.y"
        {
            (yyval.stmttype) = new CompoundStmt((yyvsp[-1].stmttype));
            SymbolTable *top = identifiers;
            identifiers = identifiers->getPrev();
            delete top;
        }
#line 1481 "src/parser.cpp"
    break;

  case 21: /* BlockStmt: LBRACE RBRACE  */
#line 178 "src/parser.y"
                    {
        (yyval.stmttype) = new EmptyStmt();
    }
#line 1489 "src/parser.cpp"
    break;

  case 22: /* IfStmt: IF LPAREN Cond RPAREN Stmt  */
#line 183 "src/parser.y"
                                            {
        (yyval.stmttype) = new IfStmt((yyvsp[-2].exprtype), (yyvsp[0].stmttype));
    }
#line 1497 "src/parser.cpp"
    break;

  case 23: /* IfStmt: IF LPAREN Cond RPAREN Stmt ELSE Stmt  */
#line 186 "src/parser.y"
                                           {
        (yyval.stmttype) = new IfElseStmt((yyvsp[-4].exprtype), (yyvsp[-2].stmttype), (yyvsp[0].stmttype));
    }
#line 1505 "src/parser.cpp"
    break;

  case 24: /* @2: %empty  */
#line 191 "src/parser.y"
                               {
        // fprintf(stderr,"WhileStmt : WHILE LPAREN Cond RPAREN\n");
        is_in_while ++;
        // pass through
        (yyval.stmttype) = new WhileStmt((yyvsp[-1].exprtype));
        whileStack.push((yyval.stmttype));
    }
#line 1517 "src/parser.cpp"
    break;

  case 25: /* WhileStmt: WHILE LPAREN Cond RPAREN @2 Stmt  */
#line 197 "src/parser.y"
          {
        (yyval.stmttype) = (yyvsp[-1].stmttype);
        dynamic_cast<WhileStmt*>((yyval.stmttype))->setStmt((yyvsp[0].stmttype));
        whileStack.pop();
        is_in_while --;
    }
#line 1528 "src/parser.cpp"
    break;

  case 26: /* BreakStmt: BREAK SEMICOLON  */
#line 205 "src/parser.y"
                      {
        if(is_in_while)
            (yyval.stmttype) = new BreakStmt(whileStack.top());
        else {
            fprintf(stderr,"break statement not in while statemrnt.\n");
            exit(EXIT_FAILURE);
        }
    }
#line 1541 "src/parser.cpp"
    break;

  case 27: /* ContinueStmt: CONTINUE SEMICOLON  */
#line 215 "src/parser.y"
                         {
        if(is_in_while)
            (yyval.stmttype) = new ContinueStmt(whileStack.top());
        else {
            fprintf(stderr,"continue statement not in while statemrnt.\n");
            exit(EXIT_FAILURE);
        }
    }
#line 1554 "src/parser.cpp"
    break;

  case 28: /* ReturnStmt: RETURN Exp SEMICOLON  */
#line 226 "src/parser.y"
                        {
        // fprintf(stderr,"ReturnStmt : RETURN Exp SEMICOLON.\n");
        (yyval.stmttype) = new ReturnStmt((yyvsp[-1].exprtype));
    }
#line 1563 "src/parser.cpp"
    break;

  case 29: /* ReturnStmt: RETURN SEMICOLON  */
#line 230 "src/parser.y"
                       {
        (yyval.stmttype) = new ReturnStmt(); 
    }
#line 1571 "src/parser.cpp"
    break;

  case 30: /* ExprStmt: Exp SEMICOLON  */
#line 236 "src/parser.y"
                    {
        (yyval.stmttype) = new ExprStmt((yyvsp[-1].exprtype));
    }
#line 1579 "src/parser.cpp"
    break;

  case 31: /* BlankStmt: SEMICOLON  */
#line 240 "src/parser.y"
                {
        (yyval.stmttype) = new BlankStmt();
    }
#line 1587 "src/parser.cpp"
    break;

  case 32: /* Exp: AddExp  */
#line 246 "src/parser.y"
           {
        // fprintf(stderr,"Exp : AddExp\n");
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 1596 "src/parser.cpp"
    break;

  case 33: /* Cond: LOrExp  */
#line 253 "src/parser.y"
           {
        // fprintf(stderr,"Cond : LOrExp\n");
        (yyval.exprtype) = (yyvsp[0].exprtype);
        (yyval.exprtype)->setCond();
    }
#line 1606 "src/parser.cpp"
    break;

  case 34: /* PrimaryExp: LVal  */
#line 261 "src/parser.y"
         {
        // fprintf(stderr,"PrimaryExp => LVal!\n");
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 1615 "src/parser.cpp"
    break;

  case 35: /* PrimaryExp: INTEGER  */
#line 265 "src/parser.y"
              {
        // fprintf(stderr,"PrimaryExp => INTEGER!:%d\n",$1);
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::constIntType, (yyvsp[0].itype));
        se->setIntValue((yyvsp[0].itype));
        (yyval.exprtype) = new Constant(se);
        (yyval.exprtype)->setConst();
        (yyval.exprtype)->setIntValue((yyvsp[0].itype));
    }
#line 1628 "src/parser.cpp"
    break;

  case 36: /* PrimaryExp: FLOATS  */
#line 273 "src/parser.y"
             {
        // fprintf(stderr,"PrimaryExp => FLOATS!:%f\n",$1);
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::constFloatType, (yyvsp[0].ftype));
        (yyval.exprtype) = new Constant(se);
        (yyval.exprtype)->setConst();
        (yyval.exprtype)->setFloatValue((yyvsp[0].ftype));
    }
#line 1640 "src/parser.cpp"
    break;

  case 37: /* PrimaryExp: LPAREN Exp RPAREN  */
#line 280 "src/parser.y"
                        {
        (yyval.exprtype) = (yyvsp[-1].exprtype);
    }
#line 1648 "src/parser.cpp"
    break;

  case 38: /* AddExp: MulExp  */
#line 286 "src/parser.y"
           {
        // fprintf(stderr,"AddExp : MulExp \n");
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 1657 "src/parser.cpp"
    break;

  case 39: /* AddExp: AddExp ADD MulExp  */
#line 292 "src/parser.y"
    {
        // fprintf(stderr,"AddExp : AddExp ADD MulExp \n");
        SymbolEntry *se;
        Type* resType = getMaxType((yyvsp[-2].exprtype),(yyvsp[0].exprtype));
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if((yyvsp[-2].exprtype)->isConst() && (yyvsp[0].exprtype)->isConst())
            (yyval.exprtype)->setConst();
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::ADD, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1671 "src/parser.cpp"
    break;

  case 40: /* AddExp: AddExp SUB MulExp  */
#line 303 "src/parser.y"
    {
        SymbolEntry *se;
        Type* resType = getMaxType((yyvsp[-2].exprtype),(yyvsp[0].exprtype));
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if((yyvsp[-2].exprtype)->isConst() && (yyvsp[0].exprtype)->isConst())
            (yyval.exprtype)->setConst();
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::SUB, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1684 "src/parser.cpp"
    break;

  case 41: /* MulExp: UnaryExp  */
#line 314 "src/parser.y"
               {
        // fprintf(stderr,"MulExp : UnaryExp.\n");
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 1693 "src/parser.cpp"
    break;

  case 42: /* MulExp: MulExp MUL UnaryExp  */
#line 318 "src/parser.y"
                          {
        SymbolEntry *se;
        //判断UnaryExp：标识符，数值，函数调用
        Type* resType = getMaxType((yyvsp[-2].exprtype),(yyvsp[0].exprtype));
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if((yyvsp[-2].exprtype)->isConst() && (yyvsp[0].exprtype)->isConst())
            (yyval.exprtype)->setConst();
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::MUL, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
        
    }
#line 1708 "src/parser.cpp"
    break;

  case 43: /* MulExp: MulExp DIV UnaryExp  */
#line 328 "src/parser.y"
                          {
        SymbolEntry *se;
        Type* resType = getMaxType((yyvsp[-2].exprtype),(yyvsp[0].exprtype));
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if((yyvsp[-2].exprtype)->isConst() && (yyvsp[0].exprtype)->isConst())
            (yyval.exprtype)->setConst();
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::DIV, (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); 
    }
#line 1721 "src/parser.cpp"
    break;

  case 44: /* MulExp: MulExp MOD UnaryExp  */
#line 336 "src/parser.y"
                          {
        SymbolEntry *se;
        Type* resType = getMaxType((yyvsp[-2].exprtype),(yyvsp[0].exprtype));
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if((yyvsp[-2].exprtype)->isConst() && (yyvsp[0].exprtype)->isConst())
            (yyval.exprtype)->setConst();
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::MOD, (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); 
    }
#line 1734 "src/parser.cpp"
    break;

  case 45: /* UnaryExp: PrimaryExp  */
#line 347 "src/parser.y"
                 {  //id   , 1,  1.23 constant
        // fprintf(stderr,"UnaryExp => PrimaryExp.\n");
        /*SymbolEntry *se;
        if($1->getSymPtr()->getType()->isInt() || $1->getSymPtr()->getType()->isConstInt())
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        else if($1->getSymPtr()->getType()->isFloat() || $1->getSymPtr()->getType()->isConstFloat())
            se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());
        $$ = new UnaryExpr(se, UnaryExpr::POS, $1);*/
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 1749 "src/parser.cpp"
    break;

  case 46: /* UnaryExp: ID LPAREN FuncRParams RPAREN  */
#line 357 "src/parser.y"
                                   {
        // fprintf(stderr,"UnaryExp => ID LPAREN FuncRPamras RPAREN.\n");
        // 检查函数id是否被注册过
        SymbolEntry* se;
        se = identifiers->lookup((yyvsp[-3].strtype));
        if(se == nullptr) {
            fprintf(stderr,"function did not defined.\n");
            exit(EXIT_FAILURE);
        }
        //检查函数的参数类型是否正确     func(32, max(a,b));
        ExprNode *node = (yyvsp[-1].exprtype);
        if(se->getType()->isFunc()) {
            std::vector<Type*> params = dynamic_cast<FunctionType*>(se->getType())->getParams();
            for(size_t i = 0; i < params.size(); i ++) {
                if(node->getSymPtr()->getType()->isFunc()){
                    if(!dynamic_cast<FunctionType* >(node->getSymPtr()->getType())->getRetType()->isIntLike()){
                        fprintf(stderr,"function params type not match.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else if(node == nullptr || !node->getSymPtr()->getType()->isIntLike()) {
                    fprintf(stderr,"function params type not match.\n");
                    exit(EXIT_FAILURE);
                }
                node = dynamic_cast<ExprNode*>(node->getNext());
            }
        }
        if(dynamic_cast<FunctionType* >(se->getType())->getRetType() == TypeSystem::voidType){
            fprintf(stderr,"parser: void ret type \n");
        }
        else{
            fprintf(stderr,"parser: NOT void ret type %s\n", se->getType(true)->toStr().c_str());
        }
        if((yyvsp[-1].exprtype)) (yyval.exprtype) = new FuncCallExpr(se, (yyvsp[-1].exprtype));
        else (yyval.exprtype) = new FuncCallExpr(se);
        fprintf(stderr,"func call\n");
    }
#line 1791 "src/parser.cpp"
    break;

  case 47: /* UnaryExp: ADD UnaryExp  */
#line 394 "src/parser.y"
                   {
        // fprintf(stderr,"UnaryExp : ADD UnaryExp.\n");
        SymbolEntry *se;
        if(!(yyvsp[0].exprtype)->isConst()){
        /*if((dynamic_cast<UnaryExpr*>($2))->getOP() == UnaryExpr::NOT) {
            $2->setType(TypeSystem::boolType);
            se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        } else {
            if($2->getSymPtr()->getType()->isInt() || $2->getSymPtr()->getType()->isConstInt())
                se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
            else if($2->getSymPtr()->getType()->isFloat() || $2->getSymPtr()->getType()->isConstFloat())
                se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());
        }*/
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
            (yyval.exprtype) = new UnaryExpr(se, UnaryExpr::POS, (yyvsp[0].exprtype));
        }
        else{
            se = (yyvsp[0].exprtype)->getSymPtr();
            (yyval.exprtype) = new UnaryExpr(se, UnaryExpr::POS, (yyvsp[0].exprtype));
            (yyval.exprtype)->setConst();
        }
        (yyval.exprtype)->setUnary();
    }
#line 1819 "src/parser.cpp"
    break;

  case 48: /* UnaryExp: SUB UnaryExp  */
#line 417 "src/parser.y"
                   { // -2 -a -(a+b-c)
        // fprintf(stderr,"UnaryExp : SUB UnaryExp.\n");
        SymbolEntry *se;
        if(!(yyvsp[0].exprtype)->isConst()){
        /*if(((dynamic_cast<UnaryExpr*>($2)))->getOP() == UnaryExpr::NOT) {
            $2->setType(TypeSystem::boolType);
            se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        } else {
            if($2->getSymPtr()->getType()->isInt() || $2->getSymPtr()->getType()->isConstInt())
                se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
            else if($2->getSymPtr()->getType()->isFloat() || $2->getSymPtr()->getType()->isConstFloat())
                se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());
        }*/
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
            (yyval.exprtype) = new UnaryExpr(se, UnaryExpr::NEG, (yyvsp[0].exprtype));
        }
        else{
            se = (yyvsp[0].exprtype)->getSymPtr();
            se->setIntValue(-se->getIntValue());
            (yyval.exprtype) = new UnaryExpr(se, UnaryExpr::NEG, (yyvsp[0].exprtype));
            (yyval.exprtype)->setIntValue(-se->getIntValue());
            (yyval.exprtype)->setConst();
        }
        (yyval.exprtype)->setUnary();
    }
#line 1849 "src/parser.cpp"
    break;

  case 49: /* UnaryExp: NOT UnaryExp  */
#line 442 "src/parser.y"
                   {
        // fprintf(stderr,"UnaryExp : NOT UnaryExp.\n");
        /*
        if((dynamic_cast<UnaryExpr*>($2))->getOP() == UnaryExpr::NOT) {
            $2->setType(TypeSystem::boolType);
        }
        SymbolEntry *se;
        if($2->getSymPtr()->getType()->isInt())
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        else if($2->getSymPtr()->getType()->isFloat())
            se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());*/
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new UnaryExpr(se, UnaryExpr::NOT, (yyvsp[0].exprtype));
        (yyval.exprtype)->setBool();
        (yyval.exprtype)->setUnary();
    }
#line 1870 "src/parser.cpp"
    break;

  case 50: /* FuncRParams: Exp  */
#line 460 "src/parser.y"
          {(yyval.exprtype) =(yyvsp[0].exprtype);/*函数实参TODO*/}
#line 1876 "src/parser.cpp"
    break;

  case 51: /* FuncRParams: Exp COMMA FuncRParams  */
#line 461 "src/parser.y"
                            {
        (yyval.exprtype) = (yyvsp[-2].exprtype);
        (yyvsp[-2].exprtype)->setNext((yyvsp[0].exprtype));
    }
#line 1885 "src/parser.cpp"
    break;

  case 52: /* FuncRParams: %empty  */
#line 465 "src/parser.y"
             {(yyval.exprtype) = nullptr;}
#line 1891 "src/parser.cpp"
    break;

  case 53: /* RelExp: AddExp  */
#line 468 "src/parser.y"
             {
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 1899 "src/parser.cpp"
    break;

  case 54: /* RelExp: RelExp LT AddExp  */
#line 471 "src/parser.y"
                       {    //<
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::LT, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1908 "src/parser.cpp"
    break;

  case 55: /* RelExp: RelExp LTE AddExp  */
#line 475 "src/parser.y"
                        {   //<=
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::LTE, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1917 "src/parser.cpp"
    break;

  case 56: /* RelExp: RelExp GT AddExp  */
#line 479 "src/parser.y"
                       {    //>
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::GT, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1926 "src/parser.cpp"
    break;

  case 57: /* RelExp: RelExp GTE AddExp  */
#line 483 "src/parser.y"
                        {   //>=
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::GTE, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1935 "src/parser.cpp"
    break;

  case 58: /* EqExp: RelExp  */
#line 489 "src/parser.y"
             {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1941 "src/parser.cpp"
    break;

  case 59: /* EqExp: EqExp EQUAL RelExp  */
#line 490 "src/parser.y"
                         {   //==
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::EQUAL, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1950 "src/parser.cpp"
    break;

  case 60: /* EqExp: EqExp NEQUAL RelExp  */
#line 494 "src/parser.y"
                          {    //!=
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::NEQUAL, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1959 "src/parser.cpp"
    break;

  case 61: /* LAndExp: EqExp  */
#line 501 "src/parser.y"
          {
        // fprintf(stderr,"LAndExp : EqExp\n");
        (yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1967 "src/parser.cpp"
    break;

  case 62: /* LAndExp: LAndExp AND EqExp  */
#line 506 "src/parser.y"
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::AND, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1976 "src/parser.cpp"
    break;

  case 63: /* LOrExp: LAndExp  */
#line 513 "src/parser.y"
            {
        // fprintf(stderr,"LOrExp : LAndExp\n");
        (yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1984 "src/parser.cpp"
    break;

  case 64: /* LOrExp: LOrExp OR LAndExp  */
#line 518 "src/parser.y"
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::OR, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1993 "src/parser.cpp"
    break;

  case 65: /* Type: INT  */
#line 524 "src/parser.y"
          {
        (yyval.type) = TypeSystem::intType;
        latestType = TypeSystem::intType;
    }
#line 2002 "src/parser.cpp"
    break;

  case 66: /* Type: VOID  */
#line 528 "src/parser.y"
           {
        (yyval.type) = TypeSystem::voidType;
        latestType = TypeSystem::voidType;
    }
#line 2011 "src/parser.cpp"
    break;

  case 67: /* Type: FLOAT  */
#line 532 "src/parser.y"
            {
        (yyval.type) = TypeSystem::floatType;
        latestType = TypeSystem::floatType;
    }
#line 2020 "src/parser.cpp"
    break;

  case 68: /* Type: CONST INT  */
#line 536 "src/parser.y"
                {
        (yyval.type) = TypeSystem::constIntType;
        latestType = TypeSystem::constIntType;
    }
#line 2029 "src/parser.cpp"
    break;

  case 69: /* Type: CONST FLOAT  */
#line 540 "src/parser.y"
                  {
        (yyval.type) = TypeSystem::constFloatType;
        latestType = TypeSystem::constFloatType;
    }
#line 2038 "src/parser.cpp"
    break;

  case 70: /* DeclStmt: Type VarDefList SEMICOLON  */
#line 546 "src/parser.y"
                                {
        (yyval.stmttype) = (yyvsp[-1].stmttype);
    }
#line 2046 "src/parser.cpp"
    break;

  case 71: /* VarDefList: VarDef COMMA VarDefList  */
#line 552 "src/parser.y"
                              {
        //fprintf(stderr,"Vardeflist set next:%s\n",((DeclStmt*)$3->getidName()).c_str());
        (yyval.stmttype) = (yyvsp[-2].stmttype);
        (yyvsp[-2].stmttype)->setNext((yyvsp[0].stmttype));
    }
#line 2056 "src/parser.cpp"
    break;

  case 72: /* VarDefList: VarDef  */
#line 557 "src/parser.y"
             {
        (yyval.stmttype) = (yyvsp[0].stmttype);
    }
#line 2064 "src/parser.cpp"
    break;

  case 73: /* VarDef: ID  */
#line 564 "src/parser.y"
         {
        // fprintf(stderr, "VarDef => ID\n");
        if(identifiers->localLookup((yyvsp[0].strtype))) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)(yyvsp[0].strtype));
            exit(EXIT_FAILURE);
        }
        IdentifierSymbolEntry *se = new IdentifierSymbolEntry(latestType, (yyvsp[0].strtype), identifiers->getLevel());
        identifiers->install((yyvsp[0].strtype), se);   //防止重复声明
        (yyval.stmttype) = new DeclStmt((yyvsp[0].strtype),new Id(se));
        delete [](yyvsp[0].strtype);
    }
#line 2080 "src/parser.cpp"
    break;

  case 74: /* VarDef: ID ASSIGN InitVal  */
#line 575 "src/parser.y"
                        {   //int a = 1; b func()
        if(identifiers->localLookup((yyvsp[-2].strtype))) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)(yyvsp[-2].strtype));
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "ID ASSIGN\n");
        IdentifierSymbolEntry *se = new IdentifierSymbolEntry(latestType, (yyvsp[-2].strtype), identifiers->getLevel());
        if((yyvsp[0].exprtype)->getSymPtr()->isVariable() && (dynamic_cast<IdentifierSymbolEntry*>((yyvsp[0].exprtype)->getSymPtr()))->isFunc()){
            Type *retType = dynamic_cast<FunctionType*>(dynamic_cast<IdentifierSymbolEntry *>((yyvsp[0].exprtype)->getSymPtr())->getType(true));
            if(retType->isVoid()) {
                fprintf(stderr,"right val can not be a void func call\n");
                exit(EXIT_FAILURE);
            } else  {
                //给左值赋予函数调用的返回值，这是在运行时才能决定的。TODO
            }
        }
        /*else {    //exp为标识符或数值或数组，数组暂不考虑
            if(latestType == TypeSystem::intType || latestType == TypeSystem::constintType) {
                if($3->getSymPtr()->getType()->isConstFloat() || $3->getSymPtr()->getType()->isFloat()) {
                    int val = $3->getSymPtr()->getFloatValue();
                    se->setIntValue(val);
                } else se->setIntValue($3->getSymPtr()->getIntValue());
            }else {
                if($3->getSymPtr()->getType()->isConstFloat() || $3->getSymPtr()->getType()->isFloat()) {
                    se->setFloatValue($3->getSymPtr()->getFloatValue());
                } else {
                    float val = (float)($3->getSymPtr()->getIntValue());
                    se->setFloatValue(val);
                }
            }
            if (latestType == TypeSystem::constintType || latestType == TypeSystem::constfloatType) {
                se->setConst();//标识这个标识符为常量
            }  
        }*/
        se->setIntValue((yyvsp[0].exprtype)->getSymPtr()->getIntValue());
        identifiers->install((yyvsp[-2].strtype), se);
        (yyval.stmttype) = new DeclStmt((yyvsp[-2].strtype),new Id(se),(yyvsp[0].exprtype));
        dynamic_cast<DeclStmt*>((yyval.stmttype))->getID()->getSymPtr()->setIntValue(se->getIntValue());
        dynamic_cast<DeclStmt*>((yyval.stmttype))->getID()->setIntValue(se->getIntValue());
        delete [](yyvsp[-2].strtype);  
    }
#line 2126 "src/parser.cpp"
    break;

  case 75: /* VarDef: ID ArrayIndices  */
#line 616 "src/parser.y"
                      {
        // int a[10];
        if(identifiers->localLookup((yyvsp[-1].strtype))) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)(yyvsp[-1].strtype));
            exit(EXIT_FAILURE);
        }
        ExprNode *node = (yyvsp[0].exprtype);
        std::vector<int> dimsize;
        while(node) {
            dimsize.push_back(node->getSymPtr()->getIntValue());
            node = node->getNext();
        }
        Type *type = TypeSystem::intType;
        if(dimsize.size()>1){
            while(!dimsize.empty()){
                type = new ArrayType(type, dimsize.back());
                dimsize.pop_back();
            }
        }else{
            type = new ArrayType(type, dimsize.back());
        }
        /*        
            Type* temp;
            while(!dimsize.empty()){
            temp = new ArrayType(type, dimsize.back());
            if(type->isArray())
                ((ArrayType*)type)->setArrayType(temp);
            type = temp;
            dimsize.pop_back();
        }*/
        arrayType = (ArrayType*)type;
        SymbolEntry* se = new IdentifierSymbolEntry(type, (yyvsp[-1].strtype), identifiers->getLevel());
        ((IdentifierSymbolEntry*)se)->setZero();
        int *p = new int[type->getSize()];
        ((IdentifierSymbolEntry*)se)->setIntArrValue(p);
        identifiers->install((yyvsp[-1].strtype), se);
        (yyval.stmttype) = new DeclStmt((yyvsp[-1].strtype), new Id(se));
        delete [](yyvsp[-1].strtype);
    }
#line 2170 "src/parser.cpp"
    break;

  case 76: /* @3: %empty  */
#line 655 "src/parser.y"
                             {
        if(identifiers->localLookup((yyvsp[-2].strtype))) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)(yyvsp[-2].strtype));
            exit(EXIT_FAILURE);
        }
        ExprNode *node = (yyvsp[-1].exprtype);
        std::vector<int> dimsize;
        while(node) {
            //fprintf(stderr, "array indices: val = %d\n", node->getIntValue());
            dimsize.push_back(node->getIntValue());
            node = node->getNext();
        }
        Type* type = TypeSystem::intType;
        if(dimsize.size()>1){
            while(!dimsize.empty()){
                type = new ArrayType(type, dimsize.back());
                dimsize.pop_back();
            }
        }else{
            type = new ArrayType(type, dimsize.back());
        }

        arrayType = (ArrayType*)type;
        SymbolEntry *se = new IdentifierSymbolEntry(type, (yyvsp[-2].strtype), identifiers->getLevel());
        (dynamic_cast<IdentifierSymbolEntry*>(se))->setArr();
        if(latestType == TypeSystem::constIntType || latestType == TypeSystem::constFloatType)
            (dynamic_cast<IdentifierSymbolEntry*>(se))->setConst();

        arrayIdx = 0;
        std::stack<InitValExpr*>().swap(stk);
        se = new IdentifierSymbolEntry(type, (yyvsp[-2].strtype), identifiers->getLevel());
        (yyval.se) = se;
        // 初始化列表不为空，用全局变量存
        arrayValue = new int[arrayType->getSize()];
    }
#line 2210 "src/parser.cpp"
    break;

  case 77: /* VarDef: ID ArrayIndices ASSIGN @3 InitVal  */
#line 689 "src/parser.y"
              {
        ((IdentifierSymbolEntry*)(yyvsp[-1].se))->setIntArrValue(arrayValue);
        if(((InitValExpr*)(yyvsp[0].exprtype))->isEmpty())
            ((IdentifierSymbolEntry*)(yyvsp[-1].se))->setZero();
        if(identifiers->localLookup((yyvsp[-4].strtype))) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)(yyvsp[-4].strtype));
            exit(EXIT_FAILURE);
        }
        identifiers->install((yyvsp[-4].strtype), (yyvsp[-1].se));  
        (yyval.stmttype) = new DeclStmt((yyvsp[-4].strtype), new Id((yyvsp[-1].se)), (yyvsp[0].exprtype));
        delete [](yyvsp[-4].strtype);
    }
#line 2227 "src/parser.cpp"
    break;

  case 78: /* ArrayIndices: LBRACKET Exp RBRACKET  */
#line 704 "src/parser.y"
                            {
        // fprintf(stderr, "ArrayIndices : LBRACKET Exp RBRACKET\n");
        if((yyvsp[-1].exprtype)->getSymPtr()->getType()->isConstInt() || (yyvsp[-1].exprtype)->getSymPtr()->getType()->isInt()) {
            (yyval.exprtype) = (yyvsp[-1].exprtype);
        }else if((yyvsp[-1].exprtype)->getSymPtr()->getType()->isArray()){
            (yyval.exprtype) = (yyvsp[-1].exprtype);
        }
        else{
            fprintf(stderr,"array index must be int. %s\n", (yyvsp[-1].exprtype)->getSymPtr()->getType()->toStr().c_str());
            exit(EXIT_FAILURE);
        }
        //fprintf(stderr, "array Indices exp val = %d\n", $2->getIntValue());
    }
#line 2245 "src/parser.cpp"
    break;

  case 79: /* ArrayIndices: LBRACKET Exp RBRACKET ArrayIndices  */
#line 717 "src/parser.y"
                                        {
        // fprintf(stderr, "ArrayIndices : LBRACKET Exp RBRACKET ArrayIndices\n");
        if((yyvsp[-2].exprtype)->getSymPtr()->getType()->isConstInt() || (yyvsp[-2].exprtype)->getSymPtr()->getType()->isInt()) {
            (yyval.exprtype) = (yyvsp[-2].exprtype);
            (yyvsp[-2].exprtype)->setNext((yyvsp[0].exprtype));
        }else {
            fprintf(stderr,"array index must be int.\n");
            exit(EXIT_FAILURE);
        }
        //fprintf(stderr, "array Indices exp val = %d\n", $2->getIntValue());
    }
#line 2261 "src/parser.cpp"
    break;

  case 80: /* InitVal: Exp  */
#line 730 "src/parser.y"
          {
        // fprintf(stderr,"InitVal : Exp \n");
        (yyval.exprtype) = (yyvsp[0].exprtype);
        if(!stk.empty()){
            // todo:float
            arrayValue[arrayIdx++] = (yyvsp[0].exprtype)->getIntValue();
            Type* arrType = stk.top()->getSymPtr()->getType();
            if(arrType == TypeSystem::intType){
                stk.top()->addExpr((yyvsp[0].exprtype));
            }
            else
                while(arrType){
                    // 栈顶是个数组类型，并且element不是int，可能是{2,3,4...，👉exp}加到一半了，push
                    if(((ArrayType*)arrType)->getElementType() != TypeSystem::intType){
                        arrType = ((ArrayType*)arrType)->getElementType();
                        // 数组的se都是constant吗，确实
                        SymbolEntry* se = new ConstantSymbolEntry(arrType, 0);
                        InitValExpr* list = new InitValExpr(se);
                        stk.top()->addExpr(list);
                        stk.push(list);
                    // element是int，不push
                    }else{
                        // 单独的数作为exp
                        stk.top()->addExpr((yyvsp[0].exprtype));
                        // 是否满，更换arrTy，pop
                        while(stk.top()->isFull() && stk.size() != (long unsigned int)leftbrace){
                            arrType = ((ArrayType*)arrType)->getArrayType();
                            stk.pop();
                        }
                        break;
                    }
                }
        }    
    }
#line 2300 "src/parser.cpp"
    break;

  case 81: /* InitVal: LBRACE RBRACE  */
#line 764 "src/parser.y"
                    {
        // fprintf(stderr,"InitVal : {}  \n");
        SymbolEntry* se;
        ExprNode* list;
        if(stk.empty()){
            // a={}，所以se是全零，initvalexpr包含符号表项
            memset(arrayValue, 0, arrayType->getSize());
            arrayIdx += arrayType->getSize() / TypeSystem::intType->getSize();
            se = new ConstantSymbolEntry(arrayType, 0);
            list = new InitValExpr(se);
        }else{
            Type* type = ((ArrayType*)(stk.top()->getSymPtr()->getType()))->getElementType();
            int len = type->getSize() / TypeSystem::intType->getSize();
            memset(arrayValue + arrayIdx, 0, type->getSize());
            arrayIdx += len;
            se = new ConstantSymbolEntry(type, 0);
            list = new InitValExpr(se);
            // = {0}
            stk.top()->addExpr(list);
            while(stk.top()->isFull() && stk.size() != (long unsigned int)leftbrace){
                stk.pop();
            }
        }
        (yyval.exprtype) = list;
    }
#line 2330 "src/parser.cpp"
    break;

  case 82: /* @4: %empty  */
#line 789 "src/parser.y"
             {
        // fprintf(stderr,"InitVal : { InitValList }  \n");
        SymbolEntry* se;
        if(!stk.empty()){
            // 当前的类型
            arrayType = (ArrayType*)(((ArrayType*)(stk.top()->getSymPtr()->getType()))->getElementType());
        }
        se = new ConstantSymbolEntry(arrayType, 0);
        if(arrayType->getElementType() != TypeSystem::intType){
            arrayType = (ArrayType*)(arrayType->getElementType());
        }
        InitValExpr* expr = new InitValExpr(se);
        // {...{}}
        if(!stk.empty())
            stk.top()->addExpr(expr);
        stk.push(expr);
        (yyval.exprtype) = expr;
        leftbrace++;
    }
#line 2354 "src/parser.cpp"
    break;

  case 83: /* InitVal: LBRACE @4 InitValList RBRACE  */
#line 807 "src/parser.y"
                         {
        leftbrace--;        
        // pop至当前维
        while(stk.top() != (yyvsp[-2].exprtype) && stk.size() > (long unsigned int)(leftbrace + 1))
            stk.pop();
        if(stk.top() == (yyvsp[-2].exprtype))
            stk.pop();
        (yyval.exprtype) = (yyvsp[-2].exprtype);
        // pop至非满
        if(!stk.empty())
            while(stk.top()->isFull() && stk.size() != (long unsigned int)leftbrace){
                stk.pop();
            }
        int size = ((ArrayType*)((yyval.exprtype)->getSymPtr()->getType()))->getSize()/ TypeSystem::intType->getSize();
        // 填0
        while(arrayIdx % size != 0)
            arrayValue[arrayIdx++] = 0;
        // 全局变量更新
        if(!stk.empty())
            arrayType = (ArrayType*)(((ArrayType*)(stk.top()->getSymPtr()->getType()))->getElementType());
    }
#line 2380 "src/parser.cpp"
    break;

  case 84: /* InitValList: InitVal  */
#line 831 "src/parser.y"
              {
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 2388 "src/parser.cpp"
    break;

  case 85: /* InitValList: InitValList COMMA InitVal  */
#line 834 "src/parser.y"
                                {
        (yyval.exprtype) = (yyvsp[-2].exprtype);
        // 加不加都行
        //$1->setNext($3);
    }
#line 2398 "src/parser.cpp"
    break;

  case 86: /* $@5: %empty  */
#line 843 "src/parser.y"
                   {
        // fprintf(stderr, "FuncDef => Type ID LPAREN FuncFParams RPAREN\n");
        // 无需实现函数重载，只用检查id是否被注册过
        SymbolEntry *se1 = identifiers->lookup((yyvsp[-1].strtype));
        if(se1 != nullptr) {
            fprintf(stderr,"funtion redefined.\n");
            exit(EXIT_FAILURE);
        }
        glob_param_cnt=0;
        Type *funcType = new FunctionType((yyvsp[-2].type),{},{});
        SymbolEntry *se = new IdentifierSymbolEntry(funcType, (yyvsp[-1].strtype), identifiers->getLevel());
        identifiers->install((yyvsp[-1].strtype), se);
        dynamic_cast<IdentifierSymbolEntry*>(se)->setFunc();
        identifiers = new SymbolTable(identifiers);
    }
#line 2418 "src/parser.cpp"
    break;

  case 87: /* $@6: %empty  */
#line 857 "src/parser.y"
                 {
        // 参数的scope应在函数作用域内?
        SymbolEntry* se = identifiers->lookup((yyvsp[-3].strtype));
        assert(se != nullptr);
        DeclStmt* node = dynamic_cast<DeclStmt*>((yyvsp[0].stmttype));
        std::vector<Type*> vec;
        std::vector<SymbolEntry*> se_vec;
        while(node) {
            vec.push_back(node->getID()->getSymPtr()->getType());
            se_vec.push_back(node->getID()->getSymPtr());
            node = dynamic_cast<DeclStmt*>(node->getNext());
        }
        (dynamic_cast<FunctionType*>(se->getType()))->setParams(vec);
        (dynamic_cast<FunctionType*>(se->getType()))->setParamsSe(se_vec);
    }
#line 2438 "src/parser.cpp"
    break;

  case 88: /* FuncDef: Type ID LPAREN $@5 FuncFParams $@6 RPAREN BlockStmt  */
#line 873 "src/parser.y"
    {
        /*StmtNode *stmt;
        if($8)
            stmt = dynamic_cast<CompoundStmt*>($8)->getStmtInBlock(); //TODO::这句可能会错，因为blockstmt可能为emptystmt
        StmtNode *laststmt;
        if(dynamic_cast<SeqNode*>(stmt) != NULL) laststmt = dynamic_cast<SeqNode*>(stmt)->getStmt(2);
        else laststmt = stmt;*/
        //laststmt此时为函数定义block中的最后一句
        //ReturnStmt *retstmt = dynamic_cast<ReturnStmt*>(laststmt);
        /*if($1->isVoid()) {
            if(retstmt != NULL && retstmt->getRetValue() != nullptr) {
                fprintf(stderr,"void function should not have returnstmt.\n");
                exit(EXIT_FAILURE);
            } else fprintf(stderr,"void function check ok\n");
        } else {
            if(retstmt == NULL) {
                fprintf(stderr,"non-void function should have returnstmt.\n");
                exit(EXIT_FAILURE);
            }
            //getType(true)是因为如果返回语句是类似： return func(); 即函数递归，则要获取的是函数的返回类型
            Type *retType = retstmt->getRetValue()->getSymPtr()->getType(true);
            if(($1->isIntLike() && retType->isFloatLike()) ||($1->isFloatLike()&&retType->isIntLike())){
                fprintf(stderr,"In funcdef : function return type not match.\n");
                exit(EXIT_FAILURE);
            }
            if($1 != retType) {
                fprintf(stderr,"In funcdef : function return type not match.\n");
                exit(EXIT_FAILURE);
            }
        }*/
        SymbolEntry *se;
        se = identifiers->getPrev()->lookup((yyvsp[-6].strtype));
        assert(se != nullptr);
        (yyval.stmttype) = new FunctionDef(se, (DeclStmt*)(yyvsp[-3].stmttype), (yyvsp[0].stmttype));
        SymbolTable *top = identifiers;
        identifiers = identifiers->getPrev();
        delete top;
        delete [](yyvsp[-6].strtype);
    }
#line 2482 "src/parser.cpp"
    break;

  case 89: /* FuncFParams: FuncFParam  */
#line 915 "src/parser.y"
                 {
        (yyval.stmttype) = (yyvsp[0].stmttype);
        //fprintf(stderr,"3\n");
    }
#line 2491 "src/parser.cpp"
    break;

  case 90: /* FuncFParams: FuncFParam COMMA FuncFParams  */
#line 919 "src/parser.y"
                                   {
        (yyval.stmttype) = (yyvsp[-2].stmttype);
        (yyvsp[-2].stmttype)->setNext((yyvsp[0].stmttype));
        //fprintf(stderr,"2\n");
    }
#line 2501 "src/parser.cpp"
    break;

  case 91: /* FuncFParams: %empty  */
#line 924 "src/parser.y"
             {(yyval.stmttype) = nullptr;}
#line 2507 "src/parser.cpp"
    break;

  case 92: /* FuncFParam: Type ID  */
#line 928 "src/parser.y"
              {
        // 对应 int func(int a)的情况
        SymbolEntry *se;
        se = new IdentifierSymbolEntry((yyvsp[-1].type), (yyvsp[0].strtype), identifiers->getLevel(), glob_param_cnt++);
        identifiers->install((yyvsp[0].strtype), se);
        ((IdentifierSymbolEntry*)se)->setLabel();
        ((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
        (yyval.stmttype) = new DeclStmt((yyvsp[0].strtype),new Id(se));
        delete [](yyvsp[0].strtype);
        //fprintf(stderr,"1\n");
    }
#line 2523 "src/parser.cpp"
    break;

  case 93: /* FuncFParam: Type ID FuncArrayIndices  */
#line 939 "src/parser.y"
                               {
        SymbolEntry* se;
        ExprNode* temp = (yyvsp[0].exprtype);
        Type* arr = TypeSystem::intType;
        Type* arr1;
        std::stack<ExprNode*> stk;
        while(temp){
            stk.push(temp);
            temp = (ExprNode*)(temp->getNext());
        }
        while(!stk.empty()){
            arr1 = new ArrayType(arr, stk.top()->getIntValue());
            if(arr->isArray())
                ((ArrayType*)arr)->setArrayType(arr1);
            arr = arr1;
            stk.pop();
        }
    
        se = new IdentifierSymbolEntry(arr, (yyvsp[-1].strtype), identifiers->getLevel(), glob_param_cnt++);
        identifiers->install((yyvsp[-1].strtype), se);
        ((IdentifierSymbolEntry*)se)->setLabel();
        ((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
        (yyval.stmttype) = new DeclStmt((yyvsp[-1].strtype), new Id(se));
        delete [](yyvsp[-1].strtype);
    }
#line 2553 "src/parser.cpp"
    break;

  case 94: /* FuncArrayIndices: LBRACKET RBRACKET  */
#line 966 "src/parser.y"
                        {
        // []
        (yyval.exprtype) = new ExprNode(nullptr);
    }
#line 2562 "src/parser.cpp"
    break;

  case 95: /* FuncArrayIndices: FuncArrayIndices LBRACKET Exp RBRACKET  */
#line 970 "src/parser.y"
                                             {
        // [][exp]
        (yyval.exprtype) = (yyvsp[-3].exprtype);
        (yyval.exprtype)->setNext((yyvsp[-1].exprtype));
    }
#line 2572 "src/parser.cpp"
    break;


#line 2576 "src/parser.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 975 "src/parser.y"


int yyerror(char const* message)
{
    std::cerr<<message<<std::endl;
    return -1;
}
