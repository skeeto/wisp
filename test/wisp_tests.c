#include <stdio.h>
#include <string.h>
#include "../common.h"
#include "../cons.h"
#include "../symtab.h"
#include "../object.h"
#include "../eval.h"
#include "../hashtab.h"
#include "../mem.h"

int err_cnt = 0, test_cnt = 0;

void assert (int b, char *msg);
void symbol_tests ();
void string_tests ();

void init ()
{
  /* These *must* be called in this order. */
  object_init ();
  symtab_init ();
  cons_init ();
  eval_init ();
}

int main ()
{
  init ();

  printf ("Running symbol tests ...\n");
  symbol_tests ();
  printf ("Running string tests ...\n");
  string_tests ();

  if (err_cnt == 0)
    {
      printf ("All %d tests passed.\n", test_cnt);
      exit (EXIT_SUCCESS);
    }
  else
    {
      printf ("%d tests failed.\n", err_cnt);
      exit (EXIT_FAILURE);
    }
}

void assert (int b, char *msg)
{
  test_cnt++;
  if (!b)
    {
      fprintf (stderr, "assert failed: %s\n", msg);
      err_cnt++;
    }
  else
    {
      printf ("assert passed: %s\n", msg);
    }
}

void string_tests ()
{
  assert (strcmp (process_str ("\"Hello\\\" there!\""), "Hello\" there!") ==
	  0, "process_str()");
  assert (strcmp (unprocess_str ("Hello\" there!"), "\"Hello\\\" there!\"") ==
	  0, "unprocess_str()");
}

void symbol_tests ()
{
  assert (c_sym ("symbol") == c_sym ("symbol"), "symbol interning");
  assert (strcmp (SYMNAME (c_sym ("str")), "str") == 0, "SYMNAME test");

  /* dynamic scoping */
  object_t *so = c_sym ("s");
  object_t *a = c_sym ("a");
  object_t *b = c_sym ("b");
  object_t *c = c_sym ("c");
  SET (so, a);
  sympush (so, b);
  sympush (so, c);
  assert (GET (so) == c, "symbol push/pop 1");
  sympop (so);
  assert (GET (so) == b, "symbol push/pop 2");
  sympop (so);
  assert (GET (so) == a, "symbol push/pop 3");
}
