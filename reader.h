#ifndef READER_H
#define READER_H

#include <stdio.h>
#include "object.h"

typedef struct reader
{
  /* source */
  FILE *fid;
  char *str;

  /* meta */
  char *name;
  int interactive;
  char *prompt;

  /* reader state */
  unsigned int linecnt;
  char *strp;
  char *buf;
  char *bufp;
  size_t buflen;
  int *readbuf;
  int *readbufp;
  size_t readbuflen;
  int *qstack;
  int *qstackp;
  int qstacklen;
  int eof;

  /* sexp stack */
  size_t ssize;
  object_t **base;
  object_t **tip;
} reader_t;

/* Create a new reader object, passing either a string or file handle
 * for parsing. */
reader_t *reader_create (FILE * fid, char *str, char *name, int interactive);
void reader_destroy (reader_t * r);

/* Read a single sexp from the reader. */
object_t *read_sexp (reader_t * r);

/* Use the core functions above to eval each sexp in a file. */
int load_file (FILE * fid, char *filename, int interactive);

extern char *prompt;

#endif /* READER_H */
