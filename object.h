/* objects.h - functions for manipulation of objects */
#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

typedef enum types
{ INT, FLOAT, STRING, SYMBOL, CONS, VECTOR, CFUNC, SPECIAL } type_t;

typedef union obval
{
  void *val;
  struct object *(*fval) (struct object *);
} obval_t;

typedef struct object
{
  type_t type;
  unsigned int refs;
  obval_t uval;
} object_t;

typedef object_t *(*cfunc_t) (object_t *);

#define OVAL(o) ((o)->uval.val)
#define FVAL(o) ((o)->uval.fval)

/* Must be called before any other functions. */
void object_init ();

/* Print an arbitrary object to stdout */
void obj_print (object_t * o, int newline);

/* Object creation */
object_t *obj_create (type_t type);
object_t *c_cons (object_t * car, object_t * cdr);
object_t *c_cfunc (cfunc_t f);
object_t *c_special (cfunc_t f);
void obj_destroy (object_t * o);

/* object hash functions */
uint32_t obj_hash (object_t * o);
uint32_t hash (void *buf, size_t buflen);

#define STRINGP(o) (o->type == STRING)
#define SYMBOLP(o) (o->type == SYMBOL)
#define CONSP(o) (o->type == CONS)

#define UPREF(o) ((o)->refs++, o)

/* Used for debugging: print string followed by object. */
#define DB_OP(str, o) printf(str); obj_print(o,1);

#endif /* OBJECT_H */
