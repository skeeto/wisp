#ifndef SYMTAB_H
#define SYMTAB_H

#include "object.h"

typedef struct symbol
{
  char *name;
} symbol_t;

/* Must be called before any other symbtab functions are called. */
void symtab_init ();

object_t *get_value (object_t * s);
void set_value (object_t * s, object_t * o);

/* Same symbol? */
int sym_eq(object_t *a, object_t *b);

object_t *c_sym (char *name);

#endif /* SYMTAB_H */
