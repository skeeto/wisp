#include <stdio.h>
#include "mem.h"
#include "object.h"
#include "symtab.h"
#include "cons.h"

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
  return is_symbol_list (CAR (lst));
}

/* Verifies that list is made only of symbols. */
int is_symbol_list (object_t * lst)
{
  if (lst == NIL)
    return 1;
  if (!SYMBOLP (CAR (lst)))
    return 0;
  return is_symbol_list (CDR (lst));
}
