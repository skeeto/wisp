#include <stdio.h>
#include <string.h>
#include "../common.h"
#include "../cons.h"
#include "../symtab.h"
#include "../object.h"
#include "../eval.h"
#include "../hashtab.h"
#include "../mem.h"

int err_cnt = 0;

void assert (int b, char *msg);
void symbol_tests ();

int main ()
{
  printf ("Running symbol tests ...\n");
  if (err_cnt == 0)
    {
      printf ("All tests passed.\n");
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
  if (!b)
    {
      fprintf (stderr, "assert failed: %s\n", msg);
      err_cnt++;
    }
}

void symbol_tests ()
{
  assert (c_sym ("symbol") == c_sym ("symbol"), "symbol interning");
  assert (strcmp (SYMNAME (c_sym ("str")), "str") == 0, "SYMNAME test");
}
