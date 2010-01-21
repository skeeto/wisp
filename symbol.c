#include <string.h>
#include "symbol.h"
#include "common.h"
#include "hashtab.h"

void symbol_init ()
{

}

int sym_eq (symbol_t * a, symbol_t * b)
{
  return strcmp (a->name, b->name) == 0;
}
