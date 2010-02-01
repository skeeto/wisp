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

void xfree (void *p)
{
  free (p);
}

char *xstrdup (char *str)
{
  char *newstr = xmalloc (strlen (str) + 1);
  strcpy (newstr, str);
  return newstr;
}

char *pathcat (char *prefix, char *path)
{
  char *str = xmalloc (strlen (prefix) + strlen (path) + 2);
  strcpy (str, prefix);
  str[strlen (prefix)] = '/';	/* Extra / don't hurt. */
  strcpy (str + strlen (prefix) + 1, path);
  return str;
}

char *process_str (char *rawstr)
{
  char *str = xstrdup (rawstr + 1);	/* trim leading quote */

  /* Remove backquotes. */
  char *eq;
  char *p = str;
  while ((eq = strchr (p, '\\')) != NULL)
    {
      /* replace \ with next character */
      *eq = *(eq + 1);
      memmove (eq + 1, eq + 2, strlen (eq) + 1);
      p = eq + 1;
    }

  str[strlen (str) - 1] = '\0';	/* remove trailing quote */

  return str;
}

char *unprocess_str (char *cleanstr)
{
  /* Count the quotes and backquotes. */
  char *p = cleanstr;
  int cnt = 0;
  while (*p != '\0')
    {
      if (*p == '\\' || *p == '"')
	cnt++;
      p++;
    }

  /* Two extra for quotes and one for each character that needs
     escaping. */
  char *str = xmalloc (strlen (cleanstr) + cnt + 2 + 1);
  strcpy (str + 1, cleanstr);

  /* Place backquotes. */
  char *c = str + 1;
  while (*c != '\0')
    {
      if (*c == '\\' || *c == '"')
	{
	  memmove (c + 1, c, strlen (c) + 1);
	  *c = '\\';
	  c++;
	}
      c++;
    }

  /* Surrounding quotes. */
  str[0] = '"';
  str[strlen (str) + 1] = '\0';
  str[strlen (str)] = '"';

  return str;
}

void error (char *str)
{
  printf ("%s\n", str);
}
