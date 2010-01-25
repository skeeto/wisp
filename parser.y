/* parser.y - parser generator for use with yacc (probably bison) */
%{
#include <stdio.h>
#include "wisp.h"
#include "common.h"

/* declarations */
extern FILE *yyin;
int yylex ();
void yyerror (char *);
void print_prompt ();
extern int line_num;
char *filename = "<unknown>";
int line_num;
int interactive;
char *prompt = "wisp> ";

int yywrap ()
{
  return 1;
}

size_t ssize;
object_t **base;
object_t **tip;

static void push ();
static object_t *pop ();
static void add_obj (object_t * o);
static object_t *list_to_cons (object_t * lst);
%}
%defines

%union {
  object_t *str;
  object_t *i;
  object_t *f;
  object_t *sym;
}

%token <str> TOK_STR
%token <i> TOK_INT
%token <f> TOK_FLOAT
%token <sym> TOK_SYMBOL
%token LP
%token RP

%%

input : /* empty */
     | input exp
     | error errrec { yyerrok; }
;

/* Error recovery */
errrec : error
     | exp
;

exp : sexp   { object_t *sexp = pop ();
               object_t *r = top_eval (CAR (sexp));
               if (interactive)
		 {
		   obj_print (r, 1);
		   print_prompt ();
		 }
	       obj_destroy (r);
	       obj_destroy (sexp);
               push (); }
;

sexp : atom
     | lp series rp         { add_obj (pop ()); }
     | lp sexp '.' sexp rp  { add_obj (list_to_cons (pop ())); }
;

series : sexp series
     | /* empty */
;

atom : TOK_STR            { add_obj(yylval.str); }
     | TOK_INT            { add_obj(yylval.i); }
     | TOK_FLOAT          { add_obj(yylval.f); }
     | TOK_SYMBOL         { add_obj(yylval.sym); }
;

lp : LP                   { push (); }
;

rp : RP
;

%%

void parser_init ()
{
  ssize = 32;
  tip = base = xmalloc (sizeof (object_t *) * ssize * 2);
  push ();
}

int parse (FILE *fid, char *name, int inter)
{
  interactive = inter;
  filename = name;
  yyin = fid;
  print_prompt ();
  line_num = 1;
  int r = yyparse ();
  if (interactive)
    printf ("\n");
  return r;
}

static void push ()
{
  tip += 2;
  if (tip == base + ssize * 2)
    {
      ssize *= 2;
      base = xrealloc (base, sizeof (object_t *) * ssize * 2);
      tip = base + ssize;
    }
  *tip = c_cons (NIL, NIL);
  *(tip + 1) = *tip;
}

static object_t *pop ()
{
  tip -= 2;
  return CDR (*(tip + 2));
}

static object_t *list_to_cons (object_t * lst)
{
  return c_cons (CAR (lst), CAR (CDR (lst)));
}

static void add_obj (object_t * o)
{
  CDR (*(tip + 1)) = c_cons (o, NIL);
  *(tip + 1) = CDR (*(tip + 1));
}

void yyerror (char *s)
{
  printf ("%s:%d: error: %s\n", filename, line_num, s);
  print_prompt ();
}

void print_prompt ()
{
  if (interactive)
    printf ("%s", prompt);
}

void count_line ()
{
  line_num++;
}
