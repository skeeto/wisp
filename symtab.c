#include <string.h>
#include "common.h"
#include "symtab.h"
#include "object.h"
#include "hashtab.h"

static hashtab_t *symbol_table;

object_t *NIL;

void symtab_init ()
{
  symbol_table = ht_init (2048, NULL);

  /* set up t and nil constants */
  NIL = c_sym ("nil");
  SET (NIL, NIL);
  SET (c_sym ("t"), c_sym ("t"));
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
}

void sympop (object_t * so)
{
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
      SET (o, NIL);
      ht_insert (symbol_table, newname, strlen (newname), o,
		 sizeof (object_t *));
    }
  return o;
}
