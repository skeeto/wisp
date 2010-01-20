#include <stdio.h>
#include "cons.h"
#include "symbol.h"
#include "common.h"

int main (int argc, char **argv)
{
  (void) argc;
  (void) argv;

  cons_init ();
  symbol_init ();
  object_init ();

  printf("begin\n");
  object_t *o = obj_create(INT);
  *((int*)o->val) = 10;
  printf("%d %d\n", o->type, INT);
  object_t *c = obj_create(CONS);
  printf("%d %d\n", o->type, INT);
  setleft(c, o);
  cons_print(c);
  printf("\n");
  cons_print(cons(obj_create(INT), c));
  printf("\n");
}
