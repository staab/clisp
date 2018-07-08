# include <stdio.h>
# include <stdlib.h>

# include <editline/readline.h>

# include "mpc.h"
# include "lval.h"

int number_of_nodes(mpc_ast_t* t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 1;

    for (int i = 0; i < t->children_num; i++) {
      total += number_of_nodes(t->children[i]);
    }

    return total;
  }

  return 0;
}

lval eval_op(lval x, char* op, lval y) {
  if (x.type == LVAL_ERR) { return x; }
  if (y.type == LVAL_ERR) { return y; }

  if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
  if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
  if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
  if (strcmp(op, "/") == 0) {
    return y.num == 0
      ? lval_err(LERR_DIV_ZERO)
      : lval_num(x.num / y.num);
  }

  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t) {
  if (strstr(t->tag, "number")) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);

    return errno == ERANGE ? lval_err(LERR_BAD_NUM) : lval_num(x);
  }

  char* op = t->children[1]->contents;

  lval x = eval(t->children[2]);

  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}

int main(int argc, char** argv) {
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* Clisp = mpc_new("clisp");

  mpca_lang(MPCA_LANG_DEFAULT,
    " \
      number : /-?[0-9]+/ ; \
      operator : '+' | '-' | '*' | '/' ; \
      expr : <number> | '(' <operator> <expr>+ ')' ; \
      clisp : /^/ <operator> <expr>+ /$/ ; \
    ",
    Number, Operator, Expr, Clisp);


  puts("clisp Version 0.0.1");
  puts("Press Ctrl+C to exit\n");

  while (1) {
    char* input = readline("clisp> ");

    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Clisp, &r)) {
      //mpc_ast_print(r.output);
      lval_println(eval(r.output));
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(4, Number, Operator, Expr, Clisp);

  return 0;
}
