#include <string.h>
#include <stdio.h>
#include "common.h"
#include "symtab.h"
#include "object.h"
#include "hashtab.h"

static hashtab_t *symbol_table;

object_t *NIL, *T;

void symtab_init ()
{
  symbol_table = ht_init (2048, NULL);

  /* Set up t and nil constants. The SET macro won't work until NIL is set. */
  NIL = c_sym ("nil");
  *((symbol_t *) OVAL (NIL))->vals = NIL;
  SYMPROPS (NIL) |= SYM_CONSTANT;
  T = c_sym ("t");
  SYMPROPS (T) |= SYM_CONSTANT;
  SET (T, T);
}

symbol_t *symbol_create ()
{
  symbol_t *s = xmalloc (sizeof (symbol_t));
  s->props = 0;
  s->cnt = 8;
  s->vals = s->stack = xmalloc (sizeof (object_t *) * s->cnt);
  return s;
}

void sympush (object_t * so, object_t * o)
{
  symbol_t *s = (symbol_t *) OVAL (so);
  s->vals++;
  if (s->vals == s->cnt + s->stack)
    {
      size_t n = s->vals - s->stack;
      s->cnt *= 2;
      s->stack = xrealloc (s->stack, s->cnt * sizeof (object_t *));
      s->vals = s->stack + n;
    }
  *s->vals = o;
  UPREF (o);
}

void sympop (object_t * so)
{
  obj_destroy (GET (so));
  symbol_t *s = (symbol_t *) OVAL (so);
  s->vals--;
}

object_t *c_usym (char *name)
{
  object_t *o;
  char *newname = xstrdup (name);
  o = obj_create (SYMBOL);
  SYMNAME (o) = newname;
  *((symbol_t *) OVAL (o))->vals = NIL;
  if (name[0] == ':')
    SET (o, o);
  return o;
}

void intern (object_t * sym)
{
  ht_insert (symbol_table, SYMNAME (sym), strlen (SYMNAME (sym)), sym,
	     sizeof (object_t *));
  SYMPROPS (sym) |= SYM_INTERNED;
}

object_t *c_sym (char *name)
{
  object_t *o = (object_t *) ht_search (symbol_table, name, strlen (name));
  if (o == NULL)
    {
      o = c_usym (name);
      intern (o);
    }
  return o;
}

uint32_t symbol_hash (object_t * o)
{
  return hash (SYMNAME (o), strlen (SYMNAME (o)));
}
