#!/bin/bash
yacc -d mql_compiler.y
lex mql_compiler.l
gcc y.tab.c lex.yy.c -ly -ll
./a.out