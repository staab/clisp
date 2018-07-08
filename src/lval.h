#ifndef lval_h_INCLUDED
#define lval_h_INCLUDED

# include "mpc.h"

typedef struct lval {
  int type;
  long num;
  char* err;
  char* sym;
  int count;
  struct lval** cell;
} lval;

enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR };

lval* lval_num(long num);
lval* lval_err(char* err);
lval* lval_sym(char* sym);
lval* lval_sexpr(void);

/* Read */

lval* lval_read_num(mpc_ast_t* t);
lval* lval_read(mpc_ast_t* t);
lval* lval_add(lval* v, lval* x);

/* Eval */

lval* lval_pop(lval* v, int i);
lval* lval_take(lval* v, int i);
lval* builtin_op(lval* a, char* op);
lval* lval_eval_sexpr(lval* v);
lval* lval_eval(lval* v);

/* Cleanup */

void lval_del(lval* v);

/* Print */

void lval_expr_print(lval* v, char open, char close);
void lval_print(lval* v);
void lval_println(lval* v);

#endif // lval_h_INCLUDED

