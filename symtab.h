#ifndef SYMTAB_H
#define SYMTAB_H

#include "object.h"

typedef struct symbol
{
  char *name;
  object_t *val;
} symbol_t;

/* Must be called before any other symbtab functions are called. */
void symtab_init ();

void set_value (object_t * s, object_t * o);

/* Same symbol? */
int sym_eq(object_t *a, object_t *b);

object_t *c_sym (char *name);

/* Useful macros for accessing the symbol's fields */
#define SYMNAME(so) ((symbol_t *) so->val)->name
#define GET(so) ((symbol_t *) so->val)->val
#define SET(so, o) ((symbol_t *) so->val)->val = o

/* Constants */
extern object_t *NIL;

#endif /* SYMTAB_H */
