IDENTIFIER [a-zA-Z]
INTEGER [0-9]
NONZEROINTEGER [1-9]
ZERO [0]
%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

%%
((G|g)(E|e)(T|t)) {return GET;}
((I|i)(N|n)(S|s)(E|e)(R|r)(T|t)) {return INSERT;}
((U|u)(P|p)(D|d)(A|a)(T|t)(E|e)) return UPDATE;
((D|d)(E|e)(L|l)(E|e)(T|t)(E|e)) return DELETE;
((F|f)(R|r)(O|o)(M|m)) return FROM;
((A|a)(N|n)(D|d)) {yylval.sval = (char *)malloc(sizeof(strlen(yytext))); yylval.sval=strdup(yytext); return AND;}
((O|o)(R|r)) {yylval.sval = (char *)malloc(sizeof(strlen(yytext))); yylval.sval=strdup(yytext); return OR;}
((W|w)(H|h)(E|e)(R|r)(E|e)) {return WHERE;}
((R|r)(E|e)(C|c)(O|o)(R|r)(D|d)) {return RECORD;}
((I|i)(N|n)(T|t)(O|o)) {return INTO;}
((S|s)(E|e)(T|t)) {return SET;}
((I|i)(N|n)) {return IN;}
((T|t)(O|o)) {return TO;}

[ ]* ;

(>|<|!)(=) {yylval.sval = (char *)malloc(sizeof(strlen(yytext))); yylval.sval=strdup(yytext);  return COMPARATOR;}
(>|<|=) {yylval.sval = (char *)malloc(sizeof(strlen(yytext))); yylval.sval=strdup(yytext);  return COMPARATOR;}


{IDENTIFIER}+\.(txt) {yylval.sval = (char *)malloc(sizeof(strlen(yytext))); yylval.sval=strdup(yytext);   return FILE_NAME;}

["]{IDENTIFIER}+["] {yylval.sval = (char *)malloc(sizeof(strlen(yytext))); yylval.sval=strdup(yytext);return STRING;}

{IDENTIFIER}+ {yylval.sval = (char *)malloc(sizeof(strlen(yytext))); yylval.sval=strdup(yytext);return FIELD_STRING;}

({ZERO}|({NONZEROINTEGER})({INTEGER}+)) {yylval.sval = (char *)malloc(sizeof(strlen(yytext))); yylval.sval=strdup(yytext);return INTEGER;}

[(] return LEFT;
[)] return RIGHT;
[,] return COMMA;
\n return Nl;
";" return SEMI_COLON;
%%

extern int yyparse();

int main()
{
    printf("Enter Your Expression:");
  	yyparse();
    yylex_destroy();
    return 0;
}