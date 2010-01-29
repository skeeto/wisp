#include <stdio.h>
#include <string.h>
#include "object.h"
#include "common.h"
#include "mem.h"
#include "str.h"

static mmanager_t *mm;

static void str_clear (void *s)
{
  str_t *str = (str_t *) s;
  str->raw = NULL;
  str->print = NULL;
  str->len = 0;
}

void str_init ()
{
  mm = mm_create (sizeof (str_t), &str_clear);
}

str_t *str_create ()
{
  return (str_t *) mm_alloc (mm);
}

void str_destroy (str_t * str)
{
  xfree (str->raw);
  if (str->print != NULL)
    xfree (str->print);
  mm_free (mm, (void *) str);
}

void str_genp (object_t * o)
{
  str_t *str = (str_t *) o->val;
  if (str->print == NULL)
    str->print = unprocess_str (str->raw);
}

object_t *str_cat (object_t * ao, object_t * bo)
{
  str_t *a = (str_t *) ao->val;
  str_t *b = (str_t *) bo->val;
  size_t nlen = a->len + b->len;
  char *nraw = xmalloc (nlen + 1);
  memcpy (nraw, a->raw, a->len);
  memcpy (nraw + a->len, b->raw, b->len);
  nraw[nlen] = '\0';
  return c_str (nraw, nlen);
}

object_t *c_str (char *str, size_t len)
{
  object_t *o = obj_create (STRING);
  OSTR (o) = str;
  OSTRLEN (o) = len;
  return o;
}

object_t *c_strs (char *str)
{
  return c_str (str, strlen (str));
}

uint32_t str_hash (object_t * o)
{
  return hash (OSTR (o), OSTRLEN (o));
}
