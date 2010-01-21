#ifndef OBJECT_H
#define OBJECT_H

#include "mem.h"
#include "symbol.h"

typedef enum types
{ INT, FLOAT, STRING, SYMBOL, CONS, CFUNC, EMPTY } type_t;

typedef struct object
{
  type_t type;
  void *val;
} object_t;

/* Must be called before any other functions. */
void object_init ();

/* Print an arbitrary object to stdout */
void obj_print (object_t * o);

/* Object creation */
object_t *obj_create (type_t type);
object_t *create_int (int n);
object_t *create_float (double f);
object_t *create_symbol (char *name);
object_t *create_string (char *str);
void obj_destroy (object_t * o);

#endif /* OBJECT_H */
