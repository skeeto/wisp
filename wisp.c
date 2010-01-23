#include <stdio.h>
#include "common.h"
#include "wisp.h"
#include "lisp.h"

/* parser crap */
extern FILE *yyin;
int yyparse ();
void parser_init ();
extern int interactive;
extern char *prompt;

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

  /* Load core lisp code. */
  interactive = 0;
  yyin = fopen ("core.wisp", "r");
  if (yyin == NULL)
    {
      fprintf (stderr, "error: could not load core lisp.\n");
      exit (EXIT_FAILURE);
    }
  yyparse ();
  fclose (yyin);
  /* yy_flush_buffer(); */

  /* Run interaction. */
  interactive = 1;
  printf ("Happy hacking!\n%s", prompt);
  yyin = stdin;
  yyparse ();
}
