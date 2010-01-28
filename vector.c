#include "vector.h"
#include "object.h"
#include "common.h"
#include "symtab.h"
#include "cons.h"
#include "mem.h"

static mmanager_t *mm;

static void vector_clear (void *o)
{
  vector_t *v = (vector_t *) o;
  v->v = NULL;
  v->len = 0;
}

void vector_init ()
{
  mm = mm_create (sizeof (vector_t), &vector_clear);
}

vector_t *vector_create ()
{
  return (vector_t *) mm_alloc (mm);
}

void vector_destroy (vector_t * v)
{
  for (size_t i = 0; i < v->len; i++)
    obj_destroy (v->v[i]);
  free (v->v);
  mm_free (mm, (void *) v);
}

object_t *c_vec (size_t len)
{
  object_t *o = obj_create (VECTOR);
  vector_t *v = o->val;
  v->len = len;
  if (v->len == 0)
    v->len = 1;
  v->v = xmalloc (sizeof (object_t **) * v->len);
  for (size_t i = 0; i < v->len; i++)
    v->v[i] = NIL;
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
  object_t *v = c_vec (cnt);
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
  vector_t *v = vo->val;
  object_t *o = v->v[i];
  v->v[i] = val;
  obj_destroy (o);
}

object_t *vget (object_t * vo, size_t i)
{
  vector_t *v = vo->val;
  return v->v[i];
}

void vec_print (object_t * vo)
{
  vector_t *v = vo->val;
  printf ("[");
  for (size_t i = 0; i < v->len - 1; i++)
    {
      obj_print (v->v[i], 0);
      printf (" ");
    }
  obj_print (v->v[v->len - 1], 0);
  printf ("]");
}
