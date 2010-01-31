#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "number.h"

object_t *c_ints (char *nstr)
{
  object_t *o = obj_create (INT);
  mpz_t *z = OVAL(o);
  mpz_init (*z);
  mpz_set_str (*z, nstr, 10);
  return o;
}

object_t *c_int (int n)
{
  object_t *o = obj_create (INT);
  mpz_t *z = OVAL(o);
  mpz_init (*z);
  mpz_set_ui (*z, n);
  return o;
}

object_t *c_floats (char *fstr)
{
  object_t *o = obj_create (FLOAT);
  mpf_t *f = OVAL(o);
  mpf_init2 (*f, 64);
  mpf_set_str (*f, fstr, 10);
  return o;
}

object_t *c_float (double d)
{
  object_t *o = obj_create (FLOAT);
  mpf_t *f = OVAL(o);
  mpf_init2 (*f, 64);
  mpf_set_d (*f, d);
  return o;
}

int into2int (object_t * into)
{
  return mpz_get_si (DINT (into));
}

double floato2float (object_t * floato)
{
  return mpf_get_d (DFLOAT (floato));
}

uint32_t int_hash (object_t * o)
{
  char *str = mpz_get_str (NULL, 16, DINT (o));
  uint32_t h = hash (str, strlen (str));
  free (str);
  return h;
}

uint32_t float_hash (object_t * o)
{
  char *str = mpf_get_str (NULL, NULL, 16, 0, DFLOAT (o));
  uint32_t h = hash (str, strlen (str));
  free (str);
  return h;
}
