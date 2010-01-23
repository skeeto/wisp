%{
#include <stdio.h>

/* lex declarations */
extern FILE *yyin;
int yylex ();
int yyerror (char *);
extern int line_num;

int yywrap ()
{
  return 1;
}

%}
%defines

%union {
  char *str;
  int i;
  double f;
}

%token <str> TOK_STR
%token <i> TOK_INT
%token <f> TOK_FLOAT

%%

input : /* empty */
     | input exp
;

exp : sexp   { printf("SEXP\n"); }
;

sexp : atom                   { printf("see atom\n"); }
     | '(' series ')'         { printf("see (s)\n"); }
     | '(' sexp '.' sexp ')'  { printf("see (s . s)\n"); }
;

series : sexp series
     | /* empty */
;

atom : TOK_STR            { printf("see STR\n"); }
     | TOK_INT            { printf("see INT\n"); }
     | TOK_FLOAT          { printf("see FLOAT\n"); }
;

%%

int yyerror (char *s)
{
  printf ("wisp:%d: error: %s\n", line_num, s);
  return 0;
}
