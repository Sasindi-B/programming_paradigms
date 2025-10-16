%{
#include "testlang.tab.h"
#include <stdio.h>
#include <string.h>
%}

%%
"GET"       { return GET; }
"POST"      { return POST; }
"ASSERT"    { return ASSERT; }
\"[^\"]*\"  { yylval.str = strdup(yytext); return STRING; }
[ \t\r\n]+  ;
.           ;
%%

int yywrap() { return 1; }
