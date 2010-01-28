/* objects.h - functions for manipulation of objects */
#ifndef OBJECT_H
#define OBJECT_H

typedef enum types
{ INT, FLOAT, STRING, SYMBOL, CONS, CFUNC, SPECIAL } type_t;

typedef struct object
{
  type_t type;
  unsigned int refs;
  void *val;
} object_t;

/* Must be called before any other functions. */
void object_init ();

/* Print an arbitrary object to stdout */
void obj_print (object_t * o, int newline);

/* Object creation */
object_t *obj_create (type_t type);
object_t *c_cons (object_t * car, object_t * cdr);
object_t *c_cfunc (object_t * (*f) (object_t * o));
object_t *c_special (object_t * (*f) (object_t * o));
void obj_destroy (object_t * o);

#define STRINGP(o) (o->type == STRING)
#define SYMBOLP(o) (o->type == SYMBOL)
#define CONSP(o) (o->type == CONS)

#define UPREF(o) ((o)->refs++, o)

/* Used for debugging: print string followed by object. */
#define DB_OP(str, o) printf(str); obj_print(o,1);

#endif /* OBJECT_H */
