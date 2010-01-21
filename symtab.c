#include <string.h>
#include "object.h"
#include "common.h"
#include "hashtab.h"
#include "symtab.h"

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

object_t *c_sym (char *name)
{
  object_t *o = (object_t *) ht_search (symbol_table, name, strlen (name));
  if (o == NULL)
    {
      /* intern this symbol */
      o = obj_create (SYMBOL);
      SYMNAME (o) = name;
      SET (o, NIL);
      ht_insert (symbol_table, name, strlen (name), o, sizeof (object_t *));
    }
  return o;
}
