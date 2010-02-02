#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <gmp.h>
#include "object.h"
#include "cons.h"
#include "symtab.h"
#include "common.h"
#include "eval.h"
#include "str.h"
#include "reader.h"
#include "number.h"
#include "vector.h"

/* From lisp_math.c */
void lisp_math_init ();
object_t *num_eq (object_t * lst);

/* Various basic stuff */

object_t *cdoc_string (object_t * lst)
{
  DOC ("Return doc-string for CFUNC or SPECIAL.");
  REQ (lst, 1, c_sym ("cdoc-string"));
  object_t *fo = CAR (lst);
  int evaled = 0;
  if (SYMBOLP (fo))
    {
      evaled = 1;
      fo = eval (fo);
    }
  if (fo->type != CFUNC && fo->type != SPECIAL)
    {
      if (evaled)
	obj_destroy (fo);
      THROW (wrong_type, UPREF (fo));
    }
  cfunc_t f = FVAL (fo);
  object_t *str = f (doc_string);
  if (evaled)
    obj_destroy (fo);
  return str;
}

object_t *lisp_apply (object_t * lst)
{
  DOC ("Apply function to a list.");
  REQ (lst, 2, c_sym ("apply"));
  object_t *f = CAR (lst);
  object_t *args = CAR (CDR (lst));
  if (!LISTP (args))
    THROW (wrong_type, UPREF (args));
  return apply (f, args);
}

object_t *lisp_and (object_t * lst)
{
  DOC ("Evaluate each argument until one returns nil.");
  object_t *r = T, *p = lst;
  while (CONSP (p))
    {
      obj_destroy (r);
      r = eval (CAR (p));
      CHECK (r);
      if (r == NIL)
	return NIL;
      p = CDR (p);
    }
  if (p != NIL)
    THROW (improper_list, UPREF (lst));
  return UPREF (r);
}

object_t *lisp_or (object_t * lst)
{
  DOC ("Evaluate each argument until one doesn't return nil.");
  object_t *r = NIL, *p = lst;
  while (CONSP (p))
    {
      r = eval (CAR (p));
      CHECK (r);
      if (r != NIL)
	return UPREF (r);
      p = CDR (p);
    }
  if (p != NIL)
    THROW (improper_list, UPREF (lst));
  return NIL;
}

object_t *lisp_cons (object_t * lst)
{
  DOC ("Construct a new cons cell, given car and cdr.");
  REQ (lst, 2, c_sym ("cons"));
  return c_cons (UPREF (CAR (lst)), UPREF (CAR (CDR (lst))));
}

object_t *lisp_quote (object_t * lst)
{
  DOC ("Return argument unevaluated.");
  REQ (lst, 1, c_sym ("quote"));
  return UPREF (CAR (lst));
}

object_t *lambda_f (object_t * lst)
{
  DOC ("Create an anonymous function.");
  if (!is_func_form (lst))
    THROW (c_sym ("bad-function-form"), UPREF (lst));
  return c_cons (lambda, UPREF (lst));
}

object_t *defun (object_t * lst)
{
  DOC ("Define a new function.");
  if (!SYMBOLP (CAR (lst)) || !is_func_form (CDR (lst)))
    THROW (c_sym ("bad-function-form"), UPREF (lst));
  object_t *f = c_cons (lambda, UPREF (CDR (lst)));
  SET (CAR (lst), f);
  return UPREF (CAR (lst));
}

object_t *defmacro (object_t * lst)
{
  DOC ("Define a new macro.");
  if (!SYMBOLP (CAR (lst)) || !is_func_form (CDR (lst)))
    THROW (c_sym ("bad-function-form"), UPREF (lst));
  object_t *f = c_cons (macro, UPREF (CDR (lst)));
  SET (CAR (lst), f);
  return f;
}

