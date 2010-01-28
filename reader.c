#include <stdio.h>
#include <string.h>
#include "object.h"
#include "symtab.h"
#include "common.h"
#include "cons.h"
#include "eval.h"
#include "str.h"
#include "reader.h"

char *atom_chars =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
  "0123456789!#$%^&*-_=+|\\/?.~";
char *prompt = "wisp> ";

reader_t *reader_create (FILE * fid, char *str, char *name, int interactive)
{
  reader_t *r = xmalloc (sizeof (reader_t));
  r->fid = fid;
  r->strp = r->str = str;
  r->name = name ? name : "<unknown>";
  r->interactive = interactive;
  r->prompt = prompt;
  r->linecnt = 1;
  r->eof = 0;
  r->error = 0;
  r->shebang = -1 + interactive;

  r->buflen = 1024;
  r->bufp = r->buf = xmalloc (r->buflen + 1);

  r->readbuflen = 8;
  r->readbufp = r->readbuf = xmalloc (r->readbuflen * sizeof (int));

  r->qstacklen = 4;
  r->qstackp = r->qstack = xmalloc (r->qstacklen * sizeof (int));
  *(r->qstackp) = -1;

  r->ssize = 32;
  r->tip = r->base = xmalloc (32 * sizeof (object_t *) * 2);
  return r;
}

void reader_destroy (reader_t * r)
{
  free (r->base);
  free (r);
}

/* Read next character in the stream. */
static int reader_getc (reader_t * r)
{
  int c;
  if (r->readbufp > r->readbuf)
    {
      c = *(r->readbufp);
      r->readbufp--;
      return c;
    }
  if (r->str != NULL)
    {
      c = *(r->strp);
      if (c != '\0')
	r->strp++;
      else
	return EOF;
    }
  else
    c = fgetc (r->fid);
  return c;
}

/* Unread a byte. */
static void reader_putc (reader_t * r, int c)
{
  r->readbufp++;
  if (r->readbufp == r->readbuf + r->readbuflen)
    {
      r->readbuflen *= 2;
      r->readbuf = xrealloc (r->readbuf, sizeof (int) * r->readbuflen);
      r->readbufp = r->readbuf + r->readbuflen / 2;
    }
  *(r->readbufp) = c;
}

/* Consume remaining whitespace on line, including linefeed. */
static void consume_whitespace (reader_t * r)
{
  int c = reader_getc (r);
  while (strchr (" \t\r", c) != NULL)
    c = reader_getc (r);
  if (c != '\n')
    reader_putc (r, c);
}

/* Consume remaining characters on line, including linefeed. */
static void consume_line (reader_t * r)
{
  int c = reader_getc (r);
  while (c != '\n')
    c = reader_getc (r);
  if (c != '\n')
    reader_putc (r, c);
}

/* Push new list onto the sexp stack. */
static void push (reader_t * r)
{
  r->tip += 2;
  if (r->tip == r->base + r->ssize * 2)
    {
      r->ssize *= 2;
      r->base = xrealloc (r->base, sizeof (object_t *) * r->ssize * 2);
      r->tip = r->base + r->ssize;
    }
  *(r->tip) = c_cons (NIL, NIL);
  *(r->tip + 1) = *(r->tip);
}

/* Remove top object from the sexp stack. */
static object_t *pop (reader_t * r)
{
  object_t *p = CDR (*(r->tip));
  CDR (*(r->tip)) = NIL;
  obj_destroy (*(r->tip));
  r->tip -= 2;
  return p;
}

/* Remove top object from the sexp stack. */
static void reset (reader_t * r)
{
  while (r->tip != r->base)
    obj_destroy (pop (r));
  r->qstackp = r->qstack;
  *(r->qstackp) = -1;
  r->bufp = r->buf;
  r->readbufp = r->readbuf;
}

/* Print an error message. */
static void read_error (reader_t * r, char *str)
{
  fprintf (stderr, "%s:%d: %s\n", r->name, r->linecnt, str);
  consume_line (r);
  reset (r);
  r->error = 1;
}

/* Return height of sexp stack. */
static size_t stack_height (reader_t * r)
{
  return (r->tip - r->base) / 2;
}

/* Determine if top list is empty. */
static int list_empty (reader_t * r)
{
  return CDR (*(r->tip)) == NIL;
}

static void print_prompt (reader_t * r)
{
  if (r->interactive && stack_height (r) == 1)
    printf ("%s", r->prompt);
}

/* Push a new object into the current list. */
static void add (reader_t * r, object_t * o)
{
  CDR (*(r->tip + 1)) = c_cons (o, NIL);
  *(r->tip + 1) = CDR (*(r->tip + 1));
}

/* Append character to buffer. */
static void buf_append (reader_t * r, char c)
{
  if (r->bufp == r->buf + r->buflen)
    {
      r->buflen *= 2;
      r->buf = xrealloc (r->buf, r->buflen + 1);
      r->bufp = r->buf + r->buflen / 2;
    }
  *(r->bufp) = c;
  *(r->bufp + 1) = '\0';
  r->bufp++;
}

/* Load into buffer until character, ignoring escaped ones. */
static int buf_read (reader_t * r, char *halt)
{
  int esc = 0;
  int c = reader_getc (r);
  esc = 0;
  if (c == '\\')
    {
      c = reader_getc (r);
      esc = 1;
    }
  while ((esc || strchr (halt, c) == NULL) && (c != EOF))
    {
      buf_append (r, c);
      c = reader_getc (r);
      esc = 0;
      if (c == '\\')
	{
	  c = reader_getc (r);
	  esc = 1;
	}
    }
  reader_putc (r, c);
  return !esc;
}

