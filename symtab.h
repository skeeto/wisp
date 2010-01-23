#ifndef SYMTAB_H
#define SYMTAB_H

#include "object.h"

typedef struct symbol
{
  char *name;
  object_t **vals;
  object_t **stack;
  unsigned short cnt;
} symbol_t;

/* Must be called before any other symbtab functions are called. */
void symtab_init ();

void set_value (object_t * s, object_t * o);

/* Same symbol? */
int sym_eq (object_t * a, object_t * b);

symbol_t *symbol_create ();
object_t *c_sym (char *name);

/* dynamic scoping */
void sympop (object_t * so);;
void sympush (object_t * so, object_t * o);

/* Useful macros for accessing the symbol's fields */
#define SYMNAME(so) ((symbol_t *) so->val)->name
#define GET(so) *((symbol_t *) so->val)->vals
#define SET(so, o) *((symbol_t *) so->val)->vals = o

/* Constants */
extern object_t *NIL;
extern object_t *T;

#endif /* SYMTAB_H */
