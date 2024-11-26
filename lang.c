#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"

struct expr * new_expr_ptr() {
  struct expr * res = (struct expr *) malloc(sizeof(struct expr));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct cmd * new_cmd_ptr() {
  struct cmd * res = (struct cmd *) malloc(sizeof(struct cmd));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct prog * new_prog_ptr() {
  struct prog * res = (struct prog *) malloc(sizeof(struct prog));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct varlist * new_varlist_ptr() {
  struct varlist * res = (struct varlist *) malloc(sizeof(struct varlist));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct exprlist * new_exprlist_ptr() {
  struct exprlist * res = (struct exprlist *) malloc(sizeof(struct exprlist));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct def * new_def_ptr() {
  struct def * res = (struct def *) malloc(sizeof(struct def));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct expr * TConst(unsigned int value) {
  struct expr * res = new_expr_ptr();
  res -> t = T_CONST;
  res -> d.CONST.value = value;
  return res;
}

struct expr * TVar(char * name) {
  struct expr * res = new_expr_ptr();
  res -> t = T_VAR;
  res -> d.VAR.name = name;
  return res;
}

struct expr * TBinOp(enum BinOpType op, struct expr * left, struct expr * right) {
  struct expr * res = new_expr_ptr();
  res -> t = T_BINOP;
  res -> d.BINOP.op = op;
  res -> d.BINOP.left = left;
  res -> d.BINOP.right = right;
  return res;
}

struct expr * TUnOp(enum UnOpType op, struct expr * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_UNOP;
  res -> d.UNOP.op = op;
  res -> d.UNOP.arg = arg;
  return res;
}

struct expr * TDeref(struct expr * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_DEREF;
  res -> d.DEREF.arg = arg;
  return res;
}

struct expr * TMalloc(struct expr * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_MALLOC;
  res -> d.MALLOC.arg = arg;
  return res;
}

struct expr * TReadInt() {
  struct expr * res = new_expr_ptr();
  res -> t = T_RI;
  return res;
}

struct expr * TReadChar() {
  struct expr * res = new_expr_ptr();
  res -> t = T_RC;
  return res;
}

struct expr * TCallE(char * name, struct exprlist * args) {
  struct expr * res = new_expr_ptr();
  res -> t = T_CALL_E;
  res -> d.CALL_E.name = name;
  res -> d.CALL_E.args = args;
  return res;
}

struct expr * TCallENoArgs(char * name) {
  struct expr * res = new_expr_ptr();
  res -> t = T_CALL_E_NO_ARGS;
  res -> d.CALL_E_NO_ARGS.name = name;
  return res;
}

struct cmd * TDecl(char * name) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_DECL;
  res -> d.DECL.name = name;
  return res;
}

struct cmd * TAsgn(struct expr * left, struct expr * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_ASGN;
  res -> d.ASGN.left = left;
  res -> d.ASGN.right = right;
  return res;
}

struct cmd * TSeq(struct cmd * left, struct cmd * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_SEQ;
  res -> d.SEQ.left = left;
  res -> d.SEQ.right = right;
  return res;
}

struct cmd * TIf(struct expr * cond, struct cmd * left, struct cmd * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_IF;
  res -> d.IF.cond = cond;
  res -> d.IF.left = left;
  res -> d.IF.right = right;
  return res;
}

struct cmd * TWhile(struct expr * cond, struct cmd * body) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_WHILE;
  res -> d.WHILE.cond = cond;
  res -> d.WHILE.body = body;
  return res;
}

struct cmd * TWriteInt(struct expr * arg) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_WI;
  res -> d.WI.arg = arg;
  return res;
}

struct cmd * TWriteChar(struct expr * arg) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_WC;
  res -> d.WC.arg = arg;
  return res;
}

struct cmd * TCallC(char * name, struct exprlist * args) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_CALL_C;
  res -> d.CALL_C.name = name;
  res -> d.CALL_C.args = args;
  return res;
}

struct cmd * TCallCNoArgs(char * name) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_CALL_C_NO_ARGS;
  res -> d.CALL_C_NO_ARGS.name = name;
  return res;
}

struct cmd * TReturn(struct expr * retval) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_RET;
  res -> d.RET.retval = retval;
  return res;
}

