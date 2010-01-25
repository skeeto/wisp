#include <stdio.h>
#include "common.h"
#include "mem.h"
#include "cons.h"
#include "symtab.h"
#include "object.h"

static mmanager_t *mm;

static void object_clear (void *o)
{
  object_t *obj = (object_t *) o;
  obj->type = SYMBOL;
  obj->refs = 0;
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
  o->refs++;
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
      o->val = symbol_create ();
      break;
    case STRING:
    case CFUNC:
    case SPECIAL:
      break;
    }
  return o;
}

object_t *c_cons (object_t * car, object_t * cdr)
{
  object_t *o = obj_create (CONS);
  CAR (o) = car;
  CDR (o) = cdr;
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

object_t *c_cfunc (object_t * (*f) (object_t *))
{
  object_t *o = obj_create (CFUNC);
  o->val = (void *) f;
  return o;
}

object_t *c_special (object_t * (*f) (object_t * o))
{
  object_t *o = obj_create (SPECIAL);
  o->val = (void *) f;
  return o;
}

void obj_destroy (object_t * o)
{
  if (SYMBOLP (o))
    return;
  o->refs--;
  if (o->refs > 0)
    return;

  switch (o->type)
    {
    case SYMBOL:
      /* Symbol objects are never destroyed. */
      return;
    case FLOAT:
    case INT:
    case STRING:
      free (o->val);
      break;
    case CONS:
      obj_destroy (CAR (o));
      obj_destroy (CDR (o));
      cons_destroy (o->val);
      break;
    case CFUNC:
    case SPECIAL:
      break;
    }
  mm_free (mm, (void *) o);
}

void obj_print (object_t * o, int newline)
{
  switch (o->type)
    {
    case CONS:
      printf ("(");
      object_t *p = o;
      while (p->type == CONS)
	{
	  obj_print (CAR (p), 0);
	  p = CDR (p);
	  if (p->type == CONS)
	    printf (" ");
	}
      if (p != NIL)
	{
	  printf (" . ");
	  obj_print (p, 0);
	}
      printf (")");
      break;
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
    case CFUNC:
      printf ("<CFUNC %p>", o->val);
      break;
    case SPECIAL:
      printf ("<SPECIAL %p>", o->val);
      break;
    default:
      printf ("ERROR");
    }

  if (newline)
    printf ("\n");
}
