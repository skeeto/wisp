#include <string.h>
#include "symbol.h"
#include "object.h"
#include "common.h"
#include "hashtab.h"

static hashtab_t *symbol_table;

void symtab_init ()
{
  symbol_table = ht_init (2048, NULL);
}

object_t *get_value (symbol_t * s)
{
  return (object_t *) ht_search (symbol_table, s->name, strlen (s->name));
}

void set_value (symbol_t * s, object_t * o)
{
  ht_insert (symbol_table, s->name, strlen (s->name), o, sizeof (object_t *));
}
