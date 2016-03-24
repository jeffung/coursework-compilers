/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "expr-codegen.y" /* yacc.c:339  */
 
#include <iostream>
#include <ostream>
#include "exprdefs.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/Verifier.h"
#include <cstdio> 
#include <stdexcept>


using namespace llvm;
using namespace std;

#include "decaf-ast.cc"

bool printAST = true;

// this global variable contains all the generated code
static Module *TheModule;

// this is the method used to construct the LLVM intermediate code (IR)
static IRBuilder<> Builder(getGlobalContext());
// the calls to getGlobalContext() in the init above and in the
// following code ensures that we are incrementally generating
// instructions in the right order
//static std::map<std::string, Value*> NamedValues;

typedef map<string, Value*> symbol_table;

typedef list<symbol_table*> symbol_table_list;

// program stack
static symbol_table_list symtbl_list;

// active record
static symbol_table *currentSymTable;

Type *getLLVMType(decafType ty) {
 switch (ty) {
   case voidTy: return Builder.getVoidTy();
   case intTy: return Builder.getInt32Ty();
   case boolTy: return Builder.getInt1Ty();
   case stringTy: return Builder.getInt8PtrTy();
   default: throw runtime_error("unknown type");
 }
}

Value* access_symtbl(string ident) {
  for (symbol_table_list::iterator i = symtbl_list.begin(); i != symtbl_list.end(); ++i) {
    symbol_table::iterator find_ident;
    
    if ((find_ident = (*i)->find(ident)) != (*i)->end())
      return find_ident->second;
  }

  return NULL;
}

void updateSymTable(string ident,Value* alloca) {
    currentSymTable = symtbl_list.front();
    //currentSymTable[ident]->lineno = variableInfo.

    (*currentSymTable)[ident] = alloca;
    //symbol_table *yes = symtbl_list.front();

    //cout << (*yes)[ident]->location << endl;
    //cout << currentSymTable[ident]->varname << endl;
    //cout << currentSymTable[ident]->type << endl;
    //cout << currentSymTable[ident]->lineno << endl;
}


AllocaInst *defineVariable(Type *llvmTy, string ident)
{
 AllocaInst *Alloca =
    Builder.CreateAlloca(llvmTy, 0, ident.c_str());
 updateSymTable(ident, Alloca);
 return Alloca;
}


Function *gen_print_int_def() {
  // create a extern definition for print_int
  std::vector<Type*> args;
  args.push_back(IntegerType::get(getGlobalContext(), 32)); // print_int takes one integer argument
  FunctionType *print_int_type = FunctionType::get(IntegerType::get(getGlobalContext(), 32), args, false);
  return Function::Create(print_int_type, Function::ExternalLinkage, "print_int", TheModule);
}

// we also have to create a main function that contains
// all the code generated for the expression and the print_int call
Function *gen_main_def(Value *RetVal, Function *print_int) {
  if (RetVal == 0) {
    throw runtime_error("something went horribly wrong\n");
  }
  // create the top-level definition for main
  FunctionType *FT = FunctionType::get(IntegerType::get(getGlobalContext(), 32), false);
  Function *TheFunction = Function::Create(FT, Function::ExternalLinkage, "main", TheModule);
  if (TheFunction == 0) {
    throw runtime_error("empty function block"); 
  }
  // Create a new basic block which contains a sequence of LLVM instructions
  BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", TheFunction);
  // All subsequent calls to IRBuilder will place instructions in this location
  Builder.SetInsertPoint(BB);



  
  Function *CalleeF = TheModule->getFunction(print_int->getName());
  if (CalleeF == 0) {
    throw runtime_error("could not find the function print_int\n");
  }

  // print the value of the expression and we are done
  Value *CallF = Builder.CreateCall(CalleeF, RetVal, "calltmp");
  
  // Finish off the function.
  // return 0 from main, which is EXIT_SUCCESS
  Builder.CreateRet(ConstantInt::get(getGlobalContext(), APInt(32, 0)));
  return TheFunction;
}


