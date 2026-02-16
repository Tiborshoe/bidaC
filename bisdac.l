%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bisdac.tab.h"
%}
%option yylineno
%%
"numero"            { return NUMERO; }
"ipagawas"           { return IPAGAWAS; }
"litir"             { return LITIR; }
"+="                { return ADDEQ; }
"-="                { return SUBEQ; }
"*="                { return MULEQ; }
"/="                { return DIVEQ; }
[0-9]+              { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return IDEN; }
"+"                 { return ADD; }
"-"                 { return SUB; }
"*"                 { return MUL; }
"/"                 { return DIV; }
"="                 { return EQL; }
";"                 { return END; }
"("                 { return OPEN; }
")"                 { return CLOSE; }
\"[^\"]*\"          { yylval.str = strdup(yytext); return STR; }
\'(\\.|[^\\'])\'    { yylval.str = strdup(yytext); return CHR; }
"<#"([^#]|"#"[^>])*"#>" { /* ignore comments */ }
[ \t\r\n]+          { /* ignore whitespace */ }
.                   { printf("Unknown character: %s\n", yytext); }
%%
int yywrap(void) { return 1; }