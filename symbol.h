#ifndef SYMBOL_H
#define SYMBOL_H

#include "object.h"

typedef struct symbol
{
  char *name;
} symbol_t;

/* Must be called before any other functions. */
void symbol_init ();

/* Same symbol? */
int sym_eq(symbol_t *a, symbol_t *b);

#endif /* SYMBOL_H */
