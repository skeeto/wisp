#include <stdio.h>
#include <stdint.h>
#include <gmp.h>
#include "common.h"
#include "mem.h"
#include "cons.h"
#include "symtab.h"
#include "str.h"
#include "object.h"
#include "number.h"
#include "vector.h"
#include "detach.h"

static mmanager_t *mm;

static void object_clear (void *o)
{
  object_t *obj = (object_t *) o;
  obj->type = SYMBOL;
  obj->refs = 0;
  FVAL (obj) = NULL;
  OVAL (obj) = NIL;
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
      OVAL (o) = xmalloc (sizeof (mpz_t));
      break;
    case FLOAT:
      OVAL (o) = xmalloc (sizeof (mpf_t));
      break;
    case CONS:
      OVAL (o) = cons_create ();
      break;
    case SYMBOL:
      OVAL (o) = symbol_create ();
      break;
    case STRING:
      OVAL (o) = str_create ();
      break;
    case VECTOR:
      OVAL (o) = vector_create ();
      break;
    case DETACH:
      OVAL (o) = xmalloc (sizeof (detach_t));
      break;
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

object_t *c_cfunc (cfunc_t f)
{
  object_t *o = obj_create (CFUNC);
  FVAL (o) = f;
  return o;
}

object_t *c_special (cfunc_t f)
{
  object_t *o = obj_create (SPECIAL);
  FVAL (o) = f;
  return o;
}

void obj_destroy (object_t * o)
{
  if (SYMBOLP (o))
    return;
  o->refs--;
  if (o->refs > 0)
    return;

  mpz_t *z;
  mpf_t *f;
  switch (o->type)
    {
    case SYMBOL:
      /* Symbol objects are never destroyed. */
      return;
    case FLOAT:
      f = OFLOAT (o);
      mpf_clear (*f);
      xfree (OVAL (o));
      break;
    case INT:
      z = OINT (o);
      mpz_clear (*z);
      xfree (OVAL (o));
      break;
    case STRING:
      str_destroy (OVAL (o));
      break;
    case CONS:
      obj_destroy (CAR (o));
      obj_destroy (CDR (o));
      cons_destroy (OVAL (o));
      break;
    case VECTOR:
      vector_destroy (OVAL (o));
      break;
    case DETACH:
      xfree (OVAL (o));
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
      gmp_printf ("%Zd", OINT (o));
      break;
    case FLOAT:
      gmp_printf ("%F.24f", OFLOAT (o));
      break;
    case STRING:
      printf ("%s", OSTRP (o));
      break;
    case SYMBOL:
      printf ("%s", ((symbol_t *) OVAL (o))->name);
      break;
    case VECTOR:
      vec_print (o);
      break;
    case DETACH:
      detach_print (o);
      break;
    case CFUNC:
      /* It's not possible to print a function pointer. */
      printf ("<CFUNC>");
      break;
    case SPECIAL:
      /* It's not possible to print a function pointer. */
      printf ("<SPECIAL>");
      break;
    default:
      printf ("ERROR");
    }

  if (newline)
    printf ("\n");
}

uint32_t obj_hash (object_t * o)
{
  switch (o->type)
    {
    case CONS:
      return cons_hash (o);
    case INT:
      return int_hash (o);
      break;
    case FLOAT:
      return float_hash (o);
      break;
    case STRING:
      return str_hash (o);
      break;
    case SYMBOL:
      return symbol_hash (o);
      break;
    case VECTOR:
      return vector_hash (o);
      break;
    case DETACH:
      return detach_hash (o);
      break;
    case CFUNC:
    case SPECIAL:
      /* Imprecise, but close enough */
      return hash (OVAL (o), sizeof (void *));
      break;
    }
  return 0;
}

uint32_t hash (void *key, size_t keylen)
{
  /* One-at-a-time hash */
  uint32_t hash, i;
  for (hash = 0, i = 0; i < keylen; ++i)
    {
      hash += ((char *) key)[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash;
}
