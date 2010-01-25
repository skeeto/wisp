#include <stdio.h>
#include "common.h"
#include "wisp.h"
#include "lisp.h"

/* parser crap */
void parser_init ();
int parse (FILE *fid, char *name, int interactive);

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
  FILE *core = fopen ("core.wisp", "r");
  if (core == NULL)
    {
      fprintf (stderr, "error: could not load core lisp.\n");
      exit (EXIT_FAILURE);
    }
  parse (core, "core.wisp", 0);
  fclose (core);

  /* Run interaction. */
  printf ("Happy hacking!\n");
  parse (stdin, "<stdin>", 1);
}
