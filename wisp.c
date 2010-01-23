#include <stdio.h>
#include "common.h"
#include "wisp.h"
#include "lisp.h"

/* parser crap */
extern FILE *yyin;
int yyparse ();
void parser_init ();

/* Initilize all the systems. */
void init ()
{
  /* These *must* be called in this order. */
  object_init ();
  symtab_init ();
  cons_init ();
  eval_init ();
  lisp_init ();
  parser_init ();
}

int main (int argc, char **argv)
{
  (void) argc;
  (void) argv;
  init ();

  yyin = stdin;
  yyparse();
}