struct prog * TProgWithoutDef(struct cmd * body) {
  struct prog * res = new_prog_ptr();
  res -> t = T_PROG_WITHOUT_DEF;
  res -> d.PROG_WITHOUT_DEF.body = body;
  return res;
}

struct prog * TProgWithDef(struct def * defs, struct cmd * body) {
  struct prog * res = new_prog_ptr();
  res -> t = T_PROG_WITH_DEF;
  res -> d.PROG_WITH_DEF.defs = defs;
  res -> d.PROG_WITH_DEF.body = body;
  return res;
}

struct varlist * TSingleVar(char * arg) {
  struct varlist * res = new_varlist_ptr();
  res -> t = T_SINGLE_VAR;
  res -> d.SINGLE_VAR.arg = arg;
  return res;
}

struct varlist * TMultiVar(char * arg, struct varlist * next) {
  struct varlist * res = new_varlist_ptr();
  res -> t = T_MULTI_VAR;
  res -> d.MULTI_VAR.arg = arg;
  res -> d.MULTI_VAR.next = next;
  return res;
}

struct exprlist * TSingleExpr(struct expr * arg) {
  struct exprlist * res = new_exprlist_ptr();
  res -> t = T_SINGLE_EXPR;
  res -> d.SINGLE_EXPR.arg = arg;
  return res;
}

struct exprlist * TMultiExpr(struct expr * arg, struct exprlist * next) {
  struct exprlist * res = new_exprlist_ptr();
  res -> t = T_MULTI_EXPR;
  res -> d.MULTI_EXPR.arg = arg;
  res -> d.MULTI_EXPR.next = next;
  return res;
}


struct def * TFunc(char * name, struct varlist * args, struct cmd * body) {
  struct def * res = new_def_ptr();
  res -> t = T_FUNC;
  res -> d.FUNC.name = name;
  res -> d.FUNC.args = args;
  res -> d.FUNC.body = body;
  return res;
}

struct def * TFuncNoArgs(char * name, struct cmd * body) {
  struct def * res = new_def_ptr();
  res -> t = T_FUNC_NO_ARGS;
  res -> d.FUNC_NO_ARGS.name = name;
  res -> d.FUNC_NO_ARGS.body = body;
  return res;
}

struct def * TProc(char * name, struct varlist * args, struct cmd * body) {
  struct def * res = new_def_ptr();
  res -> t = T_PROC;
  res -> d.PROC.name = name;
  res -> d.PROC.args = args;
  res -> d.PROC.body = body;
  return res;
}

struct def * TProcNoArgs(char * name, struct cmd * body) {
  struct def * res = new_def_ptr();
  res -> t = T_PROC_NO_ARGS;
  res -> d.PROC_NO_ARGS.name = name;
  res -> d.PROC_NO_ARGS.body = body;
  return res;
}

struct def * TExpr(char * name, struct varlist * args, struct expr * val) {
  struct def * res = new_def_ptr();
  res -> t = T_EXPR;
  res -> d.EXPR.name = name;
  res -> d.EXPR.args = args;
  res -> d.EXPR.val = val;
  return res;
}

struct def * TExprNoArgs(char * name, struct expr * val) {
  struct def * res = new_def_ptr();
  res -> t = T_EXPR_NO_ARGS;
  res -> d.EXPR_NO_ARGS.name = name;
  res -> d.EXPR_NO_ARGS.val = val;
  return res;
}

struct def * TCmd(char * name, struct varlist * args, struct cmd * body) {
  struct def * res = new_def_ptr();
  res -> t = T_CMD;
  res -> d.CMD.name = name;
  res -> d.CMD.args = args;
  res -> d.CMD.body = body;
  return res;
}

struct def * TCmdNoArgs(char * name, struct cmd * body) {
  struct def * res = new_def_ptr();
  res -> t = T_CMD_NO_ARGS;
  res -> d.CMD_NO_ARGS.name = name;
  res -> d.CMD_NO_ARGS.body = body;
  return res;
}

struct def * TSeqDef(struct def * left, struct def * right) {
  struct def * res = new_def_ptr();
  res -> t = T_SEQ_DEF;
  res -> d.SEQ_DEF.left = left;
  res -> d.SEQ_DEF.right = right;
  return res;
}


