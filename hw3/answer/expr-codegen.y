%{ 
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


// this global variable contains all the generated code
static Module *TheModule;

// this is the method used to construct the LLVM intermediate code (IR)
static IRBuilder<> Builder(getGlobalContext());
// the calls to getGlobalContext() in the init above and in the
// following code ensures that we are incrementally generating
// instructions in the right order
static std::map<std::string, Value*> NamedValues;


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

%}

%union{
  class decafAST *ast;
  int number;
  std::string *sval;
  int decaftype;
}

%token T_ASSIGN T_CLASS T_COMMA T_DIV T_PLUS T_MINUS T_MULT T_MOD T_LEFTSHIFT T_RIGHTSHIFT
%token T_EQ T_LT T_LPAREN T_RPAREN T_SEMICOLON T_EXTERN T_LCB T_RCB
%token T_INTTYPE T_STRINGTYPE T_BOOL T_VOID

%token <number> T_INTCONSTANT
%token <sval> T_ID

%type <decaftype> type method_type extern_type

%type <ast> expr constant
%type <ast> rvalue assign
%type <ast> block method_block statement statement_list var_decl_list var_decl var_list param_list param_comma_list 
%type <ast> method_decl method_decl_list extern_type_list extern_defn
%type <ast> extern_list decafclass

%left T_DIV T_PLUS T_MINUS T_MULT T_MOD T_LEFTSHIFT T_RIGHTSHIFT
%left T_EQ T_LT

%%

start: program

program: extern_list decafclass
    { 
        ProgramAST *prog = new ProgramAST((decafStmtList *)$1, (ClassAST *)$2); 
        if (printAST) {
           // cout << getString(prog) << endl;
        }
        // IRBuilder does constant folding by default so all the
       // addition and subtraction operations are computed and always result in
       // a constant integer value in this simple example
       Value *RetVal = $1->Codegen();
       delete $1; // get rid of abstract syntax tree    
       delete $2; // get rid of abstract syntax tree   

       // we create an implicit print_int function call to print
       // out the value of the expression.
       Function *print_int = gen_print_int_def();

       // create the top-level function called main
       Function *TheFunction = gen_main_def(RetVal, print_int);
       // Validate the generated code, checking for consistency.
       verifyFunction(*TheFunction); 
    }

extern_list: extern_list extern_defn
    { decafStmtList *slist = (decafStmtList *)$1; slist->push_back($2); $$ = slist; }
    | /* extern_list can be empty */
    { decafStmtList *slist = new decafStmtList(); $$ = slist; }
    ;

extern_defn: T_EXTERN method_type T_ID T_LPAREN extern_type_list T_RPAREN T_SEMICOLON
    { $$ = new ExternAST((decafType)$2, *$3, (TypedSymbolListAST *)$5); delete $3; }
    | T_EXTERN method_type T_ID T_LPAREN T_RPAREN T_SEMICOLON
    { $$ = new ExternAST((decafType)$2, *$3, NULL); delete $3; }
    ;

extern_type_list: extern_type
    { $$ = new TypedSymbolListAST(string(""), (decafType)$1); }
    | extern_type T_COMMA extern_type_list
    { 
        TypedSymbolListAST *tlist = (TypedSymbolListAST *)$3; 
        tlist->push_front(string(""), (decafType)$1); 
        $$ = tlist;
    }
    ;

extern_type: T_STRINGTYPE
    { $$ = stringTy; }
    | type
    { $$ = $1; }
    ;

decafclass: T_CLASS T_ID T_LCB method_decl_list T_RCB
    { $$ = new ClassAST(*$2, (decafStmtList *)$4); delete $2; }


method_decl_list: method_decl_list method_decl 
    { decafStmtList *slist = (decafStmtList *)$1; slist->push_back($2); $$ = slist; }
    | method_decl
    { decafStmtList *slist = new decafStmtList(); slist->push_back($1); $$ = slist; }
    ;

method_decl: T_VOID T_ID T_LPAREN param_list T_RPAREN method_block
    { $$ = new MethodDeclAST(voidTy, *$2, (TypedSymbolListAST *)$4, (MethodBlockAST *)$6); delete $2; }
    | type T_ID T_LPAREN param_list T_RPAREN method_block
    { $$ = new MethodDeclAST((decafType)$1, *$2, (TypedSymbolListAST *)$4, (MethodBlockAST *)$6); delete $2; }
    ;