#line 194 "expr-codegen.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "expr-codegen.tab.h".  */
#ifndef YY_YY_EXPR_CODEGEN_TAB_H_INCLUDED
# define YY_YY_EXPR_CODEGEN_TAB_H_INCLUDED
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
    T_ASSIGN = 258,
    T_CLASS = 259,
    T_COMMA = 260,
    T_DIV = 261,
    T_PLUS = 262,
    T_MINUS = 263,
    T_MULT = 264,
    T_MOD = 265,
    T_LEFTSHIFT = 266,
    T_RIGHTSHIFT = 267,
    T_EQ = 268,
    T_LT = 269,
    T_LPAREN = 270,
    T_RPAREN = 271,
    T_SEMICOLON = 272,
    T_EXTERN = 273,
    T_LCB = 274,
    T_RCB = 275,
    T_INTTYPE = 276,
    T_STRINGTYPE = 277,
    T_BOOL = 278,
    T_VOID = 279,
    T_RETURN = 280,
    T_INTCONSTANT = 281,
    T_CHARCONSTANT = 282,
    T_TRUE = 283,
    T_FALSE = 284,
    T_ID = 285,
    T_STRINGCONSTANT = 286
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 129 "expr-codegen.y" /* yacc.c:355  */

  class decafAST *ast;
  int number;
  std::string *sval;
  int decaftype;

#line 273 "expr-codegen.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_EXPR_CODEGEN_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 290 "expr-codegen.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   114

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  111

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   168,   168,   171,   203,   211,   217,   223,   228,   234,
     242,   248,   253,   265,   274,   282,   290,   297,   303,   308,
     319,   337,   347,   356,   362,   367,   377,   389,   395,   405,
     411,   421,   430,   438,   444,   450,   456,   467,   478,   485,
     494,   502,   508,   520,   531,   537,   541,   543,   545,   547,
     549,   551,   553,   555,   557,   563,   569,   575,   581,   587
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_ASSIGN", "T_CLASS", "T_COMMA",
  "T_DIV", "T_PLUS", "T_MINUS", "T_MULT", "T_MOD", "T_LEFTSHIFT",
  "T_RIGHTSHIFT", "T_EQ", "T_LT", "T_LPAREN", "T_RPAREN", "T_SEMICOLON",
  "T_EXTERN", "T_LCB", "T_RCB", "T_INTTYPE", "T_STRINGTYPE", "T_BOOL",
  "T_VOID", "T_RETURN", "T_INTCONSTANT", "T_CHARCONSTANT", "T_TRUE",
  "T_FALSE", "T_ID", "T_STRINGCONSTANT", "$accept", "start", "program",
  "extern_list", "extern_defn", "extern_type_list", "extern_type",
  "decafclass", "method_decl_list", "method_decl", "param_list",
  "param_comma_list", "method_block", "var_decl_list", "var_decl",
  "var_list", "method_type", "type", "statement_list", "statement",
  "assign", "method_call", "method_arg_list", "method_arg", "rvalue",
  "expr", "constant", "bool_constant", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286
};
# endif

