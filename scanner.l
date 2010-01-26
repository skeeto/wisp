/* scanner.l - lexer for use with generated parser */
%{
#include <stdio.h>
#include <string.h>
#include "wisp.h"
#include "y.tab.h"

int fileno(FILE *);
void count_line ();
%}

%option always-interactive
%%

^#!.+$                      /* ignore shebang-looking things */

 /* handle strings (non-numbers) */
\"(\\.|[^\"])*\"       { yylval.str = c_str ( process_str (yytext));
                                        return TOK_STR; }

 /* handle integers and floats */
-?[0-9]+\.[0-9]+            { yylval.f = c_float (strtod (yytext, NULL));
                                           return TOK_FLOAT; }
-?[0-9]+                    { yylval.i = c_int (atoi (yytext));
                                           return TOK_INT; }

 /* symbols */
[a-zA-Z0-9!@#$%^&*_\-=+:;<>/\\|?~]+  {
                               yylval.sym = c_sym (yytext);
                                           return TOK_SYMBOL; }

 /* parens */
\(                          return LP;
\)                          return RP;

 /* ignore comments */
;[^\n]*\n                   count_line ();

 /* line counter */
\n                          count_line ();

 /* leftover characters */
[\t\r ]+                    /* ignore whitespace */
.                           return (int) yytext[0];

%%

void empty ()
{
  (void) yyunput;
  (void) input;
}
