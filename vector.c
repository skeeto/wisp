#include "vector.h"
#include "object.h"
#include "common.h"
#include "symtab.h"
#include "cons.h"
#include "number.h"
#include "mem.h"
#include "eval.h"

static mmanager_t *mm;
static object_t *out_of_bounds;

static void vector_clear (void *o)
{
  vector_t *v = (vector_t *) o;
  v->v = NULL;
  v->len = 0;
}

void vector_init ()
{
  mm = mm_create (sizeof (vector_t), &vector_clear);
  out_of_bounds = c_sym ("index-out-of-bounds");
}

vector_t *vector_create ()
{
  return (vector_t *) mm_alloc (mm);
}

void vector_destroy (vector_t * v)
{
  size_t i;
  for (i = 0; i < v->len; i++)
    obj_destroy (v->v[i]);
  xfree (v->v);
  mm_free (mm, (void *) v);
}

object_t *c_vec (size_t len, object_t * init)
{
  object_t *o = obj_create (VECTOR);
  vector_t *v = OVAL(o);
  v->len = len;
  if (v->len == 0)
    v->len = 1;
  v->v = xmalloc (sizeof (object_t **) * v->len);
  size_t i;
  for (i = 0; i < v->len; i++)
    v->v[i] = UPREF (init);
  return o;
}

object_t *list2vector (object_t * lst)
{
  int cnt = 0;
  object_t *p = lst;
  while (p != NIL)
    {
      cnt++;
      p = CDR (p);
    }
  object_t *v = c_vec (cnt, NIL);
  p = lst;
  size_t i = 0;
  while (p != NIL)
    {
      vset (v, i, UPREF (CAR (p)));
      i++;
      p = CDR (p);
    }
  return v;
}

void vset (object_t * vo, size_t i, object_t * val)
{
  vector_t *v = OVAL(vo);
  object_t *o = v->v[i];
  v->v[i] = val;
  obj_destroy (o);
}

object_t *vset_check (object_t * vo, object_t * io, object_t * val)
{
  int i = into2int (io);
  vector_t *v = OVAL(vo);
  if (i < 0 || i >= (int) v->len)
    THROW (out_of_bounds, UPREF (io));
  vset (vo, i, UPREF (val));
  return UPREF (val);
}

object_t *vget (object_t * vo, size_t i)
{
  vector_t *v = OVAL(vo);
  return v->v[i];
}

object_t *vget_check (object_t * vo, object_t * io)
{
  int i = into2int (io);
  vector_t *v = OVAL(vo);
  if (i < 0 || i >= (int) v->len)
    THROW (out_of_bounds, UPREF (io));
  return UPREF (vget (vo, i));
}

void vec_print (object_t * vo)
{
  vector_t *v = OVAL(vo);
  printf ("[");
  size_t i;
  for (i = 0; i < v->len - 1; i++)
    {
      obj_print (v->v[i], 0);
      printf (" ");
    }
  obj_print (v->v[v->len - 1], 0);
  printf ("]");
}

object_t *vector_concat (object_t * a, object_t * b)
{
  size_t al = VLENGTH (a), bl = VLENGTH (b);
  object_t *c = c_vec (al + bl, NIL);
  size_t i;
  for (i = 0; i < al; i++)
    vset (c, i, UPREF (vget (a, i)));
  for (i = 0; i < bl; i++)
    vset (c, i + al, UPREF (vget (b, i)));
  return c;
}

uint32_t vector_hash (object_t * o)
{
  uint32_t accum = 0;
  vector_t *v = OVAL(o);
  size_t i;
  for (i = 0; i < v->len; i++)
    accum ^= obj_hash (v->v[i]);
  return accum;
}