object_t *lisp_cdr (object_t * lst)
{
  DOC ("Return cdr element of cons cell.");
  REQ (lst, 1, c_sym ("cdr"));
  if (CAR (lst) == NIL)
    return NIL;
  if (!LISTP (CAR (lst)))
    THROW (wrong_type, CAR (lst));
  return UPREF (CDR (CAR (lst)));
}

object_t *lisp_car (object_t * lst)
{
  DOC ("Return car element of cons cell.");
  REQ (lst, 1, c_sym ("car"));
  if (CAR (lst) == NIL)
    return NIL;
  if (!LISTP (CAR (lst)))
    THROW (wrong_type, CAR (lst));
  return UPREF (CAR (CAR (lst)));
}

object_t *lisp_list (object_t * lst)
{
  DOC ("Return arguments as a list.");
  return UPREF (lst);
}

object_t *lisp_if (object_t * lst)
{
  DOC ("If conditional special form.");
  REQM (lst, 2, wrong_number_of_arguments);
  object_t *r = eval (CAR (lst));
  CHECK (r);
  if (r != NIL)
    {
      obj_destroy (r);
      return eval (CAR (CDR (lst)));
    }
  return eval_body (CDR (CDR (lst)));
}

object_t *lisp_cond (object_t * lst)
{
  DOC ("Eval car of each argument until one is true. Then eval cdr of\n"
       "that argument.");
  object_t *p = lst;
  while (p != NIL)
    {
      if (!CONSP (p))
	THROW (improper_list, UPREF (lst));
      object_t *pair = CAR (p);
      if (!CONSP (pair))
	THROW (wrong_type, UPREF (pair));
      if (!LISTP (CDR (pair)))
	THROW (improper_list, UPREF (pair));
      if (CDR (pair) == NIL)
	return UPREF (CAR (pair));
      if (CDR (CDR (pair)) != NIL)
	THROW (c_sym ("bad-form"), UPREF (pair));
      object_t *r = eval (CAR (pair));
      if (r != NIL)
	{
	  obj_destroy (r);
	  return eval (CAR (CDR (pair)));
	}
      p = CDR (p);
    }
  return NIL;
}

object_t *progn (object_t * lst)
{
  DOC ("Eval each argument and return the eval of the last.");
  return eval_body (lst);
}

object_t *let (object_t * lst)
{
  DOC ("Create variable bindings in a new scope, and eval "
       "body in that scope.");
  /* verify structure */
  if (!LISTP (CAR (lst)))
    THROW (c_sym ("bad-let-form"), UPREF (lst));
  object_t *vlist = CAR (lst);
  while (vlist != NIL)
    {
      object_t *p = CAR (vlist);
      if (!LISTP (p))
	THROW (c_sym ("bad-let-form"), UPREF (lst));
      if (!SYMBOLP (CAR (p)))
	THROW (c_sym ("bad-let-form"), UPREF (lst));
      vlist = CDR (vlist);
    }

  object_t *p;
  p = vlist = CAR (lst);
  int cnt = 0;
  while (p != NIL)
    {
      object_t *pair = CAR (p);
      object_t *e = eval (CAR (CDR (pair)));
      if (e == err_symbol)
	{
	  /* Undo scoping */
	  p = vlist;
	  while (cnt)
	    {
	      sympop (CAR (CAR (p)));
	      p = CDR (p);
	      cnt--;
	    }
	  return err_symbol;
	}
      sympush (CAR (pair), e);
      obj_destroy (e);
      p = CDR (p);
      cnt++;
    }
  object_t *r = eval_body (CDR (lst));
  p = vlist;
  while (p != NIL)
    {
      object_t *pair = CAR (p);
      sympop (CAR (pair));
      p = CDR (p);
    }
  return r;
}

object_t *lisp_while (object_t * lst)
{
  DOC ("Continually evaluate body until first argument evals nil.");
  REQM (lst, 1, c_sym ("while"));
  object_t *r = NIL, *cond = CAR (lst), *body = CDR (lst);
  object_t *condr;
  while ((condr = eval (cond)) != NIL)
    {
      obj_destroy (r);
      obj_destroy (condr);
      r = eval_body (body);
    }
  return r;
}

