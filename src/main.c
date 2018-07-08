# include <stdio.h>
# include <stdlib.h>

# include <editline/readline.h>

# include "mpc.h"
# include "lval.h"

int main(int argc, char** argv) {
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Symbol = mpc_new("symbol");
  mpc_parser_t* Sexpr = mpc_new("sexpr");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* Clisp = mpc_new("clisp");

  mpca_lang(MPCA_LANG_DEFAULT,
    " \
      number : /-?[0-9]+/ ; \
      symbol : '+' | '-' | '*' | '/' ; \
      sexpr : '(' <expr>* ')' ; \
      expr : <number> | <symbol> | <sexpr> ; \
      clisp : /^/ <expr>* /$/ ; \
    ",
    Number, Symbol, Sexpr, Expr, Clisp);


  puts("clisp Version 0.0.1");
  puts("Press Ctrl+C to exit\n");

  while (1) {
    char* input = readline("clisp> ");

    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Clisp, &r)) {
      //mpc_ast_print(r.output);
      lval* parsed = lval_read(r.output);
      lval* evaled = lval_eval(parsed);
      lval_println(evaled);
      lval_del(evaled);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Clisp);

  return 0;
}
