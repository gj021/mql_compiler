%{
    #include <stdio.h>
    #include <string.h>
    int yylex(void);
    int yyerror(char *s);
    #include "mql_compiler.h"
%}

%union{
  char *sval;
}

%token Nl TO COMMA INTO INSERT RECORD UPDATE SEMI_COLON GET DELETE FROM SET IN LEFT RIGHT 
%token <sval> STRING;
%token <sval> FIELD_STRING;
%token <sval> AND;
%token <sval> COMPARATOR;
%token <sval> WHERE;
%token <sval> OR;
%token <sval> INTEGER;
%token <sval> FILE_NAME;
%type <sval> file_name;
%type <sval> attributes;
%type <sval> attribute;
%type <sval> fields;
%type <sval> field;
%type <sval> condition;
%type <sval> conditions;

%%
stmt_list : stmt Nl
;
stmt: get_stmt | insert_stmt | delete_stmt | update_stmt
;

update_stmt: UPDATE RECORD IN file_name SET field TO attribute conditions SEMI_COLON {printf("update in %s set %s to %s %s\n",$4 , $6, $8, $9);}
; 
insert_stmt: INSERT RECORD LEFT attributes RIGHT INTO file_name SEMI_COLON {insert($4,$7);}
;
delete_stmt: DELETE RECORD FROM file_name conditions SEMI_COLON {printf("DELETE from %s if %s\n", $4, $5);}
;
get_stmt: GET fields FROM file_name conditions SEMI_COLON {printf("get %s from %s where %s\n", $2, $4, $5);}
;

attributes: STRING COMMA attributes  {strcat($1,",");strcat($1,$3); $$=strdup($1);}
;
attributes: INTEGER COMMA attributes {strcat($1,","); strcat($1,$3);$$=strdup($1);}
; 
attributes: STRING {$$=strdup($1);}
;
attributes: INTEGER {$$=strdup($1);}
;

attribute: FIELD_STRING|INTEGER {$$ = strdup($1);}
;

fields: FIELD_STRING COMMA fields {strcat($1,",");strcat($1,$3);$$=strdup($1);}
;
fields: FIELD_STRING {$$=strdup($1);}
;

field: FIELD_STRING {$$=strdup($1);}
;

conditions: %empty {$$=strdup("");}
;
conditions: WHERE condition{$$=strdup($2);}
;
condition: FIELD_STRING COMPARATOR INTEGER AND condition { strcat($1,$2);strcat($1,$3);strcat($1,$4);strcat($1,$5);$$=strdup($1);}
;
condition: FIELD_STRING COMPARATOR INTEGER OR condition {strcat($1,$2);strcat($1,$3);strcat($1,$4);strcat($1,$5);$$=strdup($1);}
;
condition: FIELD_STRING COMPARATOR INTEGER {strcat($1,$2);strcat($1,$3); $$=strdup($1); }
;

file_name: FILE_NAME {$$=strdup($1);}
;

%%