void print_prog(struct prog * p) {
  switch (p -> t) {
  case T_PROG_WITHOUT_DEF:
    printf("PROG_WITHOUT_DEF(\n");
    print_cmd(p -> d.PROG_WITHOUT_DEF.body);
    printf(")\n");
    break;
  case T_PROG_WITH_DEF:
    printf("PROG_WITH_DEF(\n");
    print_def(p -> d.PROG_WITH_DEF.defs);
    printf(",\n");
    print_cmd(p -> d.PROG_WITH_DEF.body);
    printf(")\n");
    break;
  }
}

void print_def(struct def * d_) {
  switch (d_ -> t) {
  case T_FUNC:
    printf("FUNC(%s,", d_ -> d.FUNC.name);
    print_varlist(d_ -> d.FUNC.args);
    printf(",\n");
    print_cmd(d_ -> d.FUNC.body);
    printf(")\n");
    break;
  case T_FUNC_NO_ARGS:
    printf("FUNC_NO_ARGS(%s,\n", d_ -> d.FUNC_NO_ARGS.name);
    print_cmd(d_ -> d.FUNC_NO_ARGS.body);
    printf(")\n");
    break;
  case T_PROC:
    printf("PROC(%s,", d_ -> d.PROC.name);
    print_varlist(d_ -> d.PROC.args);
    printf(",\n");
    print_cmd(d_ -> d.PROC.body);
    printf(")\n");
    break;
  case T_PROC_NO_ARGS:
    printf("PROC_NO_ARGS(%s,", d_ -> d.PROC_NO_ARGS.name);
    print_cmd(d_ -> d.PROC_NO_ARGS.body);
    printf(")\n");
    break;
  case T_EXPR:
    printf("EXPR(%s,", d_ -> d.EXPR.name);
    print_varlist(d_ -> d.EXPR.args);
    printf(",\n");
    print_expr(d_ -> d.EXPR.val);
    printf(")\n");
    break;
  case T_EXPR_NO_ARGS:
    printf("EXPR_NO_ARGS(%s,", d_ -> d.EXPR_NO_ARGS.name);
    print_expr(d_ -> d.EXPR_NO_ARGS.val);
    printf(")\n");
    break;
  case T_CMD:
    printf("CMD(%s,", d_ -> d.CMD.name);
    print_varlist(d_ -> d.CMD.args);
    printf(",\n");
    print_cmd(d_ -> d.CMD.body);
    printf(")\n");
    break;
  case T_CMD_NO_ARGS:
    printf("CMD_NO_ARGS(%s,", d_ -> d.CMD_NO_ARGS.name);
    print_cmd(d_ -> d.CMD_NO_ARGS.body);
    printf(")\n");
    break;
  case T_SEQ_DEF:
    printf("SEQ_DEF(\n");
    print_def(d_ -> d.SEQ_DEF.left);
    printf(",\n");
    print_def(d_ -> d.SEQ_DEF.right);
    printf(")\n");
    break;
  }
}

void print_varlist(struct varlist * vl) {
  switch (vl -> t) {
  case T_SINGLE_VAR:
    printf("SINGLE_VAR(%s)", vl -> d.SINGLE_VAR.arg);
    break;
  case T_MULTI_VAR:
    printf("MULTI_VAR(%s,", vl -> d.MULTI_VAR.arg);
    print_varlist(vl -> d.MULTI_VAR.next);
    printf(")");
    break;
  }
}

void print_exprlist(struct exprlist * el) {
  switch (el -> t) {
  case T_SINGLE_EXPR:
    printf("SINGLE_EXPR(");
    print_expr(el -> d.SINGLE_EXPR.arg);
    printf(")");
    break;
  case T_MULTI_EXPR:
    printf("MULTI_EXPR(");
    print_expr(el -> d.MULTI_EXPR.arg);
    printf(",");
    print_exprlist(el -> d.MULTI_EXPR.next);
    printf(")");
    break;
  }
}

void print_binop(enum BinOpType op) {
  switch (op) {
  case T_PLUS:
    printf("PLUS");
    break;
  case T_MINUS:
    printf("MINUS");
    break;
  case T_MUL:
    printf("MUL");
    break;
  case T_DIV:
    printf("DIV");
    break;
  case T_MOD:
    printf("MOD");
    break;
  case T_LT:
    printf("LT");
    break;
  case T_GT:
    printf("GT");
    break;
  case T_LE:
    printf("LE");
    break;
  case T_GE:
    printf("GE");
    break;
  case T_EQ:
    printf("EQ");
    break;
  case T_NE:
    printf("NE");
    break;
  case T_AND:
    printf("AND");
    break;
  case T_OR:
    printf("OR");
    break;
  }
}

