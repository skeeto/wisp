#include "cons.h"
#include "common.h"
#include "object.h"
#include "symtab.h"

static mmanager_t *mm;

static void object_clear (void *o)
{
  object_t *obj = (object_t *) o;
  obj->type = EMPTY;
  obj->val = NULL;
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

object_t *create_int (int n)
{
  object_t *o = obj_create (INT);
  *((int *) o->val) = n;
  return o;
}

object_t *create_float (double f)
{
  object_t *o = obj_create (FLOAT);
  *((double *) o->val) = f;
  return o;
}

object_t *create_symbol (char *name)
{
  object_t *o = obj_create (SYMBOL);
  ((symbol_t *) o->val)->name = name;
  return o;
}

object_t *create_string (char *str)
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
  if (o == NULL)
    {
      printf ("nil");
      return;
    }

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
      cons_print (o);
      break;
    case CFUNC:
      printf("CFUNC(%p)", o->val);
      break;
    default:
      printf ("ERROR");
    }
}