method_type: T_VOID
    { $$ = voidTy; }
    | type
    { $$ = $1; }
    ;

param_list: param_comma_list
    { $$ = $1; }
    | /* empty */
    { $$ = NULL; }
    ;

param_comma_list: type T_ID T_COMMA param_comma_list
    { 
        TypedSymbolListAST *tlist = (TypedSymbolListAST *)$4; 
        tlist->push_front(*$2, (decafType)$1); 
        $$ = tlist;
        delete $2;
    }
    | type T_ID
    { $$ = new TypedSymbolListAST(*$2, (decafType)$1); delete $2; }
    ;

type: T_INTTYPE
    { $$ = intTy; }
    | T_BOOL
    { $$ = boolTy; }
    ;

block: T_LCB var_decl_list statement_list T_RCB
    { $$ = new BlockAST((decafStmtList *)$2, (decafStmtList *)$3); }

method_block: T_LCB var_decl_list statement_list T_RCB
    { $$ = new MethodBlockAST((decafStmtList *)$2, (decafStmtList *)$3); }

var_decl_list: var_decl var_decl_list
    { decafStmtList *slist = (decafStmtList *)$2; slist->push_front($1); $$ = slist; }
    | /* empty */
    { decafStmtList *slist = new decafStmtList(); $$ = slist; }
    ;

var_decl: var_list T_SEMICOLON
    { $$ = $1; }

var_list: var_list T_COMMA T_ID
    { 
        TypedSymbolListAST *tlist = (TypedSymbolListAST *)$1; 
        tlist->new_sym(*$3); 
        $$ = tlist;
        delete $3;
    }
    | type T_ID
    { $$ = new TypedSymbolListAST(*$2, (decafType)$1); delete $2; }
    ;

statement_list: statement statement_list
    { decafStmtList *slist = (decafStmtList *)$2; slist->push_front($1); $$ = slist; }
    | /* empty */ 
    { decafStmtList *slist = new decafStmtList(); $$ = slist; }
    ;
  
statement: assign T_SEMICOLON
    { $$ = $1; }

assign: T_ID T_ASSIGN expr
    { $$ = new AssignVarAST(*$1, $3); delete $1; }
    ;

rvalue: T_ID
    { $$ = new VariableExprAST(*$1); delete $1; }
    ;

expr:
    rvalue 
  { $$ = $1;}
  | constant
	{ $$ = $1; }
	| expr T_PLUS expr 
	{ $$ = new BinaryExprAST(T_PLUS, $1, $3); }
	| expr T_MINUS expr 
	{  $$ = new BinaryExprAST(T_MINUS, $1, $3); }
	| expr T_MULT expr 
	{  $$ = new BinaryExprAST(T_MULT, $1, $3); }
	| expr T_DIV expr 
	{  $$ = new BinaryExprAST(T_DIV, $1, $3); }
	| expr T_LEFTSHIFT expr 
	{  $$ = new BinaryExprAST(T_LEFTSHIFT, $1, $3); }
	| expr T_RIGHTSHIFT expr 
	{  $$ = new BinaryExprAST(T_RIGHTSHIFT, $1, $3); }
	| expr T_MOD expr 
	{  $$ = new BinaryExprAST(T_MOD, $1, $3); }
	| expr T_EQ expr
    	{ $$ = new BinaryExprAST(T_EQ, $1, $3); }
        | expr T_LT expr
    	{ $$ = new BinaryExprAST(T_LT, $1, $3); }
	;

constant: T_INTCONSTANT
    	{ $$ = new NumberExprAST($1); }
	;

%%

// Code Generation

Value *NumberExprAST::Codegen() {
  return ConstantInt::get(getGlobalContext(), APInt(32, Val));
}

Value *BinaryExprAST::Codegen() {
  Value *L = LHS->Codegen();
  Value *R = RHS->Codegen();
  if (L == 0 || R == 0) return 0;
  
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

// Main 

int main() {
  fprintf(stderr, "ready> ");
  // initialize LLVM
  LLVMContext &Context = getGlobalContext();
  // Make the module, which holds all the code.
  TheModule = new Module("module for very simple expressions", Context);
  // parse the input and create the abstract syntax tree
  int retval = yyparse();
  // Print out all of the generated code to stderr
  TheModule->dump();
  return(retval >= 1 ? 1 : 0);
}