/* Equality */

object_t *eq (object_t * lst)
{
  DOC ("Return t if both arguments are the same lisp object.");
  REQ (lst, 2, c_sym ("eq"));
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  if (a == b)
    return T;
  return NIL;
}

object_t *eql (object_t * lst)
{
  DOC ("Return t if both arguments are similar.");
  REQ (lst, 2, c_sym ("eql"));
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  if (a->type != b->type)
    return NIL;
  switch (a->type)
    {
    case INT:
    case FLOAT:
      return num_eq (lst);
      break;
    case SYMBOL:
    case CONS:
      if (a == b)
	return T;
      break;
    case STRING:
      if (OSTRLEN (a) == OSTRLEN (b))
	if (memcmp (OSTR (a), OSTR (b), OSTRLEN (a)) == 0)
	  return T;
      break;
    case VECTOR:
      return NIL;
      break;
    case CFUNC:
    case SPECIAL:
      if (FVAL (a) == FVAL (b))
	return T;
      break;
    }
  return NIL;
}

object_t *lisp_hash (object_t * lst)
{
  DOC ("Return integer hash of object.");
  REQ (lst, 1, c_sym ("hash"));
  return c_int (obj_hash (CAR (lst)));
}

object_t *lisp_print (object_t * lst)
{
  DOC ("Print object or sexp in parse-able form.");
  REQ (lst, 1, c_sym ("print"));
  obj_print (CAR (lst), 1);
  return NIL;
}

/* Symbol table */

object_t *lisp_set (object_t * lst)
{
  DOC ("Store object in symbol.");
  REQ (lst, 2, c_sym ("set"));
  if (!SYMBOLP (CAR (lst)))
    THROW (wrong_type, c_cons (c_sym ("set"), CAR (lst)));
  if (CONSTANTP (CAR (lst)))
    THROW (c_sym ("setting-constant"), CAR (lst));

  SET (CAR (lst), CAR (CDR (lst)));
  return UPREF (CAR (CDR (lst)));
}

object_t *lisp_value (object_t * lst)
{
  DOC ("Get value stored in symbol.");
  REQ (lst, 1, c_sym ("value"));
  if (!SYMBOLP (CAR (lst)))
    THROW (wrong_type, c_cons (c_sym ("value"), CAR (lst)));

  return UPREF (GET (CAR (lst)));
}

object_t *symbol_name (object_t * lst)
{
  DOC ("Return symbol name as string.");
  REQ (lst, 1, c_sym ("symbol-name"));
  if (!SYMBOLP (CAR (lst)))
    THROW (wrong_type, UPREF (CAR (lst)));
  return c_strs (xstrdup (SYMNAME (CAR (lst))));
}

/* String */

object_t *lisp_concat (object_t * lst)
{
  DOC ("Concatenate two strings.");
  REQ (lst, 2, c_sym ("concat"));
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  if (!STRINGP (a))
    THROW (wrong_type, UPREF (a));
  if (!STRINGP (b))
    THROW (wrong_type, UPREF (b));
  return str_cat (a, b);
}

/* Predicates */

object_t *nullp (object_t * lst)
{
  DOC ("Return t if object is nil.");
  REQ (lst, 1, c_sym ("nullp"));
  if (CAR (lst) == NIL)
    return T;
  return NIL;
}

object_t *funcp (object_t * lst)
{
  DOC ("Return t if object is a function.");
  REQ (lst, 1, c_sym ("funcp"));
  if (FUNCP (CAR (lst)))
    return T;
  return NIL;
}

object_t *listp (object_t * lst)
{
  DOC ("Return t if object is a list.");
  REQ (lst, 1, c_sym ("listp"));
  if (LISTP (CAR (lst)))
    return T;
  return NIL;
}

