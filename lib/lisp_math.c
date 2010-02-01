#include <stdio.h>
#include <gmp.h>
#include "wisp.h"

typedef enum arith_enum
{ ADD, SUB, MUL, DIV } arith_t;

typedef enum cmp_enum
{ EQ, LT, LTE, GT, GTE, } cmp_t;

/* Maths */
object_t *arith (arith_t op, object_t * lst)
{
  if (op == DIV)
    REQM (lst, 2, c_sym ("div"));
  int intmode = 1;
  object_t *accumz = NIL, *accumf = NIL, *convf = c_float (0);
  switch (op)
    {
    case SUB:
    case ADD:
      accumz = c_int (0);
      accumf = c_float (0);
      break;
    case MUL:
    case DIV:
      accumz = c_int (1);
      accumf = c_float (1);
      break;
    }
  if (op == SUB || op == DIV)
    {
      object_t *num = CAR (lst);
      if (FLOATP (num))
	{
	  intmode = 0;
	  mpf_set (DFLOAT (accumf), DFLOAT (num));
	}
      else if (INTP (num))
	{
	  mpf_set_z (DFLOAT (accumf), DINT (num));
	  mpz_set (DINT (accumz), DINT (num));
	}
      else
	{
	  obj_destroy (accumz);
	  obj_destroy (accumf);
	  obj_destroy (convf);
	  THROW (wrong_type, UPREF (num));
	}
      if (op == SUB && CDR (lst) == NIL)
	{
	  if (intmode)
	    {
	      obj_destroy (accumf);
	      obj_destroy (convf);
	      mpz_neg (DINT (accumz), DINT (accumz));
	      return accumz;
	    }
	  else
	    {
	      obj_destroy (accumz);
	      obj_destroy (convf);
	      mpf_neg (DFLOAT (accumf), DFLOAT (accumf));
	      return accumf;
	    }
	}
      lst = CDR (lst);
    }

  while (lst != NIL)
    {
      object_t *num = CAR (lst);
      if (!NUMP (num))
	{
	  obj_destroy (accumz);
	  obj_destroy (accumf);
	  obj_destroy (convf);
	  THROW (wrong_type, UPREF (num));
	}

      if (intmode)
	{
	  if (FLOATP (num))
	    {
	      intmode = 0;
	      mpf_set_z (DFLOAT (accumf), DINT (accumz));
	      /* Fall through to !intmode */
	    }
	  else if (INTP (num))
	    {
	      switch (op)
		{
		case ADD:
		  mpz_add (DINT (accumz), DINT (accumz), DINT (CAR (lst)));
		  break;
		case MUL:
		  mpz_mul (DINT (accumz), DINT (accumz), DINT (CAR (lst)));
		  break;
		case SUB:
		  mpz_sub (DINT (accumz), DINT (accumz), DINT (CAR (lst)));
		  break;
		case DIV:
		  mpz_div (DINT (accumz), DINT (accumz), DINT (CAR (lst)));
		  break;
		}
	    }
	}
      if (!intmode)
	{
	  if (FLOATP (num))
	    switch (op)
	      {
	      case ADD:
		mpf_add (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (num));
		break;
	      case MUL:
		mpf_mul (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (num));
		break;
	      case SUB:
		mpf_sub (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (num));
		break;
	      case DIV:
		mpf_div (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (num));
		break;
	      }
	  else if (INTP (num))
	    {
	      /* Convert to float and add. */
	      mpf_set_z (DFLOAT (convf), DINT (num));
	      switch (op)
		{
		case ADD:
		  mpf_add (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (convf));
		  break;
		case MUL:
		  mpf_mul (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (convf));
		  break;
		case SUB:
		  mpf_sub (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (convf));
		  break;
		case DIV:
		  mpf_div (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (convf));
		  break;
		}
	    }
	}
      lst = CDR (lst);
    }
  obj_destroy (convf);

  /* Destroy whatever went unused. */
  if (intmode)
    {
      obj_destroy (accumf);
      return accumz;
    }
  obj_destroy (accumz);
  return accumf;
}

object_t *addition (object_t * lst)
{
  return arith (ADD, lst);
}

object_t *multiplication (object_t * lst)
{
  return arith (MUL, lst);
}

object_t *subtraction (object_t * lst)
{
  return arith (SUB, lst);
}

object_t *division (object_t * lst)
{
  return arith (DIV, lst);
}

object_t *num_cmp (cmp_t cmp, object_t * lst)
{
  REQM (lst, 2, c_sym ("="));
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  int r = 0, invr = 1;
  if (INTP (a) && INTP (b))
    r = mpz_cmp (DINT (a), DINT (b));
  else if (FLOATP (a) && FLOATP (b))
    r = mpf_cmp (DFLOAT (a), DFLOAT (b));
  else if (INTP (a) && FLOATP (b))
    {
      /* Swap and handle below. */
      object_t *c = b;
      b = a;
      a = c;
      invr = -1;
    }
  if (FLOATP (a) && INTP (b))
    {
      /* Convert down. */
      object_t *convf = c_float (0);
      mpf_set_z (DFLOAT (convf), DINT (b));
      r = mpf_cmp (DFLOAT (a), DFLOAT (convf));
      obj_destroy (convf);
    }
  r *= invr;
  switch (cmp)
    {
    case EQ:
      r = (0 == r);
      break;
    case LT:
      r = (r < 0);
      break;
    case LTE:
      r = (r <= 0);
      break;
    case GT:
      r = (r > 0);
      break;
    case GTE:
      r = (r >= 0);
      break;
    }
  if (r)
    return T;
  return NIL;
}

object_t *num_eq (object_t * lst)
{
  return num_cmp (EQ, lst);
}

object_t *num_lt (object_t * lst)
{
  return num_cmp (LT, lst);
}

object_t *num_lte (object_t * lst)
{
  return num_cmp (LTE, lst);
}

object_t *num_gt (object_t * lst)
{
  return num_cmp (GT, lst);
}

object_t *num_gte (object_t * lst)
{
  return num_cmp (GTE, lst);
}

object_t *modulus (object_t * lst)
{
  REQ (lst, 2, c_sym ("%"));
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  if (!INTP (a))
    THROW (wrong_type, UPREF (a));
  if (!INTP (b))
    THROW (wrong_type, UPREF (b));
  object_t *m = c_int (0);
  mpz_mod (DINT (m), DINT (a), DINT (b));
  return m;
}

/* Install all the math functions */
void lisp_math_init ()
{
  SSET (c_sym ("+"), c_cfunc (&addition));
  SSET (c_sym ("*"), c_cfunc (&multiplication));
  SSET (c_sym ("-"), c_cfunc (&subtraction));
  SSET (c_sym ("/"), c_cfunc (&division));
  SSET (c_sym ("="), c_cfunc (&num_eq));
  SSET (c_sym ("<"), c_cfunc (&num_lt));
  SSET (c_sym ("<="), c_cfunc (&num_lte));
  SSET (c_sym (">"), c_cfunc (&num_gt));
  SSET (c_sym (">="), c_cfunc (&num_gte));
  SSET (c_sym ("%"), c_cfunc (&modulus));
}