#define YYPACT_NINF -59

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-59)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -59,     3,   -59,    -2,   -59,   -12,    19,   -59,   -59,     0,
     -59,   -59,   -59,    11,   -59,    61,    64,    24,    57,   -59,
      35,     5,    68,   -59,   -59,    72,    73,   -59,    75,    84,
     -59,   -17,   -17,   -59,    76,     1,    78,   -59,    62,    79,
     -59,   -59,    77,    92,    77,   -17,   -59,   -17,   -59,    -5,
     -17,    -4,    69,   -59,    83,     2,    85,    80,    -5,    86,
      87,   -59,    71,   -59,   -59,    46,    46,    40,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
      39,   -59,   -59,    50,   -59,    89,   101,    50,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    90,   -59,    40,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,   -59,   -59,   -59,
     -59
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     2,     0,     1,     0,     0,     4,     3,     0,
      29,    30,    27,     0,    28,     0,     0,     0,     0,    14,
       0,     0,     0,    12,    13,     0,     0,    10,     0,     8,
      11,    18,    18,     7,     0,     0,     0,    17,     0,     0,
       6,     9,     0,    20,     0,    23,    15,     0,    16,    32,
      23,     0,     0,    19,     0,     0,     0,     0,    32,     0,
       0,    22,     0,    24,    26,     0,     0,     0,    35,    21,
      31,    33,    34,    25,    55,    56,    58,    59,    43,    44,
       0,    45,    57,    37,    42,     0,    39,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,     0,
      49,    46,    47,    48,    52,    50,    51,    53,    54,    36,
      40
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -59,   -59,   -59,   -59,   -59,    74,   -59,   -59,   -59,    93,
      81,    63,    44,    58,   -59,   -59,   -59,    -6,    54,   -59,
     -59,   -59,    15,   -59,   -59,   -58,   -59,   -59
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,     3,     7,    28,    29,     8,    18,    19,
      36,    37,    56,    49,    50,    51,    13,    38,    57,    58,
      59,    60,    85,    86,    79,    87,    81,    82
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      14,    62,     5,     4,    10,    66,    11,    80,    83,    20,
      95,    96,    20,    63,    45,    30,     6,    67,     9,    15,
      54,    26,    10,    27,    11,    55,    10,    27,    11,    30,
     100,   101,   102,   103,   104,   105,   106,   107,   108,    52,
      10,    16,    11,    12,    52,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    22,    97,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    25,    74,    75,    76,    77,
      78,    84,    74,    75,    76,    77,    78,    23,    10,    21,
      11,    17,    10,    31,    11,    17,    46,    32,    48,    35,
      33,    34,    43,    40,    42,    44,    45,    47,    65,    64,
      69,    73,    68,    71,    72,    98,    99,   109,    61,    41,
      53,    24,    70,    39,   110
};

static const yytype_uint8 yycheck[] =
{
       6,     5,     4,     0,    21,     3,    23,    65,    66,    15,
      13,    14,    18,    17,    19,    21,    18,    15,    30,    19,
      25,    16,    21,    22,    23,    30,    21,    22,    23,    35,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    45,
      21,    30,    23,    24,    50,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    30,    16,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    30,    26,    27,    28,    29,
      30,    31,    26,    27,    28,    29,    30,    20,    21,    15,
      23,    24,    21,    15,    23,    24,    42,    15,    44,     5,
      17,    16,    30,    17,    16,    16,    19,     5,    15,    30,
      20,    30,    17,    17,    17,    16,     5,    17,    50,    35,
      47,    18,    58,    32,    99
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    33,    34,    35,     0,     4,    18,    36,    39,    30,
      21,    23,    24,    48,    49,    19,    30,    24,    40,    41,
      49,    15,    30,    20,    41,    30,    16,    22,    37,    38,
      49,    15,    15,    17,    16,     5,    42,    43,    49,    42,
      17,    37,    16,    30,    16,    19,    44,     5,    44,    45,
      46,    47,    49,    43,    25,    30,    44,    50,    51,    52,
      53,    45,     5,    17,    30,    15,     3,    15,    17,    20,
      50,    17,    17,    30,    26,    27,    28,    29,    30,    56,
      57,    58,    59,    57,    31,    54,    55,    57,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    16,    16,     5,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    17,
      54
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    32,    33,    34,    35,    35,    36,    36,    37,    37,
      38,    38,    39,    40,    40,    41,    41,    42,    42,    43,
      43,    44,    45,    45,    46,    47,    47,    48,    48,    49,
      49,    50,    50,    51,    51,    51,    51,    52,    53,    54,
      54,    55,    55,    56,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    58,    58,    58,    59,    59
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     0,     7,     6,     1,     3,
       1,     1,     5,     2,     1,     6,     6,     1,     0,     4,
       2,     4,     2,     0,     2,     3,     2,     1,     1,     1,
       1,     2,     0,     2,     2,     2,     5,     3,     4,     1,
       3,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
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
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 3:
#line 171 "expr-codegen.y" /* yacc.c:1646  */
    {
    /*ProgramAST *prog = new ProgramAST((decafStmtLst *)$1, (decafStmtList *)$2); 
          
    if (printAST) {
          cout << getString(prog) << endl;
    }*/
    
    // IRBuilder does constant folding by default so all the
    // addition and subtraction operations are computed and always result in
    // a constant integer value in this simple example
    Value *RetVal = (yyvsp[-1].ast)->Codegen();
    delete (yyvsp[-1].ast); // get rid of abstract syntax tree    
    //delete $2; // get rid of abstract syntax tree   

    // we create an implicit print_int function call to print
    // out the value of the expression.
    Function *print_int = gen_print_int_def();

    // create the top-level function called main
    Function *TheFunction = gen_main_def(RetVal, print_int);
     
    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);
    
  }
