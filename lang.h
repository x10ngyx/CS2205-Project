#ifndef LANG_H_INCLUDED
#define LANG_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

// structure of the AST

enum BinOpType {
  T_PLUS,
  T_MINUS,
  T_MUL,
  T_DIV,
  T_MOD,
  T_LT,
  T_GT,
  T_LE,
  T_GE,
  T_EQ,
  T_NE,
  T_AND,
  T_OR
};

enum UnOpType {
  T_UMINUS,
  T_NOT
};

enum ExprType {
  T_CONST = 0,
  T_VAR,
  T_BINOP,
  T_UNOP,
  T_DEREF,
  T_MALLOC,
  T_RI,
  T_RC,
  T_CALL_E, // 
  T_CALL_E_NO_ARGS //
};

enum CmdType {
  T_DECL = 0,
  T_ASGN,
  T_SEQ,
  T_IF,
  T_WHILE,
  T_WI,
  T_WC,
  T_CALL_C, //
  T_CALL_C_NO_ARGS, // 
  T_RET //
};

struct expr {
  enum ExprType t;
  union {
    struct {unsigned int value; } CONST;
    struct {char * name; } VAR;
    struct {enum BinOpType op; struct expr * left; struct expr * right; } BINOP;
    struct {enum UnOpType op; struct expr * arg; } UNOP;
    struct {struct expr * arg; } DEREF;
    struct {struct expr * arg; } MALLOC;
    struct {void * none; } RI;
    struct {void * none; } RC;
    struct {char * name; struct exprlist * args; } CALL_E; //
    struct {char * name; } CALL_E_NO_ARGS; //
  } d;
};

struct cmd {
  enum CmdType t;
  union {
    struct {char * name; } DECL;
    struct {struct expr * left; struct expr * right; } ASGN;
    struct {struct cmd * left; struct cmd * right; } SEQ;
    struct {struct expr * cond; struct cmd * left; struct cmd * right; } IF;
    struct {struct expr * cond; struct cmd * body; } WHILE;
    struct {struct expr * arg; } WI;
    struct {struct expr * arg; } WC;
    struct {char * name; struct exprlist * args; } CALL_C; //
    struct {char * name; } CALL_C_NO_ARGS; //
    struct {struct expr * retval; } RET; //
  } d;
};



// add more structures

enum VarListType {
  T_SINGLE_VAR = 0,
  T_MULTI_VAR
};

struct varlist {
  enum VarListType t;
  union {
    struct {char * arg; } SINGLE_VAR;
    struct {char * arg; struct varlist * next; } MULTI_VAR;
  } d;
};

enum ExprListType {
  T_SINGLE_EXPR= 0,
  T_MULTI_EXPR
};

struct exprlist {
  enum ExprListType t;
  union {
    struct {struct expr * arg; } SINGLE_EXPR;
    struct {struct expr * arg; struct exprlist * next; } MULTI_EXPR;
  } d;
};

enum DefType {
  T_FUNC = 0,
  T_FUNC_NO_ARGS,
  T_PROC,
  T_PROC_NO_ARGS,
  T_EXPR,
  T_EXPR_NO_ARGS,
  T_CMD,
  T_CMD_NO_ARGS,
  T_SEQ_DEF
};

struct def {
  enum DefType t;
  union {
    struct {char * name; struct varlist * args; struct cmd * body; } FUNC;
    struct {char * name; struct cmd * body; } FUNC_NO_ARGS;
    struct {char * name; struct varlist * args; struct cmd * body; } PROC;
    struct {char * name; struct cmd * body; } PROC_NO_ARGS;
    struct {char * name; struct varlist * args; struct expr * val; } EXPR;
    struct {char * name; struct expr * val; } EXPR_NO_ARGS;
    struct {char * name; struct varlist * args; struct cmd * body; } CMD;
    struct {char * name; struct cmd * body; } CMD_NO_ARGS;
    struct {struct def * left; struct def * right; } SEQ_DEF;
  } d;
};

enum ProgType {
  T_PROG_WITHOUT_DEF = 0,
  T_PROG_WITH_DEF
};

struct prog {
  enum ProgType t;
  union {
    struct {struct cmd * body; } PROG_WITHOUT_DEF;
    struct {struct def * defs; struct cmd * body; } PROG_WITH_DEF;
  } d;
};




// helper functions

struct expr * TConst(unsigned int value);
struct expr * TVar(char * name);
struct expr * TBinOp(enum BinOpType op, struct expr * left, struct expr * right);
struct expr * TUnOp(enum UnOpType op, struct expr * arg);
struct expr * TDeref(struct expr * arg);
struct expr * TMalloc(struct expr * arg);
struct expr * TReadInt();
struct expr * TReadChar();
struct expr * TCallE(char * name, struct exprlist * args); //
struct expr * TCallENoArgs(char * name); //

struct cmd * TDecl(char * name);
struct cmd * TAsgn(struct expr * left, struct expr * right);
struct cmd * TSeq(struct cmd * left, struct cmd * right);
struct cmd * TIf(struct expr * cond, struct cmd * left, struct cmd * right);
struct cmd * TWhile(struct expr * cond, struct cmd * body);
struct cmd * TWriteInt(struct expr * arg);
struct cmd * TWriteChar(struct expr * arg);
struct cmd * TCallC(char * name, struct exprlist * args); //
struct cmd * TCallCNoArgs(char * name); //
struct cmd * TReturn(struct expr * arg); //

struct prog * TProgWithoutDef(struct cmd * body);
struct prog * TProgWithDef(struct def * defs, struct cmd * body);

struct varlist * TSingleVar(char * arg);
struct varlist * TMultiVar(char * arg, struct varlist * next);

struct exprlist * TSingleExpr(struct expr * arg);
struct exprlist * TMultiExpr(struct expr * arg, struct exprlist * next);

struct def * TFunc(char * name, struct varlist * args, struct cmd * body);
struct def * TFuncNoArgs(char * name, struct cmd * body);
struct def * TProc(char * name, struct varlist * args, struct cmd * body);
struct def * TProcNoArgs(char * name, struct cmd * body);
struct def * TExpr(char * name, struct varlist * args, struct expr * val);
struct def * TExprNoArgs(char * name, struct expr * val);
struct def * TCmd(char * name, struct varlist * args, struct cmd * body);
struct def * TCmdNoArgs(char * name, struct cmd * body);
struct def * TSeqDef(struct def * left, struct def * right);

void print_binop(enum BinOpType op);
void print_unop(enum UnOpType op);
void print_expr(struct expr * e);
void print_cmd(struct cmd * c);
void print_prog(struct prog * p);
void print_varlist(struct varlist * vl);
void print_exprlist(struct exprlist * el);
void print_def(struct def * d_);

unsigned int build_nat(char * c, int len);
char * new_str(char * str, int len);

#endif // LANG_H_INCLUDED