/* Turn string in buffer into string object. */
static object_t *parse_str (reader_t * r)
{
  /* process_str? */
  r->bufp = r->buf;
  return c_strs ( xstrdup (r->buf));
}

/* Turn string in buffer into atom object. */
static object_t *parse_atom (reader_t * r)
{
  char *str = r->buf;
  char *end;

  /* Detect integer */
  int i = strtol (str, &end, 10);
  if (end != str && *end == '\0')
    {
      r->bufp = r->buf;
      return c_int (i);
    }

  /* Detect float */
  double f = strtod (str, &end);
  if (end != str && *end == '\0')
    {
      r->bufp = r->buf;
      return c_float (f);
    }

  /* Might be a symbol then */
  char *p = r->buf;
  while (p <= r->bufp)
    {
      if (strchr (atom_chars, *p) == NULL)
	{
	  char *errstr = xstrdup ("invalid symbol character: X");
	  errstr[strlen (errstr) - 1] = *p;
	  read_error (r, errstr);
	  free (errstr);
	  return NIL;
	}
      p++;
    }
  r->bufp = r->buf;
  return c_sym (r->buf);
}

/* Increase quote depth */
static void up_quote (reader_t * r)
{
  if (*(r->qstackp) >= 0)
    (*(r->qstackp))++;
}

/* Add quote to quote stack. */
static void add_quote (reader_t * r)
{
  up_quote (r);
  r->qstackp++;
  if (r->qstackp == r->qstack + r->qstacklen)
    {
      r->qstacklen *= 2;
      r->qstack = xrealloc (r->qstack, r->qstacklen * sizeof (int));
      r->qstackp = r->qstack + r->qstacklen / 2;
    }
  *(r->qstackp) = 0;
  push (r);
  add (r, quote);
}

/* Decrease quote depth */
static void down_quote (reader_t * r)
{
  if (*(r->qstackp) >= 0)
    (*(r->qstackp))--;
}

/* Height of qstack */
static int qstack_height (reader_t * r)
{
  return r->qstackp - r->qstack;
}

/* Check if quote needs to be popped. */
static void check_quote (reader_t * r)
{
  if (*(r->qstackp) < 0)
    return;
  if (*(r->qstackp) == 0)
    {
      if (stack_height (r) <= 1)
	read_error (r, "unbalanced parenthesis");
      else
	{
	  add (r, pop (r));
	  r->qstackp--;
	  down_quote (r);
	  check_quote (r);
	}
    }
}

/* Read a single sexp from the reader. */
object_t *read_sexp (reader_t * r)
{
  /* Check for a shebang line. */
  if (r->shebang == -1)
    {
      char str[2];
      str[0] = reader_getc (r);
      str[1] = reader_getc (r);
      if (str[0] == '#' && str[1] == '!')
	{
	  /* Looks like a she-bang line. */
	  r->shebang = 1;
	  consume_line (r);
	}
      else
	{
	  r->shebang = 0;
	  reader_putc (r, str[1]);
	  reader_putc (r, str[0]);
	}
    }

  r->error = 0;
  push (r);
  print_prompt (r);
  while (!r->eof && !r->error && (list_empty (r) || stack_height (r) > 1))
    {
      int c = reader_getc (r);
      switch (c)
	{
	case EOF:
	  r->eof = 1;
	  break;

	  /* Comments */
	case ';':
	  consume_line (r);
	  break;

	  /* Whitespace */
	case '\n':
	  r->linecnt++;
	  print_prompt (r);
	case ' ':
	case '\t':
	case '\r':
	  break;

	  /* Parenthesis */
	case '(':
	  push (r);
	  up_quote (r);
	  break;
	case ')':
	  if (stack_height (r) <= 1 || *(r->qstackp) == 0)
	    read_error (r, "unbalanced parenthesis");
	  else
	    {
	      add (r, pop (r));
	      down_quote (r);
	      check_quote (r);
	    }
	  break;

	  /* Quoting */
	case '\'':
	  add_quote (r);
	  break;

	  /* strings */
	case '"':
	  buf_read (r, "\"");
	  add (r, parse_str (r));
	  check_quote (r);
	  reader_getc (r);	/* Throw away other quote. */
	  break;

	  /* numbers and symbols */
	default:
	  buf_append (r, c);
	  buf_read (r, " \t\r\n();");
	  object_t *o = parse_atom (r);
	  if (!r->error)
	    {
	      add (r, o);
	      check_quote (r);
	    }
	  break;
	}
    }
  if (!r->eof && !r->error)
    consume_whitespace (r);
  if (r->error)
    return err_symbol;

  /* Check state */
  if (stack_height (r) > 1 || qstack_height (r) > 1)
    {
      read_error (r, "premature end of file");
      return err_symbol;
    }
  if (list_empty (r))
    return NIL;

  object_t *wrap = pop (r);
  object_t *sexp = UPREF (CAR (wrap));
  obj_destroy (wrap);
  return sexp;
}

/* Use the core functions above to eval each sexp in a file. */
int load_file (FILE * fid, char *filename, int interactive)
{
  if (fid == NULL)
    {
      fid = fopen (filename, "r");
      if (fid == NULL)
	return 0;
    }
  reader_t *r = reader_create (fid, NULL, filename, interactive);
  while (!r->eof)
    {
      object_t *sexp = read_sexp (r);
      if (sexp != err_symbol)
	{
	  object_t *ret = top_eval (sexp);
	  if (r->interactive)
	    obj_print (ret, 1);
	  obj_destroy (ret);
	}
    }
  return 1;
}