#line 1469 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 203 "expr-codegen.y" /* yacc.c:1646  */
    { 
    decafStmtList *slist = (decafStmtList *)(yyvsp[-1].ast); 
    slist->push_back((yyvsp[0].ast)); 
    (yyval.ast) = slist; 
  }
#line 1479 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 211 "expr-codegen.y" /* yacc.c:1646  */
    { 
    decafStmtList *slist = new decafStmtList(); 
    (yyval.ast) = slist; 
  }
#line 1488 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 217 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new ExternAST((decafType)(yyvsp[-5].decaftype), *(yyvsp[-4].sval), (TypedSymbolListAST *)(yyvsp[-2].ast)); delete (yyvsp[-4].sval); 
  }
#line 1496 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 223 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new ExternAST((decafType)(yyvsp[-4].decaftype), *(yyvsp[-3].sval), NULL); delete (yyvsp[-3].sval); 
  }
#line 1504 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 228 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new TypedSymbolListAST(string(""), (decafType)(yyvsp[0].decaftype)); 
  }
#line 1512 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 234 "expr-codegen.y" /* yacc.c:1646  */
    { 
      TypedSymbolListAST *tlist = (TypedSymbolListAST *)(yyvsp[0].ast); 

      tlist->push_front(string(""), (decafType)(yyvsp[-2].decaftype)); 
      (yyval.ast) = tlist;
  }
#line 1523 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 242 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.decaftype) = stringTy; 
  }
#line 1531 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 248 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.decaftype) = (yyvsp[0].decaftype); 
  }
#line 1539 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 253 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new ClassAST(*(yyvsp[-3].sval), (decafStmtList *)(yyvsp[-1].ast)); 

    delete (yyvsp[-3].sval); 
  }
#line 1549 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 265 "expr-codegen.y" /* yacc.c:1646  */
    { 
    decafStmtList *slist = (decafStmtList *)(yyvsp[-1].ast); 
    
    slist->push_back((yyvsp[0].ast)); 
    (yyval.ast) = slist; 
  }
#line 1560 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 274 "expr-codegen.y" /* yacc.c:1646  */
    { 
    decafStmtList *slist = new decafStmtList(); 
    
    slist->push_back((yyvsp[0].ast)); 
    (yyval.ast) = slist; 
  }
#line 1571 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 282 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new MethodDeclAST(voidTy, *(yyvsp[-4].sval), (TypedSymbolListAST *)(yyvsp[-2].ast), (MethodBlockAST *)(yyvsp[0].ast)); 
    
    delete (yyvsp[-4].sval); 
  }
#line 1581 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 290 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new MethodDeclAST((decafType)(yyvsp[-5].decaftype), *(yyvsp[-4].sval), (TypedSymbolListAST *)(yyvsp[-2].ast), (MethodBlockAST *)(yyvsp[0].ast)); 

    delete (yyvsp[-4].sval); 
  }
#line 1591 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 297 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = (yyvsp[0].ast); 
  }
#line 1599 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 303 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = NULL; 
  }
#line 1607 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 308 "expr-codegen.y" /* yacc.c:1646  */
    { 
    TypedSymbolListAST *tlist = (TypedSymbolListAST *)(yyvsp[0].ast); 
    
    tlist->push_front(*(yyvsp[-2].sval), (decafType)(yyvsp[-3].decaftype)); 
    (yyval.ast) = tlist;
    
    delete (yyvsp[-2].sval);
  }
#line 1620 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 319 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new TypedSymbolListAST(*(yyvsp[0].sval), (decafType)(yyvsp[-1].decaftype)); 

    delete (yyvsp[0].sval); 
  }
#line 1630 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 337 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new MethodBlockAST((decafStmtList *)(yyvsp[-2].ast), (decafStmtList *)(yyvsp[-1].ast));
  }
