#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "object.h"
#include "cons.h"

static mmanager_t *mm;

static void cons_clear (void *o)
{
  cons_t *c = (cons_t *) o;
  c->left = NULL;
  c->right = NULL;
}

void cons_init ()
{
  mm = mm_create (sizeof (object_t), &cons_clear);
}

cons_t *cons_create ()
{
  return (cons_t *) mm_alloc (mm);
}

void cons_destroy (cons_t * o)
{
  mm_free (mm, (void *) o);
}

object_t *cons(object_t *o, object_t *c)
{
  if (c != NULL && c->type != CONS)
    return NULL;
  object_t *newo = obj_create(CONS);
  cons_t *car = (cons_t *)newo->val;
  car->left = o;
  car->right = c;
  return newo;
}

void setleft (object_t * o, object_t * left)
{
  ((cons_t *)o->val)->left = left;
}

void cons_print (object_t * o)
{
  cons_t *c = (cons_t *) o->val;
  printf("(");
  obj_print(c->left);
  printf(" . ");
  if (c->right == NULL)
    {
      printf("nil)");
      return;
    }
  if (c->right->type == CONS)
    {
      cons_print(c->right);
      printf(")");
    }
  else
    {
      printf(" . ");
      obj_print(c->right);
      printf(")\n");
    }
}
