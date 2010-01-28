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
  mm = mm_create (sizeof (cons_t), &cons_clear);
}

cons_t *cons_create ()
{
  return (cons_t *) mm_alloc (mm);
}

void cons_destroy (cons_t * o)
{
  mm_free (mm, (void *) o);
}

object_t *req_length (object_t * lst, object_t *thr, int n)
{
  /* TODO detect loops? */
  int cnt = 0;
  object_t *p = lst;
  while (p != NIL)
    {
      cnt++;
      p = CDR (p);
      if (!LISTP (p))
	{
	  obj_destroy (thr);
	  THROW(improper_list, lst);
	}
      if (cnt > n)
	THROW(wrong_number_of_arguments, thr);
    }
  if (cnt != n)
    THROW(wrong_number_of_arguments, thr);
  return T;
}

object_t *reqm_length (object_t * lst, object_t *thr, int n)
{
  /* TODO detect loops? */
  int cnt = 0;
  object_t *p = lst;
  while (p != NIL)
    {
      cnt++;
      p = CDR (p);
      if (!LISTP (p))
	{
	  obj_destroy (thr);
	  THROW(improper_list, lst);
	}
      if (cnt >= n)
	return T;
    }
  if (cnt < n)
    THROW(wrong_number_of_arguments, thr);
  return T;
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

/* Determine if list is proper (ends with NIL) */
object_t *properlistp (object_t * lst)
{
  if (lst == NIL)
    return T;
  if (!CONSP (lst))
    return NIL;
  return properlistp (CDR (lst));
}