#line 1638 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 347 "expr-codegen.y" /* yacc.c:1646  */
    { 
    decafStmtList *slist = (decafStmtList *)(yyvsp[0].ast); 

    slist->push_front((yyvsp[-1].ast)); 
    (yyval.ast) = slist; 
  }
#line 1649 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 356 "expr-codegen.y" /* yacc.c:1646  */
    { 
    decafStmtList *slist = new decafStmtList(); (yyval.ast) = slist; 
  }
#line 1657 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 362 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = (yyvsp[-1].ast); 
  }
#line 1665 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 367 "expr-codegen.y" /* yacc.c:1646  */
    { 
        TypedSymbolListAST *tlist = (TypedSymbolListAST *)(yyvsp[-2].ast); 
        tlist->new_sym(*(yyvsp[0].sval)); 
        (yyval.ast) = tlist;

        delete (yyvsp[0].sval);
    }
#line 1677 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 377 "expr-codegen.y" /* yacc.c:1646  */
    { 
      (yyval.ast) = new TypedSymbolListAST(*(yyvsp[0].sval), (decafType)(yyvsp[-1].decaftype)); 

      delete (yyvsp[0].sval); 
    }
#line 1687 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 389 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.decaftype) = voidTy; 
  }
#line 1695 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 395 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.decaftype) = (yyvsp[0].decaftype); 
  }
#line 1703 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 405 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.decaftype) = intTy; 
  }
#line 1711 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 411 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.decaftype) = boolTy; 
  }
#line 1719 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 421 "expr-codegen.y" /* yacc.c:1646  */
    { 
    decafStmtList *slist = (decafStmtList *)(yyvsp[0].ast); 

    slist->push_front((yyvsp[-1].ast)); 
    (yyval.ast) = slist; 
  }
#line 1730 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 430 "expr-codegen.y" /* yacc.c:1646  */
    { 
    decafStmtList *slist = new decafStmtList(); 

    (yyval.ast) = slist; 
  }
#line 1740 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 438 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = (yyvsp[-1].ast); 
  }
#line 1748 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 444 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = (yyvsp[-1].ast);
  }
#line 1756 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 450 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = (yyvsp[-1].ast);
  }
#line 1764 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 456 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = new ReturnStmt((yyvsp[-2].ast));
  }
#line 1772 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 467 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new AssignVarAST(*(yyvsp[-2].sval), (yyvsp[0].ast)); 
    delete (yyvsp[-2].sval); 
  }
#line 1781 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 478 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = new MethodCallAST(*(yyvsp[-3].sval), (decafStmtList *)(yyvsp[-1].ast));
    
    delete (yyvsp[-3].sval);
  }
#line 1791 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 485 "expr-codegen.y" /* yacc.c:1646  */
    {
    decafStmtList *slist = new decafStmtList()); 

    slist->push_front((yyvsp[0].ast)); 
    (yyval.ast) = slist; 
  }
#line 1802 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 494 "expr-codegen.y" /* yacc.c:1646  */
    {
    decafStmtList *slist = (decafStmtList *)(yyvsp[0].ast); 

    slist->push_front((yyvsp[-2].ast)); 
    (yyval.ast) = slist; 
  }
#line 1813 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 502 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = (yyvsp[0].ast);
  }
#line 1821 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 508 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = new StringConstantAST(*(yyvsp[0].sval));

    delete (yyvsp[0].sval);
  }
#line 1831 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 520 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new VariableExprAST(*(yyvsp[0].sval)); 

    delete (yyvsp[0].sval); 
  }
#line 1841 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 531 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = (yyvsp[0].ast);
  }
#line 1849 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 537 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = (yyvsp[0].ast); 
  }
