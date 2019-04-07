#!/bin/bash
yacc -d methastruct.y
lex mql_compiler.l
gcc y.tab.c lex.yy.c -ly -ll
./a.out