#ifndef READER_H
#define READER_H

#include <stdio.h>
#include "object.h"

extern char *wisproot;

/* stack-dependant reader state */
typedef struct reader_state
{
  object_t *head;
  object_t *tail;
  int quote_mode, dotpair_mode, vector_mode;
} rstate_t;

/* the reader object */
typedef struct reader
{
  /* source */
  FILE *fid;
  char *str;

  /* meta */
  char *name;
  int interactive;
  char *prompt;

  /** reader state **/
  unsigned int linecnt;
  char *strp;

  /* atom read buffer */
  char *buf, *bufp;
  size_t buflen;

  /* read buffer */
  int *readbuf, *readbufp;
  size_t readbuflen;

  /* indicators */
  int eof, error, shebang, done;

  /* state stack */
  size_t ssize;
  rstate_t *base;
  rstate_t *state;
} reader_t;

/* Create a new reader object, passing either a string or file handle
 * for parsing. */
reader_t *reader_create (FILE * fid, char *str, char *name, int interactive);
void reader_destroy (reader_t * r);

/* Read a single sexp from the reader. */
object_t *read_sexp (reader_t * r);

/* Use the core functions above to eval each sexp in a file. */
int load_file (FILE * fid, char *filename, int interactive);

/* Convenience function for calling load_file() on stdin. */
void repl ();

/* Load a library file from the Wisp root */
int require (char *libname);

extern char *prompt;

#endif /* READER_H */
