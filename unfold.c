#include "lang.h"
#include "unfold.h"

int definition_not_found = 0, call_type_error = 0;

struct macro_node * find_macro(struct macro_list *macros, char *name) {
    struct macro_node *node = macros->head;
    while(node != NULL) {
        if(strcmp(node->def->d.EXPR.name, name) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

struct varlist * empty_varlist() {
    struct varlist * res = NULL;
    return res;
}
struct exprlist * empty_exprlist() {
    struct exprlist * res = NULL;
    return res;
}


int find_var_in_varlist(struct varlist *v_list, char *name) {
    // 1 based index, return -1 if not found
    if(v_list == NULL) return -1;
    switch (v_list->t) {
        case T_SINGLE_VAR:
            if(strcmp(v_list->d.SINGLE_VAR.arg, name) == 0) return 1;
            else return -1;
            break;
        case T_MULTI_VAR:
            if(strcmp(v_list->d.MULTI_VAR.arg, name) == 0) return 1;
            else {
                int res = find_var_in_varlist(v_list->d.MULTI_VAR.next, name);
                if(res == -1) return -1;
                else return res + 1;
            }
            break;
        default:
            break;
    }
    return -1;
}
struct expr * find_expr_by_index(struct exprlist *e_list, int ind) {
    if(e_list == NULL) return NULL;
    switch (e_list->t) {
        case T_SINGLE_EXPR:
            if(ind == 1) return e_list->d.SINGLE_EXPR.arg;
            else return NULL;
            break;
        case T_MULTI_EXPR:
            if(ind == 1) return e_list->d.MULTI_EXPR.arg;
            else {
                return find_expr_by_index(e_list->d.MULTI_EXPR.next, ind - 1);
            }
            break;
        default:
            break;
    }
    return NULL;
}
struct exprlist * unfold_expr_list(struct exprlist *e_list, struct macro_list * macros, struct varlist *v_list, struct exprlist *e_list2) {
    if(e_list == NULL) return NULL;
    if(e_list->t == T_SINGLE_EXPR) {
        struct exprlist *res = malloc(sizeof(struct exprlist));
        res->t = T_SINGLE_EXPR;
        res->d.SINGLE_EXPR.arg = unfold_expr(e_list->d.SINGLE_EXPR.arg, macros, v_list, e_list2);
        return res;
    }
    else {
        struct exprlist *res = malloc(sizeof(struct exprlist));
        res->t = T_MULTI_EXPR;
        res->d.MULTI_EXPR.arg = unfold_expr(e_list->d.MULTI_EXPR.arg, macros, v_list, e_list2);
        res->d.MULTI_EXPR.next = unfold_expr_list(e_list->d.MULTI_EXPR.next, macros, v_list, e_list2);
        return res;
    }
}
struct expr * unfold_expr(struct expr *e, struct macro_list *macros, struct varlist *v_list, struct exprlist *e_list) {
    if(e == NULL) return NULL;
    struct expr *res = malloc(sizeof(struct expr));
    switch (e->t) {
        case T_CONST:
            res->t = T_CONST;
            res->d.CONST.value = e->d.CONST.value;
            break;
        case T_VAR:
            {
            int v_index = find_var_in_varlist(v_list, e->d.VAR.name);
            if(v_index == -1) {
                res->t = T_VAR;
                res->d.VAR.name = e->d.VAR.name;
            }
            else if (v_index > 0 && e_list == NULL) {
                res->t = T_VAR;
                res->d.VAR.name = e->d.VAR.name;
            }
            else res = find_expr_by_index(e_list, v_index);
            break;
            }
        case T_BINOP:
            res->t = T_BINOP;
            res->d.BINOP.op = e->d.BINOP.op;
            res->d.BINOP.left = unfold_expr(e->d.BINOP.left, macros, v_list, e_list);
            res->d.BINOP.right = unfold_expr(e->d.BINOP.right, macros, v_list, e_list);
            break;
        case T_UNOP:
            res->t = T_UNOP;
            res->d.UNOP.op = e->d.UNOP.op;
            res->d.UNOP.arg = unfold_expr(e->d.UNOP.arg, macros, v_list, e_list);
            break;
        case T_DEREF:
            res->t = T_DEREF;
            res->d.DEREF.arg = unfold_expr(e->d.DEREF.arg, macros, v_list, e_list);
            break;
        case T_MALLOC:
            res->t = T_MALLOC;
            res->d.MALLOC.arg = unfold_expr(e->d.MALLOC.arg, macros, v_list, e_list);
            break;
        case T_RI:
            res->t = T_RI;
            break;
        case T_RC:
            res->t = T_RC;
            break;
        case T_CALL_E:
            {
            struct macro_node *node = find_macro(macros, e->d.CALL_E.name);
            if(node == NULL) {
                definition_not_found = 1;
            }
            else if(node->def->t == T_EXPR) {
                struct exprlist *args = unfold_expr_list(e->d.CALL_E.args, macros, v_list, e_list);
                res = unfold_expr(node->def->d.EXPR.val, macros, node->def->d.EXPR.args, args);
            } 
            else if(node->def->t == T_FUNC) {
                res -> t = T_CALL_E;
                res -> d.CALL_E.name = node->def->d.FUNC.name;
                res -> d.CALL_E.args = unfold_expr_list(e->d.CALL_E.args, macros, v_list, e_list);
                // res -> d.CALL_E.args = node->def->d.FUNC.args; 
            }
            else {
                call_type_error = 1;
            }
            break;
            }
        case T_CALL_E_NO_ARGS:
            {
            struct macro_node *node2 = find_macro(macros, e->d.CALL_E_NO_ARGS.name);
            if(node2 == NULL) {
                definition_not_found = 1;
            }
            else if(node2->def->t == T_EXPR_NO_ARGS) {
                res = unfold_expr(node2->def->d.EXPR_NO_ARGS.val, macros, empty_varlist(), empty_exprlist());
            } 
            else if(node2->def->t == T_FUNC_NO_ARGS) {
                res -> t = T_CALL_E_NO_ARGS;
                res -> d.CALL_E_NO_ARGS.name = node2->def->d.FUNC_NO_ARGS.name;
            }
            else {
                call_type_error = 1;
            }
            break;
            }
        default:
            break;
    }
    return res;
}
struct cmd * unfold_cmd(struct cmd *body, struct macro_list *macros, struct varlist *v_list, struct exprlist *e_list) {
    if(body == NULL) return NULL;
    struct cmd *res = malloc(sizeof(struct cmd));
    switch (body->t) {
        case T_DECL:
            res->t = T_DECL;
            res->d.DECL.name = body->d.DECL.name;
            break;
        case T_ASGN:
            res->t = T_ASGN;
            res->d.ASGN.left = unfold_expr(body->d.ASGN.left, macros, v_list, e_list);
            res->d.ASGN.right = unfold_expr(body->d.ASGN.right, macros, v_list, e_list);
            break;
        case T_SEQ:
            res->t = T_SEQ;
            res->d.SEQ.left = unfold_cmd(body->d.SEQ.left, macros, v_list, e_list);
            res->d.SEQ.right = unfold_cmd(body->d.SEQ.right, macros, v_list, e_list);
            break;
        case T_IF:
            res->t = T_IF;
            res->d.IF.cond = unfold_expr(body->d.IF.cond, macros, v_list, e_list);
            res->d.IF.left = unfold_cmd(body->d.IF.left, macros, v_list, e_list);
            res->d.IF.right = unfold_cmd(body->d.IF.right, macros, v_list, e_list);
            break;
        case T_WHILE:
            res->t = T_WHILE;
            res->d.WHILE.cond = unfold_expr(body->d.WHILE.cond, macros, v_list, e_list);
            res->d.WHILE.body = unfold_cmd(body->d.WHILE.body, macros, v_list, e_list);
            break;
        case T_WI:
            res->t = T_WI;
            res->d.WI.arg = unfold_expr(body->d.WI.arg, macros, v_list, e_list);
            break;
        case T_WC:
            res->t = T_WC;
            res->d.WC.arg = unfold_expr(body->d.WC.arg, macros, v_list, e_list);
            break;
        case T_CALL_C:
            {
            struct macro_node *node = find_macro(macros, body->d.CALL_C.name);
            if(node == NULL) {
                definition_not_found = 1;
            }
            else if(node->def->t == T_CMD) {
                struct exprlist *args = unfold_expr_list(body->d.CALL_C.args, macros, v_list, e_list);
                res = unfold_cmd(node->def->d.CMD.body, macros, node->def->d.CMD.args, args);
            } 
            else if(node->def->t == T_PROC) {
                res -> t = T_CALL_C;
                res -> d.CALL_C.name = node->def->d.PROC.name;
                res -> d.CALL_C.args = unfold_expr_list(body->d.CALL_C.args, macros, v_list, e_list);
                // res -> d.CALL_C.args = node->def->d.PROC.args; 
            }
            else {
                call_type_error = 1;
            }
            break;
            }
        case T_CALL_C_NO_ARGS:
            {
            struct macro_node *node2 = find_macro(macros, body->d.CALL_C_NO_ARGS.name);
            if(node2 == NULL) {
                definition_not_found = 1;
            }
            else if(node2->def->t == T_CMD_NO_ARGS) {
                res = unfold_cmd(node2->def->d.CMD_NO_ARGS.body, macros, empty_varlist(), empty_exprlist());
            } 
            else if(node2->def->t == T_PROC_NO_ARGS) {
                res -> t = T_CALL_C_NO_ARGS;
                res -> d.CALL_C_NO_ARGS.name = node2->def->d.PROC_NO_ARGS.name;
            }
            else {
                call_type_error = 1;
            }
            break;
            }
        case T_RET:
            res->t = T_RET;
            res->d.RET.retval = unfold_expr(body->d.RET.retval, macros, v_list, e_list);
            break;
        default:
            break;
    }
    return res;
}


void add_macro(struct macro_list *macros, struct def *def) {
    struct macro_node *node = malloc(sizeof(struct macro_node));
    node->def = malloc(sizeof(struct def));
    node->def->t = def->t;
    switch (def->t) {
        case T_FUNC:
            node->def->d.FUNC.name = def->d.FUNC.name;
            node->def->d.FUNC.args = def->d.FUNC.args;
            break;
        case T_PROC:
            node->def->d.PROC.name = def->d.PROC.name;
            node->def->d.PROC.args = def->d.PROC.args; 
            break;
        case T_FUNC_NO_ARGS:
            node->def->d.FUNC_NO_ARGS.name = def->d.FUNC_NO_ARGS.name;
            break;
        case T_PROC_NO_ARGS:    
            node->def->d.PROC_NO_ARGS.name = def->d.PROC_NO_ARGS.name;
            break;
        case T_EXPR:
            node->def->d.EXPR.name = def->d.EXPR.name;
            node->def->d.EXPR.args = def->d.EXPR.args; 
            node->def->d.EXPR.val = unfold_expr(def->d.EXPR.val, macros, node->def->d.EXPR.args, empty_exprlist());
            break;
        case T_EXPR_NO_ARGS:
            node->def->d.EXPR_NO_ARGS.name = def->d.EXPR_NO_ARGS.name;
            node->def->d.EXPR_NO_ARGS.val = unfold_expr(def->d.EXPR_NO_ARGS.val, macros, empty_varlist(), empty_exprlist());
            break;
        case T_CMD:
            node->def->d.CMD.name = def->d.CMD.name;
            node->def->d.CMD.args = def->d.CMD.args;
            node->def->d.CMD.body = unfold_cmd(def->d.CMD.body, macros, node->def->d.CMD.args, empty_exprlist());
            break;
        case T_CMD_NO_ARGS:
            node->def->d.CMD_NO_ARGS.name = def->d.CMD_NO_ARGS.name;
            node->def->d.CMD_NO_ARGS.body = unfold_cmd(def->d.CMD_NO_ARGS.body, macros, empty_varlist(), empty_exprlist());
            break;
        default:
            break;
    }
    node->next = NULL;
    if(macros->head == NULL) {
        macros->head = node;
        macros->tail = node;
    } else {
        macros->tail->next = node;
        macros->tail = node;
    }
    switch(def->t) {
        case T_FUNC:
            node->def->d.FUNC.body = unfold_cmd(def->d.FUNC.body, macros, node->def->d.FUNC.args, empty_exprlist());
            break;
        case T_PROC:
            node->def->d.PROC.body = unfold_cmd(def->d.PROC.body, macros, node->def->d.PROC.args, empty_exprlist());
            break;
        case T_FUNC_NO_ARGS:
            node->def->d.FUNC_NO_ARGS.body = unfold_cmd(def->d.FUNC_NO_ARGS.body, macros, empty_varlist(), empty_exprlist());
            break;
        case T_PROC_NO_ARGS:
            node->def->d.PROC_NO_ARGS.body = unfold_cmd(def->d.PROC_NO_ARGS.body, macros, empty_varlist(), empty_exprlist());   
            break;
        default:
            break;
    }

    return ;
}

struct def * clean_def(struct def *defs, struct macro_list *macros) {
    if(defs == NULL) return NULL;
    struct def *res = malloc(sizeof(struct def));
    switch (defs->t) {
        case T_SEQ_DEF:
            res->t = T_SEQ_DEF;
            res->d.SEQ_DEF.left = clean_def(defs->d.SEQ_DEF.left, macros);
            res->d.SEQ_DEF.right = clean_def(defs->d.SEQ_DEF.right, macros);
            if(res->d.SEQ_DEF.left == NULL) return res->d.SEQ_DEF.right;
            if(res->d.SEQ_DEF.right == NULL) return res->d.SEQ_DEF.left;
            break;
        case T_FUNC:
        case T_PROC:
        case T_FUNC_NO_ARGS:
        case T_PROC_NO_ARGS:
            {
            struct macro_node *node = find_macro(macros, defs->d.FUNC.name);
            res = node->def;
            break;
            }
        case T_EXPR:
        case T_EXPR_NO_ARGS:
        case T_CMD:
        case T_CMD_NO_ARGS:
            res = NULL;
            break;
        default:
            break;
    }
    return res;
}

void clean_macros(struct def *defs, struct macro_list *macros, struct prog *res) {
    if(defs == NULL) return;

    struct def * new_defs = clean_def(defs, macros);
    if(new_defs != NULL) {
        res->t = T_PROG_WITH_DEF;
        res->d.PROG_WITH_DEF.defs = new_defs;
    }
    else {
        res->t = T_PROG_WITHOUT_DEF;
    }

    return;
}

void record_defs(struct def *defs, struct macro_list *macros) {
    //record all the macros
    if(defs == NULL) return;
    switch (defs->t) {
        case T_SEQ_DEF:
            record_defs(defs->d.SEQ_DEF.left, macros);
            record_defs(defs->d.SEQ_DEF.right, macros);
        break;
        default:
            add_macro(macros, defs); //dircetly expand the macros when adding
        break;
    }
    return;
}

void print_macros_list(struct macro_list *macros) {
    struct macro_node *node = macros->head;
    while(node != NULL) {
        switch (node->def->t) {
            case T_EXPR_NO_ARGS:
                printf("EXPR_NO_ARGS(%s,", node->def->d.EXPR_NO_ARGS.name);
                print_expr(node->def->d.EXPR_NO_ARGS.val);
                printf(")\n");
                break;
            default:
                break;
        }
        node = node->next;
    }
    return;
}

struct prog * conv(struct prog * p) {
    /*
    * to do: accept a program and return a new program with all the macros unfolded
    */

   if(p->t == T_PROG_WITHOUT_DEF) return p;

    struct prog *res = malloc(sizeof(struct prog));

    struct def *defs = p->d.PROG_WITH_DEF.defs;
    struct cmd *body = p->d.PROG_WITH_DEF.body;

    struct macro_list *macros = malloc(sizeof(struct macro_list));
    macros->head = NULL;
    macros->tail = NULL;

    record_defs(defs, macros);

    clean_macros(defs, macros, res);

    if(res->t == T_PROG_WITHOUT_DEF) {
        res->d.PROG_WITHOUT_DEF.body = unfold_cmd(body, macros, empty_varlist(), empty_exprlist());
    }
    else if(res->t == T_PROG_WITH_DEF) {
        res->d.PROG_WITH_DEF.body = unfold_cmd(body, macros, empty_varlist(), empty_exprlist());
    }
    if(definition_not_found) {
        printf("Error: definition not found\n");
        return NULL;
    }
    if(call_type_error) {
        printf("Error: call type error\n");
        return NULL;
    }
    return res;
}
