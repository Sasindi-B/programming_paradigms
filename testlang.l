%{
#include "testlang.tab.h"
#include <string.h>

int line_num = 1;
%}

%%

"TEST"                  { return TEST; }
"REQUEST"               { return REQUEST; }
"ASSERT"                { return ASSERT; }
"METHOD"                { return METHOD; }
"URL"                   { return URL; }
"HEADERS"               { return HEADERS; }
"BODY"                  { return BODY; }

"GET"|"POST"|"PUT"|"DELETE" {
    yylval.str = strdup(yytext);
    return HTTP_METHOD;
}

"status"                { yylval.str = strdup(yytext); return STRING; }
"body"                  { yylval.str = strdup(yytext); return STRING; }

"contains"              { return CONTAINS; }
"=="                    { return EQ; }

"{"                     { return LBRACE; }
"}"                     { return RBRACE; }
":"                     { return COLON; }
"."                     { return DOT; }

\"([^"]*)\"             {
    yylval.str = strdup(yytext);
    return STRING;
}

[0-9]+                  {
    yylval.num = atoi(yytext);
    return NUMBER;
}

[a-zA-Z_][a-zA-Z0-9_]*  {
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

[ \t\r]+                { /* ignore spaces and tabs */ }
\n                      { line_num++; }

.                       { printf("Unknown token: %s at line %d\n", yytext, line_num); }

%%

int yywrap() { return 1; }
