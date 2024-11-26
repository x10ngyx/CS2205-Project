%{
	// this part is copied to the beginning of the parser 
	#include <stdio.h>
	#include "lang.h"
	#include "lexer.h"
	void yyerror(char *);
	int yylex(void);
        struct prog * root;
%}

// result of the parser

%union {
unsigned int n;
char * i;
struct expr * e;
struct cmd * c;
struct varlist * vl;
struct exprlist * el;
struct def * d_;
struct prog * p;
void * none;
}

// Terminals
%token <n> TM_NAT
%token <i> TM_IDENT
%token <none> TM_LEFT_BRACE TM_RIGHT_BRACE
%token <none> TM_LEFT_PAREN TM_RIGHT_PAREN
%token <none> TM_SEMICOL
%token <none> TM_MALLOC TM_RI TM_RC TM_WI TM_WC
%token <none> TM_VAR TM_IF TM_THEN TM_ELSE TM_WHILE TM_DO
%token <none> TM_ASGNOP
%token <none> TM_OR
%token <none> TM_AND
%token <none> TM_NOT
%token <none> TM_LT TM_LE TM_GT TM_GE TM_EQ TM_NE
%token <none> TM_PLUS TM_MINUS
%token <none> TM_MUL TM_DIV TM_MOD

// add new terminals
%token <none> TM_DEF_EXPR TM_DEF_CMD
%token <none> TM_DEF_FUNC TM_DEF_PROC
%token <none> TM_RET
%token <none> TM_COMMA


// Nonterminals
%type <p> NT_WHOLE
%type <c> NT_CMD
%type <e> NT_EXPR_2
%type <e> NT_EXPR

%type <p> NT_PROG
%type <d_> NT_DEF
%type <vl> NT_VAR_LIST
%type <el> NT_EXPR_LIST

// Priority
%nonassoc TM_ASGNOP
%left TM_OR
%left TM_AND
%left TM_LT TM_LE TM_GT TM_GE TM_EQ TM_NE
%left TM_PLUS TM_MINUS
%left TM_MUL TM_DIV TM_MOD
%left TM_NOT
%left TM_LEFT_PAREN TM_RIGHT_PAREN
%right TM_SEMICOL

// rules

%%

NT_WHOLE:
  NT_PROG
  {
    $$ = ($1);
    root = $$;
  }
;

NT_PROG:
  NT_CMD
  {
    $$ = (TProgWithoutDef($1));
  }
| NT_DEF TM_SEMICOL NT_CMD
  {
    $$ = (TProgWithDef($1,$3));
  }
;

NT_CMD:
  TM_VAR TM_IDENT
  {
    $$ = (TDecl($2));
  }
| NT_EXPR TM_ASGNOP NT_EXPR
  {
    $$ = (TAsgn($1,$3));
  }
| NT_CMD TM_SEMICOL NT_CMD
  {
    $$ = (TSeq($1,$3));
  }
| TM_IF NT_EXPR TM_THEN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE TM_ELSE TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TIf($2,$5,$9));
  }
| TM_WHILE NT_EXPR TM_DO TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TWhile($2,$5));
  }
| TM_WI TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
  {
    $$ = (TWriteInt($3));
  }
| TM_WC TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
  {
    $$ = (TWriteChar($3));
  }

| TM_IDENT TM_LEFT_PAREN NT_EXPR_LIST TM_RIGHT_PAREN
  {
    $$ = (TCallC($1,$3));
  }

| TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN
  {
    $$ = (TCallCNoArgs($1));
  }

| TM_RET NT_EXPR
  {
    $$ = (TReturn($2));
  }
;

NT_EXPR_2:
  TM_NAT
  {
    $$ = (TConst($1));
  }
| TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
  {
    $$ = ($2);
  }
| TM_IDENT
  {
    $$ = (TVar($1));
  }
| TM_RI TM_LEFT_PAREN TM_RIGHT_PAREN
  {
    $$ = (TReadInt());
  }
| TM_RC TM_LEFT_PAREN TM_RIGHT_PAREN
  {
    $$ = (TReadChar());
  }
