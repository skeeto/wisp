#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include "common.h"
#include "wisp.h"
#include "lisp.h"

char *version = "alpha";

/* options */
char *progname;
int force_interaction = 0;
int print_help = 0;
char *core_file = "core.wisp";
char *root = NULL;

/* parser crap */
void parser_init ();
int parse (FILE * fid, char *name, int interactive);

void print_usage (int ret)
{
  printf ("Usage: %s [switches] programfile [arguments]\n", progname);
  printf ("\t-i           Force interaction mode\n");
  printf ("\t-v           Print version information\n");
  printf ("\t-h           Print this usage text\n");
  exit (ret);
}

void print_version ()
{
  printf ("wisp, version %s.\n", version);
  printf ("Copyright (C) 2010 Chris Wellons\n");
  printf ("This is free software; see the source code ");
  printf ("for copying conditions.\n");
  printf ("There is ABSOLUTELY NO WARRANTY; not even for ");
  printf ("MERCHANTIBILITY or\nFITNESS FOR A PARTICULAR PURPOSE.\n\n");
}

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
  progname = argv[0];
  init ();

  /* parse arguments */
  int c;
  while ((c = getopt (argc, argv, "+ihv")) != -1)
    switch (c)
      {
      case 'i':
	force_interaction = 1;
	break;
      case 'v':
	print_version ();
	break;
      case 'h':
	print_help = 1;
	break;
      case '?':
	print_usage (EXIT_FAILURE);
	break;
      }
  if (print_help)
    print_usage (EXIT_SUCCESS);

  /* Load core lisp code. */
  root = getenv ("WISPROOT");
  if (root != NULL)
    core_file = pathcat (root, core_file);
  FILE *core = fopen (core_file, "r");
  if (core == NULL)
    {
      fprintf (stderr, "error: could not load core lisp \"%s\": %s\n",
	       core_file, strerror (errno));
      if (root == NULL)
	fprintf (stderr, "warning: perhaps you should set WISPROOT\n");
      exit (EXIT_FAILURE);
    }
  parse (core, "core.wisp", 0);
  fclose (core);

  if (argc - optind < 1)
    {
      /* Run interaction. */
      parse (stdin, "<stdin>", 1);
    }
  else
    {
      /* Run script */
      char *file = argv[optind];
      FILE *fid = fopen (file, "r");
      if (fid == NULL)
	{
	  fprintf (stderr, "error: could not load %s: %s\n",
		   file, strerror (errno));
	  exit (EXIT_FAILURE);
	}
      /* expose argv to wisp program */
      object_t *args = NIL;
      while (argc > optind)
	{
	  args = c_cons (c_str (argv[argc - 1]), args);
	  argc--;
	}
      SET (c_sym ("ARGS"), args);
      parse (fid, file, 0);
      fclose (fid);
    }
}