object_t *symbolp (object_t * lst)
{
  DOC ("Return t if object is a symbol.");
  REQ (lst, 1, c_sym ("symbolp"));
  if (SYMBOLP (CAR (lst)))
    return T;
  return NIL;
}

object_t *numberp (object_t * lst)
{
  DOC ("Return t if object is a number.");
  REQ (lst, 1, c_sym ("numberp"));
  if (NUMP (CAR (lst)))
    return T;
  return NIL;
}

object_t *stringp (object_t * lst)
{
  DOC ("Return t if object is a string.");
  REQ (lst, 1, c_sym ("stringp"));
  if (STRINGP (CAR (lst)))
    return T;
  return NIL;
}

object_t *integerp (object_t * lst)
{
  DOC ("Return t if object is an integer.");
  REQ (lst, 1, c_sym ("integerp"));
  if (INTP (CAR (lst)))
    return T;
  return NIL;
}

object_t *floatp (object_t * lst)
{
  DOC ("Return t if object is a floating-point number.");
  REQ (lst, 1, c_sym ("floatp"));
  if (FLOATP (CAR (lst)))
    return T;
  return NIL;
}

object_t *vectorp (object_t * lst)
{
  DOC ("Return t if object is a vector.");
  REQ (lst, 1, c_sym ("vectorp"));
  if (VECTORP (CAR (lst)))
    return T;
  return NIL;
}

/* Input/Output */

object_t *lisp_load (object_t * lst)
{
  DOC ("Evaluate contents of a file.");
  REQ (lst, 1, c_sym ("load"));
  object_t *str = CAR (lst);
  if (!STRINGP (str))
    THROW (wrong_type, UPREF (str));
  char *filename = OSTR (str);
  int r = load_file (NULL, filename, 0);
  if (!r)
    THROW (c_sym ("load-file-error"), UPREF (str));
  return T;
}

object_t *lisp_read_string (object_t * lst)
{
  DOC ("Parse a string into a sexp or list object.");
  REQ (lst, 1, c_sym ("eval-string"));
  object_t *stro = CAR (lst);
  if (!STRINGP (stro))
    THROW (wrong_type, UPREF (stro));
  char *str = OSTR (stro);
  reader_t *r = reader_create (NULL, str, "eval-string", 0);
  object_t *sexp = read_sexp (r);
  reader_destroy (r);
  if (sexp == err_symbol)
    THROW (c_sym ("parse-error"), UPREF (stro));
  return sexp;
}

/* Error handling */

object_t *throw (object_t * lst)
{
  DOC ("Throw an object, and attachment, as an exception.");
  THROW (UPREF (CAR (lst)), UPREF (CAR (CDR (lst))));
}

object_t *catch (object_t * lst)
{
  DOC ("Catch an exception and return attachment.");
  object_t *csym = eval (CAR (lst));
  CHECK (csym);
  object_t *body = CDR (lst);
  object_t *r = eval_body (body);
  if (r == err_symbol)
    {
      if (csym == err_thrown)
	{
	  obj_destroy (csym);
	  obj_destroy (err_thrown);
	  return err_attach;
	}
      else
	return err_symbol;
    }
  return r;
}

/* Vectors */

object_t *lisp_vset (object_t * lst)
{
  DOC ("Set slot in a vector to object.");
  REQ (lst, 3, c_sym ("vset"));
  object_t *vec = CAR (lst);
  object_t *ind = CAR (CDR (lst));
  object_t *val = CAR (CDR (CDR (lst)));
  if (!VECTORP (vec))
    THROW (wrong_type, UPREF (vec));
  if (!INTP (ind))
    THROW (wrong_type, UPREF (ind));
  return vset_check (vec, ind, val);
}

object_t *lisp_vget (object_t * lst)
{
  DOC ("Get object stored in vector slot.");
  REQ (lst, 2, c_sym ("vget"));
  object_t *vec = CAR (lst);
  object_t *ind = CAR (CDR (lst));
  if (!VECTORP (vec))
    THROW (wrong_type, UPREF (vec));
  if (!INTP (ind))
    THROW (wrong_type, UPREF (ind));
  return vget_check (vec, ind);
}

