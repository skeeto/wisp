#include "cons.h"
#include "common.h"
#include "object.h"

static mmanager_t *mm;

static void object_clear(void *o)
{
  object_t *obj = (object_t*)o;
  obj->type = EMPTY;
  obj->val = NULL;
}

void object_init ()
{
  mm = mm_create (sizeof (object_t), &object_clear);
}

object_t *obj_create (type_t type)
{
  object_t *o = (object_t *) mm_alloc (mm);
  o->type = type;
  switch (type)
    {
    case INT:
      o->val = xmalloc(sizeof(int));
      break;
    case FLOAT:
      o->val = xmalloc(sizeof(double));
      break;
    case CONS:
      o->val = cons_create();
      break;
    case STRING:
    case SYMBOL:
    case EMPTY:
      break;
    }
  return o;
}

void obj_destroy (object_t * o)
{
  mm_free (mm, (void *) o);
}

void obj_print(object_t *o)
{
  switch (o->type)
    {
    case INT:
      printf("%d", *((int *) o->val));
      break;
    case FLOAT:
      printf("%f", *((double *) o->val));
      break;
    case STRING:
      printf("STRING");
    case SYMBOL:
      printf("SYMBOL");
    case EMPTY:
      printf("EMPTY");
    case CONS:
      printf("CONS");
      break;
    default:
      printf("BAD!");
   }
}
