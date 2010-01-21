#include <stdio.h>
#include "cons.h"
#include "symtab.h"
#include "common.h"
#include "eval.h"

/* Initilize all the systems. */
void init ()
{
  cons_init ();
  object_init ();
  symtab_init ();
}

object_t *setq(object_t *lst)
{
  object_t *v = car(lst);
  if (v->type != SYMBOL)
    {
      printf("error: setq: not a symbol!\n");
      return NULL;
    }
  set_value(v->val, car(cdr(lst)));
  return NULL;
}

int main (int argc, char **argv)
{
  (void) argc;
  (void) argv;
  init ();

  object_t *c = obj_create (CONS);
  setcar (c, create_int (10));
  obj_print (cons (create_symbol ("defun"), cons (create_string ("hello"), c)));
  printf ("\n");

  object_t *str = create_string("hello, world");
  obj_print(str);
  printf("\n");
  set_value((create_symbol ("str"))->val, str);
  obj_print(get_value((create_symbol ("str"))->val));
  printf("\n");

  /* install setq */
  object_t *setqo = obj_create(CFUNC);
  setqo->val = &setq;
  set_value(create_symbol("setq")->val, setqo);
  
  object_t *tail = obj_create(CONS);
  setcar (tail, create_int(102));
  object_t *lst = cons(create_symbol("setq"), cons(create_symbol("n"), tail));
  obj_print(lst);
  printf("\n");
  eval( lst );
  obj_print(get_value(create_symbol("n")->val));
  printf("\n");
}
