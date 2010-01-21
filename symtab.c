#include <string.h>
#include "object.h"
#include "common.h"
#include "hashtab.h"
#include "symtab.h"

typedef struct entry
{
  object_t *sym;
  object_t *val;
} entry_t;

static hashtab_t *symbol_table;

void symtab_init ()
{
  symbol_table = ht_init (2048, NULL);
}

object_t *get_value (object_t * so)
{
  if (so->type != SYMBOL)
    {
      error ("warning: attempted lookup on non-symbol");
      return NULL;
    }
  symbol_t *s = (symbol_t *) so->val;
  entry_t *e =
    (entry_t *) ht_search (symbol_table, s->name, strlen (s->name));
  if (e == NULL)
    return NULL;
  return e->val;
}

void set_value (object_t * so, object_t * o)
{
  if (so->type != SYMBOL)
    {
      error ("warning: attempted set on non-symbol");
      return;
    }
  symbol_t *s = (symbol_t *) so->val;
  entry_t *e = xmalloc (sizeof (entry_t));
  e->sym = so;
  e->val = o;
  ht_insert (symbol_table, s->name, strlen (s->name), e, sizeof (entry_t *));
}

int sym_eq (object_t * a, object_t * b)
{
  return a == b;
}

object_t *c_sym (char *name)
{
  entry_t *e =
    (entry_t *) ht_search (symbol_table, name, strlen (name));
  if (e == NULL)
    {
      /* intern this symbol */
      e = xmalloc (sizeof (entry_t));
      object_t *o = obj_create (SYMBOL);
      symbol_t *s = o->val;
      s->name = name;
      e->sym = o;
      e->val = NULL;
      ht_insert (symbol_table, s->name, strlen (s->name), e,
		 sizeof (entry_t *));
    }
  return e->sym;
}
