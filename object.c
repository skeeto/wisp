#include "cons.h"
#include "common.h"
#include "object.h"
#include "symtab.h"

static mmanager_t *mm;

static void object_clear (void *o)
{
  object_t *obj = (object_t *) o;
  obj->type = EMPTY;
  obj->val = NIL;
}

void object_init ()
{
  mm = mm_create (sizeof (object_t), &object_clear);
}

object_t *obj_create (type_t type)
{
  object_t *o = (object_t *) mm_alloc (mm);
  o->type = type;
  switch (type)
    {
    case INT:
      o->val = xmalloc (sizeof (int));
      break;
    case FLOAT:
      o->val = xmalloc (sizeof (double));
      break;
    case CONS:
      o->val = cons_create ();
      break;
    case SYMBOL:
      o->val = xmalloc (sizeof (symbol_t));
      break;
    case STRING:
    case CFUNC:
    case EMPTY:
      break;
    }
  return o;
}

object_t *c_int (int n)
{
  object_t *o = obj_create (INT);
  *((int *) o->val) = n;
  return o;
}

object_t *c_float (double f)
{
  object_t *o = obj_create (FLOAT);
  *((double *) o->val) = f;
  return o;
}

object_t *c_str (char *str)
{
  object_t *o = obj_create (STRING);
  o->val = (void *) str;
  return o;
}

void obj_destroy (object_t * o)
{
  mm_free (mm, (void *) o);
}

void obj_print (object_t * o)
{
  switch (o->type)
    {
    case INT:
      printf ("%d", *((int *) o->val));
      break;
    case FLOAT:
      printf ("%f", *((double *) o->val));
      break;
    case STRING:
      printf ("\"%s\"", (char *) o->val);
      break;
    case SYMBOL:
      printf ("%s", ((symbol_t *) o->val)->name);
      break;
    case EMPTY:
      printf ("EMPTY");
      break;
    case CONS:
      printf ("(");
      obj_print (car (o));
      printf (" . ");
      obj_print (cdr (o));
      printf (")");
      break;
    case CFUNC:
      printf ("CFUNC(%p)", o->val);
      break;
    default:
      printf ("ERROR");
    }
}
