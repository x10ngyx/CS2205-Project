#include "lang.h"
#include "unfold.h"

struct var_list {
    char *name;
    struct var_list *next;
};

struct macro_list {
    enum DefType t;
    char *name;
    struct var_list *args;
    struct expr *val;
    struct cmd *body;
    struct macro_list *next;
};

void record_defs(struct def *defs) {
    if(defs == NULL) return;
    switch (defs->t)
    {
    case T_FUNC:

        
        break;
    case T_FUNC_NO_ARGS:
        break;
    case T_PROC:

    }
}

struct prog * conv(struct prog * p) {
    /*
    * to do: accept a program and return a new program with all the macros unfolded
    */
   if(p->t == T_PROG_WITHOUT_DEF) return p;

    struct prog *res = new_prog_ptr();
    res->t = T_PROG_WITHOUT_DEF;

    struct def *defs = p->d.PROG_WITH_DEF.defs;
    struct cmd *body = p->d.PROG_WITH_DEF.body;

    record_defs(defs);

    unfold_macros(body, res);


    return NULL;
}
