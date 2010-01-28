#include <stdio.h>
#include <gmp.h>
#include "wisp.h"

/* Maths */
object_t *addition (object_t * lst)
{
  object_t *accumz = c_int (0);
  object_t *accumf = c_float (0);
  object_t *convf = c_float (0);
  int intmode = 1;
  while (lst != NIL)
    {
      object_t *num = CAR (lst);
      if (!NUMP (num))
	{
	  obj_destroy (accumz);
	  obj_destroy (accumf);
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
	    mpz_add (DINT (accumz), DINT (accumz), DINT (CAR (lst)));
	}
      if (!intmode)
	{
	  if (FLOATP (num))
	    mpf_add (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (num));
	  else if (INTP (num))
	    {
	      /* Convert to float and add. */
	      mpf_set_z (DFLOAT (convf), DINT (num));
	      mpf_add (DFLOAT (accumf), DFLOAT (accumf), DFLOAT (num));
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

/* Install all the math functions */
void lisp_math_init ()
{
  SSET (c_sym ("+"), c_cfunc (&addition));
}
