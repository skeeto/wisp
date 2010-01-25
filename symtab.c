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
  *((symbol_t *) NIL->val)->vals = NIL;
  T = c_sym ("t");
  SET (T, T);
}

int sym_eq (object_t * a, object_t * b)
{
  return a == b;
}

symbol_t *symbol_create ()
{
  symbol_t *s = xmalloc (sizeof (symbol_t));
  s->cnt = 8;
  s->vals = s->stack = xmalloc (sizeof (object_t *) * s->cnt);
  return s;
}

void sympush (object_t * so, object_t * o)
{
  symbol_t *s = (symbol_t *) so->val;
  if (s->vals - s->stack > s->cnt)
    {
      size_t n = s->vals - s->stack;
      s->cnt *= 2;
      s->stack = xrealloc (s->stack, s->cnt);
      s->vals = s->stack + n;
    }
  s->vals++;
  *s->vals = o;
  UPREF (o);
}

void sympop (object_t * so)
{
  obj_destroy (GET (so));
  symbol_t *s = (symbol_t *) so->val;
  s->vals--;
}

object_t *c_sym (char *name)
{
  object_t *o = (object_t *) ht_search (symbol_table, name, strlen (name));
  if (o == NULL)
    {
      /* intern this symbol */
      char *newname = xstrdup (name);
      o = obj_create (SYMBOL);
      SYMNAME (o) = newname;
      *((symbol_t *) o->val)->vals = NIL;
      ht_insert (symbol_table, newname, strlen (newname), o,
		 sizeof (object_t *));
      if (name[0] == ':')
	SET (o, o);
    }
  return o;
}
