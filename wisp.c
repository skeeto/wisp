#include <stdio.h>
#include "common.h"
#include "wisp.h"
#include "lisp.h"

/* parser crap */
extern FILE *yyin;
int yyparse ();

/* Initilize all the systems. */
void init ()
{
  /* These *must* be called in this order. */
  object_init ();
  symtab_init ();
  cons_init ();
  eval_init ();
  lisp_init ();
}

int main (int argc, char **argv)
{
  (void) argc;
  (void) argv;
  init ();

  object_t *lst =
    c_cons (c_sym ("+"), c_cons (c_int (10), c_cons (c_int (34), NIL)));
  object_t *sexp = c_cons (c_sym ("+"), c_cons (lst, CDR (lst)));
  obj_print (sexp);
  printf ("\n");
  obj_print (eval (sexp));
  printf ("\n");

  yyin = stdin;
  yyparse();
}
