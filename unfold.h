#ifndef UNFOLD_H_INCLUDED
#define UNFOLD_H_INCLUDED

#include "lang.h"

struct macro_node {
    struct def *def;
    struct macro_node *next;
};
struct macro_list {
    struct macro_node *head;
    struct macro_node *tail;
};
struct prog * conv(struct prog * arg);

struct macro_node * find_macro(struct macro_list *macros, char *name);
struct varlist * empty_varlist();
struct exprlist * empty_exprlist();
int find_var_in_varlist(struct varlist *v_list, char *name);
struct expr * find_expr_by_index(struct exprlist*e_list, int ind);
struct exprlist * unfold_expr_list(struct exprlist *e_list, struct macro_list * macros, struct varlist *v_list, struct exprlist *e_list2);
struct expr * unfold_expr(struct expr *e, struct macro_list *macros, struct varlist *v_list, struct exprlist *e_list);
struct cmd * unfold_cmd(struct cmd *body, struct macro_list *macros, struct varlist *v_list, struct exprlist *e_list);
void add_macro(struct macro_list *macros, struct def *def);
struct def * clean_def(struct def *defs, struct macro_list *macros);
void clean_macros(struct def *defs, struct macro_list *macros, struct prog *res);
void record_defs(struct def *defs, struct macro_list *macros);

#endif // UNFOLD_H_INCLUDED
