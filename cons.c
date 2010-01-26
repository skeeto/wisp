#include <stdio.h>
#include "mem.h"
#include "object.h"
#include "symtab.h"
#include "cons.h"
#include "eval.h"

static mmanager_t *mm;

static void cons_clear (void *o)
{
  cons_t *c = (cons_t *) o;
  c->car = NIL;
  c->cdr = NIL;
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

int length (object_t * lst)
{
  /* TODO detect loops? */
  int cnt = 0;
  while (lst != NIL)
    {
      cnt++;
      lst = CDR (lst);
    }
  return cnt;
}

/* Verifies that list has form of a function. */
int is_func_form (object_t * lst)
{
  if (!LISTP (CAR (lst)))
    return 0;
  return is_var_list (CAR (lst));
}

/* Verifies that list is made only of symbols. */
int is_var_list (object_t * lst)
{
  int rest_cnt = -1;
  while (lst != NIL)
    {
      object_t *car = CAR (lst);
      /* &rest must be the second to last item */
      if (rest_cnt >= 0)
	{
	  rest_cnt--;
	  if (rest_cnt < 0)
	    return 0;
	  if (car == rest)
	    return 0;
	}
      if (!SYMBOLP (car))
	return 0;
      if (car == rest)
	rest_cnt = 1;
      lst = CDR (lst);
    }
  if (rest_cnt == 1)
    return 0;
  return 1;
}