void print_unop(enum UnOpType op) {
  switch (op) {
  case T_UMINUS:
    printf("UMINUS");
    break;
  case T_NOT:
    printf("NOT");
    break;
  }
}

void print_expr(struct expr * e) {
  switch (e -> t) {
  case T_CONST:
    printf("CONST(%d)", e -> d.CONST.value);
    break;
  case T_VAR:
    printf("VAR(%s)", e -> d.VAR.name);
    break;
  case T_BINOP:
    print_binop(e -> d.BINOP.op);
    printf("(");
    print_expr(e -> d.BINOP.left);
    printf(",");
    print_expr(e -> d.BINOP.right);
    printf(")");
    break;
  case T_UNOP:
    print_unop(e -> d.UNOP.op);
    printf("(");
    print_expr(e -> d.UNOP.arg);
    printf(")");
    break;
  case T_DEREF:
    printf("DEREF(");
    print_expr(e -> d.DEREF.arg);
    printf(")");
    break;
  case T_MALLOC:
    printf("MALLOC(");
    print_expr(e -> d.MALLOC.arg);
    printf(")");
    break;
  case T_RI:
    printf("READ_INT()");
    break;
  case T_RC:
    printf("READ_CHAR()");
    break;
  case T_CALL_E:
    printf("CALL_E(%s,", e -> d.CALL_E.name);
    print_exprlist(e -> d.CALL_E.args);
    printf(")");
    break;
  case T_CALL_E_NO_ARGS:
    printf("CALL_E_NO_ARGS(%s)", e -> d.CALL_E_NO_ARGS.name);
    break;
  }
}

void print_cmd(struct cmd * c) {
  switch (c -> t) {
  case T_DECL:
    printf("DECL(%s)", c -> d.DECL.name);
    break;
  case T_ASGN:
    printf("ASGN(");
    print_expr(c -> d.ASGN.left);
    printf(",");
    print_expr(c -> d.ASGN.right);
    printf(")");
    break;
  case T_SEQ:
    printf("SEQ(");
    print_cmd(c -> d.SEQ.left);
    printf(",");
    print_cmd(c -> d.SEQ.right);
    printf(")");
    break;
  case T_IF:
    printf("IF(");
    print_expr(c -> d.IF.cond);
    printf(",");
    print_cmd(c -> d.IF.left);
    printf(",");
    print_cmd(c -> d.IF.right);
    printf(")");
    break;
  case T_WHILE:
    printf("WHILE(");
    print_expr(c -> d.WHILE.cond);
    printf(",");
    print_cmd(c -> d.WHILE.body);
    printf(")");
    break;
  case T_WI:
    printf("WRITE_INT(");
    print_expr(c -> d.WI.arg);
    printf(")");
    break;
  case T_WC:
    printf("WRITE_CHAR(");
    print_expr(c -> d.WC.arg);
    printf(")");
    break;
  case T_CALL_C:
    printf("CALL_C(%s,", c -> d.CALL_C.name);
    print_exprlist(c -> d.CALL_C.args);
    printf(")");
    break;
  case T_CALL_C_NO_ARGS:
    printf("CALL_C_NO_ARGS(%s)", c -> d.CALL_C_NO_ARGS.name);
    break;
  case T_RET:
    printf("RET(");
    print_expr(c -> d.RET.retval);
    printf(")");
    break;
  }
}

unsigned int build_nat(char * c, int len) {
  int s = 0, i = 0;
  for (i = 0; i < len; ++i) {
    if (s > 429496729) {
      printf("We cannot handle natural numbers greater than 4294967295.\n");
      exit(0);
    }
    if (s == 429496729 && c[i] > '5') {
      printf("We cannot handle natural numbers greater than 4294967295.\n");
      exit(0);
    }
    s = s * 10 + (c[i] - '0');
  }
  return s;
}

char * new_str(char * str, int len) {
  char * res = (char *) malloc(sizeof(char) * (len + 1));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  strcpy(res, str);
  return res;
}

