%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

int fileno(FILE *);
int line_num;			/* line counter */
%}

%%

 /* handle strings (non-numbers) */
\"([^\"\\]*((\\\")|(\\\\))*)*\"        yylval.str = yytext; return TOK_STR;

 /* handle integers and floats */
-?[0-9]+\.[0-9]+            yylval.f   = strtod(yytext, NULL); return TOK_FLOAT;
-?[0-9]+                    yylval.i   = atoi(yytext); return TOK_INT;

 /* ignore comments */
;[^\n]*\n                   line_num++;

 /* line counter */
\n                          line_num++;

 /* leftover characters */
[\t\r ]+                    /* ignore whitespace */
.                           return (int) yytext[0];

%%

void empty ()
{
  (void) yyunput;
  (void) input;
}
