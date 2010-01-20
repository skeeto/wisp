#include "symbol.h"
#include "common.h"
#include "hashtab.h"

static hashtab_t *symbol_table;

void symbol_init()
{
  symbol_table = ht_init(2048, NULL);
}
