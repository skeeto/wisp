#ifndef NUMBER_H
#define NUMBER_H

#include <gmp.h>
#include "object.h"

object_t *c_ints (char *n);
object_t *c_int (int n);
object_t *c_floats (char *f);
object_t *c_float (double f);

/* get native numbers from number objects */
int into2int (object_t * into);
double floato2int (object_t * floato);

#define OINT(o) ((mpz_t *) (o)->val)
#define OFLOAT(o) ((mpf_t *) (o)->val)
#define DINT(o) (*((mpz_t *) (o)->val))
#define DFLOAT(o) (*((mpf_t *) (o)->val))

#define INTP(o) (o->type == INT)
#define FLOATP(o) (o->type == FLOAT)
#define NUMP(o) (INTP (o) || FLOATP (o))

uint32_t int_hash (object_t * o);
uint32_t float_hash (object_t * o);

#endif /* NUMBER_H */
