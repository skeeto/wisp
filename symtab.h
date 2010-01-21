#ifndef SYMTAB_H
#define SYMTAB_H

#include "symbol.h"
#include "object.h"

/* Must be called before any other symbtab functions are called. */
void symtab_init ();

object_t *get_value (symbol_t * s);
void set_value (symbol_t * s, object_t * o);

#endif /* SYMTAB_H */
