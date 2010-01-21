#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "common.h"

void *xmalloc (size_t size)
{
  void *p = malloc (size);
  if (p == NULL)
    {
      fprintf (stderr, "error: fatal: out of memory: %s\n", strerror (errno));
      exit (EXIT_FAILURE);
    }
  return p;
}

void *xrealloc (void *p, size_t size)
{
  void *np = realloc (p, size);
  if (np == NULL)
    {
      fprintf (stderr, "error: fatal: out of memory: %s\n", strerror (errno));
      exit (EXIT_FAILURE);
    }
  return np;
}

void error (char *str)
{
  printf ("%s\n", str);
}
