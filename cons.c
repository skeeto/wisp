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