#line 1857 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 542 "expr-codegen.y" /* yacc.c:1646  */
    { (yyval.ast) = new BinaryExprAST(T_PLUS, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1863 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 544 "expr-codegen.y" /* yacc.c:1646  */
    {  (yyval.ast) = new BinaryExprAST(T_MINUS, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1869 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 546 "expr-codegen.y" /* yacc.c:1646  */
    {  (yyval.ast) = new BinaryExprAST(T_MULT, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1875 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 548 "expr-codegen.y" /* yacc.c:1646  */
    {  (yyval.ast) = new BinaryExprAST(T_DIV, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1881 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 550 "expr-codegen.y" /* yacc.c:1646  */
    {  (yyval.ast) = new BinaryExprAST(T_LEFTSHIFT, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1887 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 552 "expr-codegen.y" /* yacc.c:1646  */
    {  (yyval.ast) = new BinaryExprAST(T_RIGHTSHIFT, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1893 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 554 "expr-codegen.y" /* yacc.c:1646  */
    {  (yyval.ast) = new BinaryExprAST(T_MOD, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1899 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 556 "expr-codegen.y" /* yacc.c:1646  */
    { (yyval.ast) = new BinaryExprAST(T_EQ, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1905 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 558 "expr-codegen.y" /* yacc.c:1646  */
    { (yyval.ast) = new BinaryExprAST(T_LT, (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1911 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 563 "expr-codegen.y" /* yacc.c:1646  */
    { 
    (yyval.ast) = new NumberExprAST((yyvsp[0].number)); 
  }
#line 1919 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 569 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = new CharExprAST((yyvsp[0].number));
  }
#line 1927 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 575 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = (yyvsp[0].ast);
  }
#line 1935 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 581 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = new BoolExprAST(1);
  }
#line 1943 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 587 "expr-codegen.y" /* yacc.c:1646  */
    {
    (yyval.ast) = new BoolExprAST(0);
  }
#line 1951 "expr-codegen.tab.c" /* yacc.c:1646  */
    break;


#line 1955 "expr-codegen.tab.c" /* yacc.c:1646  */
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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

yyreturn:
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
                  yystos[*yyssp], yyvsp);
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
#line 591 "expr-codegen.y" /* yacc.c:1906  */


/////////////////////
// CODE GENERATION //
/////////////////////

Value *TypedSymbolListAST::Codegen() {
  // TypedSymbol *symbol = arglist.front();
  Value* definedvar = defineVariable(getLLVMType(listType),arglist.front()->Sym);
  
  return definedvar;
}

Value *VariableExprAST::Codegen() {
  //Value* definedvar = defineVariable(type,ident);
  Value *getvar = access_symtbl(Name);

  return Builder.CreateLoad(getvar, Name.c_str());
}

Value *NumberExprAST::Codegen() {
  return ConstantInt::get(getGlobalContext(), APInt(32, Val));
}

Value *BoolExprAST::Codegen() {
  return ConstantInt::get(getGlobalContext(), APInt(1, Val));
}

Value *BinaryExprAST::Codegen() {
  Value *L = LHS->Codegen();
  Value *R = RHS->Codegen();

  if (L == 0 || R == 0) {
    return 0;
  }
  
  switch (Op) {
    case T_PLUS: return Builder.CreateAdd(L, R, "addtmp");
    case T_MINUS: return Builder.CreateSub(L, R, "subtmp");
    case T_MULT: return Builder.CreateMul(L, R, "multmp");
    case T_DIV: return Builder.CreateSDiv(L, R, "divtmp");
    case T_LEFTSHIFT: return Builder.CreateShl(L, R, "shltmp");
    case T_RIGHTSHIFT: return Builder.CreateLShr(L, R, "shrtmp");
    case T_MOD: return Builder.CreateSRem(L, R, "remtmp");
    case T_EQ: return Builder.CreateFCmpUEQ(L, R, "booltmp");
    case T_LT: return Builder.CreateFCmpUEQ(L, R, "booltmp");
    default: throw runtime_error("what operator is this? never heard of it.");
  }
}

Value *BinaryReturnAST::Codegen() {
  Value *RetVal = Value->Codegen();

  if (RetVal == 0) {
    return Builder.CreateRetVoid();
  }
}

Value *MethodCallAST::Codegen() {
  // TODO
}

//////////
// MAIN // 
//////////

int main() {
  fprintf(stderr, "ready> ");

  // initialize LLVM
  LLVMContext &Context = getGlobalContext();

  // make the module, which holds all the code.
  TheModule = new Module("module for very simple expressions", Context);

  // parse the input and create the abstract syntax tree
  int retval = yyparse();

  // print out all of the generated code to stderr
  TheModule->dump();

  return(retval >= 1 ? 1 : 0);
}
