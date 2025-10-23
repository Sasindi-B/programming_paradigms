%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern FILE *yyin;

FILE *out;

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
%}

%union {
    char* str;
    int num;
}

/* --- Tokens from lexer --- */
%token <str> TEST REQUEST ASSERT METHOD URL HEADERS BODY
%token <str> STRING IDENTIFIER HTTP_METHOD VARREF
%token <num> NUMBER
%token EQ CONTAINS ASSIGN SEMICOLON
%token CONFIG BASE_URL HEADER LET
%token LBRACE RBRACE COLON DOT

/* --- Type declarations for nonterminals --- */
%type <str> value
%type <str> var_decl
%type <str> config_item

%%

program:
    config_opt var_list_opt test_list { fprintf(out, "}\n"); }
;

/* ---------- CONFIG BLOCK ---------- */
config_opt:
    /* empty */
  | CONFIG LBRACE config_body RBRACE
;

config_body:
    config_item
  | config_body config_item
;

config_item:
    BASE_URL ASSIGN STRING SEMICOLON {
        fprintf(out, "    private static final String BASE_URL = %s;\n", $3);
    }
  | HEADER STRING ASSIGN STRING SEMICOLON {
        fprintf(out, "    // default header %s = %s\n", $2, $4);
    }
;

/* ---------- VARIABLES ---------- */
var_list_opt:
    /* empty */
  | var_list
;

var_list:
    var_decl
  | var_list var_decl
;

var_decl:
    LET IDENTIFIER ASSIGN value SEMICOLON {
        fprintf(out, "    private static final String %s = %s;\n", $2, $4);
    }
;

value:
    STRING { $$ = $1; }
  | NUMBER {
        char buf[32];
        sprintf(buf, "\"%d\"", $1);
        $$ = strdup(buf);
    }
;

/* ---------- TESTS ---------- */
test_list:
    test
  | test_list test
;

test:
    TEST IDENTIFIER LBRACE test_body RBRACE {
        fprintf(out, "\n    @Test\n");
        fprintf(out, "    public void %s() throws Exception {\n", $2);
        fprintf(out, "        HttpRequest request = HttpRequest.newBuilder()\n");
        fprintf(out, "            .uri(URI.create(url))\n");
        fprintf(out, "            .method(method, bodyPublisher)\n");
        fprintf(out, "            .headers(headersArray)\n");
        fprintf(out, "            .build();\n");
        fprintf(out, "        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());\n");
        fprintf(out, "        Assertions.assertEquals(expectedStatus, response.statusCode());\n");
        fprintf(out, "    }\n");
    }
;

test_body:
    request_section assert_section
;

/* ---------- REQUEST SECTION ---------- */
request_section:
    REQUEST LBRACE method_decl url_decl headers_opt body_opt RBRACE
;

method_decl:
    METHOD COLON HTTP_METHOD {
        fprintf(out, "        String method = \"%s\";\n", $3);
    }
;

url_decl:
    URL COLON STRING {
        fprintf(out, "        String url = %s;\n", $3);
    }
  | URL COLON VARREF {
        fprintf(out, "        String url = %s;\n", $3);
    }
;

headers_opt:
    /* empty */
  | HEADERS LBRACE header_list RBRACE
;

header_list:
    header
  | header_list header
;

header:
    STRING COLON STRING {
        fprintf(out, "        // header %s: %s\n", $1, $3);
    }
;

/* ---------- BODY SECTION ---------- */
body_opt:
    /* empty */
  | BODY COLON STRING {
        fprintf(out, "        String body = %s;\n", $3);
    }
;

/* ---------- ASSERT SECTION ---------- */
assert_section:
    ASSERT LBRACE assert_list RBRACE
;

assert_list:
    assert
  | assert_list assert
;

assert:
    "status" EQ NUMBER {
        fprintf(out, "        int expectedStatus = %d;\n", $3);
    }
  | "body" DOT IDENTIFIER EQ STRING {
        fprintf(out, "        // assert body.%s == %s\n", $3, $5);
    }
  | "body" DOT IDENTIFIER CONTAINS STRING {
        fprintf(out, "        // assert body.%s contains %s\n", $3, $5);
    }
;

%%

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename.test>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("File not found");
        return 1;
    }

    out = fopen("GeneratedTests.java", "w");
    if (!out) {
        perror("Cannot open output file");
        return 1;
    }

    fprintf(out, "import org.junit.jupiter.api.*;\n");
    fprintf(out, "import java.net.http.*;\n");
    fprintf(out, "import java.net.URI;\n");
    fprintf(out, "\npublic class GeneratedTests {\n");
    fprintf(out, "    private static final HttpClient client = HttpClient.newHttpClient();\n");

    yyin = file;
    printf("Parsing %s...\n", argv[1]);
    yyparse();
    printf("✅ Parsing complete. Generated: GeneratedTests.java\n");

    fclose(file);
    fclose(out);
    return 0;
}
