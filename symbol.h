#ifndef SYMBOL_H
#define SYMBOL_H

#include "object.h"

typedef struct symbol
{
  char *name;
  object_t *object;
} symbol_t;

/* Must be called before any other functions. */
void symbol_init ();

#endif /* SYMBOL_H */
