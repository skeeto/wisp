#include "number.h"

object_t *c_ints (char *nstr)
{
  object_t *o = obj_create (INT);
  mpz_t *z = o->val;
  mpz_init (*z);
  mpz_set_str (*z, nstr, 10);
  return o;
}

object_t *c_int (int n)
{
  object_t *o = obj_create (INT);
  mpz_t *z = o->val;
  mpz_init (*z);
  mpz_set_ui (*z, n);
  return o;
}

object_t *c_floats (char *fstr)
{
  object_t *o = obj_create (FLOAT);
  mpf_t *f = o->val;
  mpf_init (*f);
  mpf_set_str (*f, fstr, 10);
  return o;
}

object_t *c_float (double d)
{
  object_t *o = obj_create (FLOAT);
  mpf_t *f = o->val;
  mpf_init (*f);
  mpf_set_d (*f, d);
  return o;
}
