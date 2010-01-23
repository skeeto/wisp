#ifndef OBJECT_H
#define OBJECT_H

typedef enum types
{ INT, FLOAT, STRING, SYMBOL, CONS, CFUNC, CMACRO, SPECIAL } type_t;

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
object_t *c_cons (object_t * car, object_t * cdr);
object_t *c_int (int n);
object_t *c_float (double f);
object_t *c_str (char *str);
object_t *c_cfunc (object_t * (*f) (object_t * o));
object_t *c_cmacro (object_t * (*f) (object_t * o));
void obj_destroy (object_t * o);

#define OINT(o) *((int *) o->val)
#define OFLOAT(o) *((double *) o->val)
#define OSTR(o) (char *) o->val

#endif /* OBJECT_H */