| TM_MALLOC TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
  {
    $$ = (TMalloc($3));
  }

| TM_IDENT TM_LEFT_PAREN NT_EXPR_LIST TM_RIGHT_PAREN
  {
    $$ = (TCallE($1,$3));
  }
| TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN
  {
    $$ = (TCallENoArgs($1));
  }

| TM_NOT NT_EXPR_2
  {
    $$ = (TUnOp(T_NOT,$2));
  }
| TM_MINUS NT_EXPR_2
  {
    $$ = (TUnOp(T_UMINUS,$2));
  }
| TM_MUL NT_EXPR_2
  {
    $$ = (TDeref($2));
  }
;

NT_EXPR:
  NT_EXPR_2
  {
    $$ = ($1);
  }
| NT_EXPR TM_MUL NT_EXPR
  {
    $$ = (TBinOp(T_MUL,$1,$3));
  }
| NT_EXPR TM_PLUS NT_EXPR
  {
    $$ = (TBinOp(T_PLUS,$1,$3));
  }
| NT_EXPR TM_MINUS NT_EXPR
  {
    $$ = (TBinOp(T_MINUS,$1,$3));
  }
| NT_EXPR TM_DIV NT_EXPR
  {
    $$ = (TBinOp(T_DIV,$1,$3));
  }
| NT_EXPR TM_MOD NT_EXPR
  {
    $$ = (TBinOp(T_MOD,$1,$3));
  }
| NT_EXPR TM_LT NT_EXPR
  {
    $$ = (TBinOp(T_LT,$1,$3));
  }
| NT_EXPR TM_GT NT_EXPR
  {
    $$ = (TBinOp(T_GT,$1,$3));
  }
| NT_EXPR TM_LE NT_EXPR
  {
    $$ = (TBinOp(T_LE,$1,$3));
  }
| NT_EXPR TM_GE NT_EXPR
  {
    $$ = (TBinOp(T_GE,$1,$3));
  }
| NT_EXPR TM_EQ NT_EXPR
  {
    $$ = (TBinOp(T_EQ,$1,$3));
  }
| NT_EXPR TM_NE NT_EXPR
  {
    $$ = (TBinOp(T_NE,$1,$3));
  }
| NT_EXPR TM_AND NT_EXPR
  {
    $$ = (TBinOp(T_AND,$1,$3));
  }
| NT_EXPR TM_OR NT_EXPR
  {
    $$ = (TBinOp(T_OR,$1,$3));
  }
;

NT_DEF:
  TM_DEF_EXPR TM_IDENT TM_LEFT_PAREN NT_VAR_LIST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TExpr($2,$4,$7));
  }
| TM_DEF_EXPR TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TExprNoArgs($2,$6));
  }
| TM_DEF_CMD TM_IDENT TM_LEFT_PAREN NT_VAR_LIST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TCmd($2,$4,$7));
  }
| TM_DEF_CMD TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TCmdNoArgs($2,$6));
  }
| TM_DEF_FUNC TM_IDENT TM_LEFT_PAREN NT_VAR_LIST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TFunc($2,$4,$7));
  }
| TM_DEF_FUNC TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TFuncNoArgs($2,$6));
  }
| TM_DEF_PROC TM_IDENT TM_LEFT_PAREN NT_VAR_LIST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TProc($2,$4,$7));
  }
| TM_DEF_PROC TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = (TProcNoArgs($2,$6));
  }
| NT_DEF TM_SEMICOL NT_DEF
  {
    $$ = (TSeqDef($1,$3));
  }
;


NT_VAR_LIST:
  TM_IDENT
  {
    $$ = (TSingleVar($1));
  }
| TM_IDENT TM_COMMA NT_VAR_LIST
  {
    $$ = (TMultiVar($1,$3));
  }
;

NT_EXPR_LIST:
  NT_EXPR
  {
    $$ = (TSingleExpr($1));
  }
| NT_EXPR TM_COMMA NT_EXPR_LIST
  {
    $$ = (TMultiExpr($1,$3));
  }


%%

void yyerror(char* s)
{
    fprintf(stderr , "%s\n",s);
}