object_t *lisp_vlength (object_t * lst)
{
  DOC ("Return length of the vector.");
  REQ (lst, 1, c_sym ("vlength"));
  object_t *vec = CAR (lst);
  if (!VECTORP (vec))
    THROW (wrong_type, UPREF (vec));
  return c_int (VLENGTH (vec));
}

object_t *make_vector (object_t * lst)
{
  DOC ("Make a new vector of given length, initialized to given object.");
  REQ (lst, 2, c_sym ("make-vector"));
  object_t *len = CAR (lst);
  object_t *o = CAR (CDR (lst));
  if (!INTP (len))
    THROW (wrong_type, UPREF (len));
  return c_vec (into2int (len), o);
}

object_t *lisp_vconcat (object_t * lst)
{
  DOC ("Concatenate two vectors.");
  REQ (lst, 2, c_sym ("vconcat2"));
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  if (!VECTORP (a))
    THROW (wrong_type, UPREF (a));
  if (!VECTORP (b))
    THROW (wrong_type, UPREF (b));
  return vector_concat (a, b);
}

object_t *lisp_vsub (object_t * lst)
{
  DOC ("Return subsection of vector.");
  REQM (lst, 2, c_sym ("subv"));
  object_t *v = CAR (lst);
  object_t *starto = CAR (CDR (lst));
  if (!VECTORP (v))
    THROW (wrong_type, UPREF (v));
  if (!INTP (starto))
    THROW (wrong_type, UPREF (starto));
  int start = into2int (starto);
  if (start >= (int) VLENGTH (v))
    THROW (c_sym ("bad-index"), UPREF (starto));
  if (start < 0)
    THROW (c_sym ("bad-index"), UPREF (starto));
  if (CDR (CDR (lst)) == NIL)
    {
      /* to the end */
      return vector_sub (v, start, -1);
    }
  object_t *endo = CAR (CDR (CDR (lst)));
  if (!INTP (endo))
    THROW (wrong_type, UPREF (endo));
  int end = into2int (endo);
  if (end >= (int) VLENGTH (v))
    THROW (c_sym ("bad-index"), UPREF (endo));
  if (end < start)
    THROW (c_sym ("bad-index"), UPREF (endo));
  return vector_sub (v, start, end);
}

/* Internals */

object_t *lisp_refcount (object_t * lst)
{
  DOC ("Return number of reference counts to object.");
  REQ (lst, 1, c_sym ("refcount"));
  return c_int (CAR (lst)->refs);
}

object_t *lisp_eval_depth (object_t * lst)
{
  DOC ("Return the current evaluation depth.");
  REQ (lst, 0, c_sym ("eval-depth"));
  return c_int (stack_depth);
}

object_t *lisp_max_eval_depth (object_t * lst)
{
  DOC ("Return or set the maximum evaluation depth.");
  REQX (lst, 1, c_sym ("max-eval-depth"));
  if (lst == NIL)
    return c_int (max_stack_depth);
  object_t *arg = CAR (lst);
  if (!INTP (arg))
    THROW (wrong_type, UPREF (arg));
  int new_depth = into2int (arg);
  if (new_depth < 10)
    return NIL;
  max_stack_depth = new_depth;
  return UPREF (arg);
}

/* System */

object_t *lisp_exit (object_t * lst)
{
  DOC ("Halt the interpreter and return given integer.");
  REQX (lst, 1, c_sym ("exit"));
  if (lst == NIL)
    exit (EXIT_SUCCESS);
  if (!INTP (CAR (lst)))
    THROW (wrong_type, UPREF (CAR (lst)));
  exit (into2int (CAR (lst)));
}

