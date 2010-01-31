/* symtab.h - functions related to symbols and the symbol table */
#ifndef SYMTAB_H
#define SYMTAB_H

#include "object.h"

typedef struct symbol
{
  char *name;
  char props;
  object_t **vals;
  object_t **stack;
  unsigned int cnt;
} symbol_t;

/* Must be called before any other symbtab functions are called. */
void symtab_init ();

/* Symbol creation */
symbol_t *symbol_create ();
object_t *c_sym (char *name);	/* Interned symbol. */
object_t *c_usym (char *name);	/* Uinterned symbol. */
void intern (object_t * sym);

/* Dynamic scoping */
void sympop (object_t * so);
void sympush (object_t * so, object_t * o);

/* Useful macros for accessing the symbol's fields */
#define SYMNAME(so) (((symbol_t *) so->val)->name)
#define GET(so) (*((symbol_t *) so->val)->vals)
#define SET(so, o) (obj_destroy (GET (so)), (void) UPREF (o), \
		    *((symbol_t *) so->val)->vals = o)
#define SSET(so, o) (obj_destroy (GET (so)), *((symbol_t *) so->val)->vals = o)

/* Constants */
extern object_t *NIL;
extern object_t *T;

/* symbol properties */
#define SYM_CONSTANT 1
#define SYM_INTERNED 2

#define SYMPROPS(o) ((((symbol_t *)(o)->val))->props)
#define CONSTANTP(o) (SYMPROPS(o) & SYM_CONSTANT)
#define INTERNP(o) (SYMPROPS(o) & SYM_INTERNED)

uint32_t symbol_hash (object_t * o);

#endif /* SYMTAB_H */
