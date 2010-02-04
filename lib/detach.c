#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "object.h"
#include "symtab.h"
#include "eval.h"
#include "reader.h"
#include "detach.h"

object_t *parent_detach;

uint8_t detach_hash (object_t * o)
{
  pid_t proc = OPROC (o);
  return hash (&proc, sizeof (pid_t));
}

void detach_print (object_t * o)
{
  pid_t proc = OPROC (o);
  printf ("<detach %d>", proc);
}

object_t *c_detach (object_t * o)
{
  if (SYMBOLP (o))
    o = GET (o);
  object_t *dob = obj_create (DETACH);
  detach_t *d = OVAL (dob);
  int pipea[2], pipeb[2];
  if (pipe (pipea) != 0)
    THROW (c_sym ("detach-pipe-error"), c_strs (xstrdup (strerror (errno))));
  if (pipe (pipeb) != 0)
    THROW (c_sym ("detach-pipe-error"), c_strs (xstrdup (strerror (errno))));
  d->proc = fork ();
  if (d->proc == 0)
    {
      /* Child process */

      /* Set up pipes */
      d->in = pipeb[0];
      d->out = pipea[1];
      close (pipeb[1]);
      close (pipea[0]);

      /* Change stdin and stdout. */
      fclose (stdin);
      int stdout_no = fileno (stdout);
      close (stdout_no);
      dup2 (d->out, stdout_no);
      d->read = reader_create (fdopen (d->in, "r"), NULL, "parent", 0);
      parent_detach = dob;

      /* Execute given function. */
      object_t *f = c_cons (o, NIL);
      eval (f);
      exit (0);
      THROW (c_sym ("exit-failed"), dob);
    }
  /* Parent process */
  d->in = pipea[0];
  d->out = pipeb[1];
  close (pipea[1]);
  close (pipeb[0]);
  d->read = reader_create (fdopen (d->in, "r"), NULL, "detach", 0);
  return dob;
}

detach_t *detach_create ()
{
  return (detach_t *) xmalloc (sizeof (detach_t));
}

void detach_destroy (object_t * o)
{
  detach_t *d = OVAL (o);
  reader_destroy (d->read);
  close (d->in);
  close (d->out);
}

object_t *lisp_detach (object_t * lst)
{
  DOC ("Create process detachment.");
  REQ (lst, 1, c_sym ("detach"));
  return c_detach (CAR (lst));
}

object_t *lisp_receive (object_t * lst)
{
  DOC ("Get an object from the detached process.");
  REQ (lst, 1, c_sym ("receive"));
  object_t *d = CAR (lst);
  if (!DETACHP (d))
    THROW (wrong_type, UPREF (d));
  reader_t *r = OREAD (d);
  return read_sexp (r);
}

object_t *lisp_send (object_t * lst)
{
  DOC ("Send an object to the parent process.");
  REQ (lst, 1, c_sym ("send"));
  object_t *o = CAR (lst);
  if (parent_detach == NULL || parent_detach == NIL)
    THROW (c_sym ("send-from-non-detachment"), UPREF (o));
  obj_print (o, 1);
  return T;
}