/* Installs all of the above functions. */
void lisp_init ()
{
  /* Maths */
  lisp_math_init ();

  /* Various */
  SSET (c_sym ("cdoc-string"), c_cfunc (&cdoc_string));
  SSET (c_sym ("apply"), c_cfunc (&lisp_apply));
  SSET (c_sym ("and"), c_special (&lisp_and));
  SSET (c_sym ("or"), c_special (&lisp_or));
  SSET (c_sym ("quote"), c_special (&lisp_quote));
  SSET (c_sym ("lambda"), c_special (&lambda_f));
  SSET (c_sym ("defun"), c_special (&defun));
  SSET (c_sym ("defmacro"), c_special (&defmacro));
  SSET (c_sym ("car"), c_cfunc (&lisp_car));
  SSET (c_sym ("cdr"), c_cfunc (&lisp_cdr));
  SSET (c_sym ("list"), c_cfunc (&lisp_list));
  SSET (c_sym ("if"), c_special (&lisp_if));
  SSET (c_sym ("not"), c_cfunc (&nullp));
  SSET (c_sym ("progn"), c_special (&progn));
  SSET (c_sym ("let"), c_special (&let));
  SSET (c_sym ("while"), c_special (&lisp_while));
  SSET (c_sym ("eval"), c_cfunc (&eval_body));
  SSET (c_sym ("print"), c_cfunc (&lisp_print));
  SSET (c_sym ("cons"), c_cfunc (&lisp_cons));
  SSET (c_sym ("cond"), c_special (&lisp_cond));

  /* Symbol table */
  SSET (c_sym ("set"), c_cfunc (&lisp_set));
  SSET (c_sym ("value"), c_cfunc (&lisp_value));
  SSET (c_sym ("symbol-name"), c_cfunc (&symbol_name));

  /* Strings */
  SSET (c_sym ("concat2"), c_cfunc (&lisp_concat));

  /* Equality */
  SSET (c_sym ("eq"), c_cfunc (&eq));
  SSET (c_sym ("eql"), c_cfunc (&eql));
  SSET (c_sym ("hash"), c_cfunc (&lisp_hash));

  /* Predicates */
  SSET (c_sym ("nullp"), c_cfunc (&nullp));
  SSET (c_sym ("funcp"), c_cfunc (&funcp));
  SSET (c_sym ("listp"), c_cfunc (&listp));
  SSET (c_sym ("symbolp"), c_cfunc (&symbolp));
  SSET (c_sym ("stringp"), c_cfunc (&stringp));
  SSET (c_sym ("numberp"), c_cfunc (&numberp));
  SSET (c_sym ("integerp"), c_cfunc (&integerp));
  SSET (c_sym ("floatp"), c_cfunc (&floatp));
  SSET (c_sym ("vectorp"), c_cfunc (&vectorp));

  /* Input/Output */
  SSET (c_sym ("load"), c_cfunc (&lisp_load));
  SSET (c_sym ("read-string"), c_cfunc (&lisp_read_string));

  /* Error handling */
  SSET (c_sym ("throw"), c_cfunc (&throw));
  SSET (c_sym ("catch"), c_special (&catch));

  /* Vectors */
  SSET (c_sym ("vset"), c_cfunc (&lisp_vset));
  SSET (c_sym ("vget"), c_cfunc (&lisp_vget));
  SSET (c_sym ("vlength"), c_cfunc (&lisp_vlength));
  SSET (c_sym ("make-vector"), c_cfunc (&make_vector));
  SSET (c_sym ("vconcat2"), c_cfunc (&lisp_vconcat));
  SSET (c_sym ("vsub"), c_cfunc (&lisp_vsub));

  /* Internals */
  SSET (c_sym ("refcount"), c_cfunc (&lisp_refcount));
  SSET (c_sym ("eval-depth"), c_cfunc (&lisp_eval_depth));
  SSET (c_sym ("max-eval-depth"), c_cfunc (&lisp_max_eval_depth));

  /* System */
  SSET (c_sym ("exit"), c_cfunc (&lisp_exit));
}
