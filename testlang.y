%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int yylex();
extern FILE *yyin;

FILE *out;

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
/* Buffer for deferred assertions emitted in assert_section */
static char assert_buffer[65536];
static void asserts_reset() { assert_buffer[0] = '\0'; }
static void asserts_append(const char* s) {
    size_t cur = strlen(assert_buffer);
    size_t add = strlen(s);
    if (cur + add + 1 < sizeof(assert_buffer)) {
        memcpy(assert_buffer + cur, s, add + 1);
    }
}
/* Buffer for default headers from config */
static char default_headers[8192];
static void headers_append(const char* s) {
    size_t cur = strlen(default_headers);
    size_t add = strlen(s);
    if (cur + add + 1 < sizeof(default_headers)) {
        memcpy(default_headers + cur, s, add + 1);
    }
}

/* Build a Java expression for URL from a DSL string that may include $vars.
   If the path is relative (starts with '/'), prefix with BASE_URL. */
static char* build_url_expr_from_string(const char* yylval_str) {
    const char* s = yylval_str;
    size_t len = strlen(s);
    const char* p = s;
    const char* end = s + len;
    if (len >= 2 && s[0] == '"' && s[len-1] == '"') {
        p = s + 1;
        end = s + len - 1;
    }
    char buf[4096];
    size_t pos = 0;
    int is_abs = ((end - p) >= 4 && strncmp(p, "http", 4) == 0);
    if (!is_abs) {
        pos += snprintf(buf + pos, sizeof(buf) - pos, "BASE_URL + ");
    }
    int in_quote = 0;
    pos += snprintf(buf + pos, sizeof(buf) - pos, "\"");
    in_quote = 1;
    while (p < end && pos < sizeof(buf) - 1) {
        if (*p == '$') {
            if (in_quote) {
                pos += snprintf(buf + pos, sizeof(buf) - pos, "\" + ");
                in_quote = 0;
            }
            p++;
            const char* start = p;
            while (p < end && (isalnum((unsigned char)*p) || *p == '_')) p++;
            size_t n = (size_t)(p - start);
            char var[128];
            if (n > sizeof(var) - 1) n = sizeof(var) - 1;
            memcpy(var, start, n);
            var[n] = '\0';
            pos += snprintf(buf + pos, sizeof(buf) - pos, "%s + ", var);
            pos += snprintf(buf + pos, sizeof(buf) - pos, "\"");
            in_quote = 1;
            continue;
        }
        if (*p == '\\' || *p == '"') {
            pos += snprintf(buf + pos, sizeof(buf) - pos, "\\%c", *p);
        } else {
            pos += snprintf(buf + pos, sizeof(buf) - pos, "%c", *p);
        }
        p++;
    }
    if (in_quote) {
        pos += snprintf(buf + pos, sizeof(buf) - pos, "\"");
    }
    char* out = (char*)malloc(pos + 1);
    memcpy(out, buf, pos);
    out[pos] = '\0';
    return out;
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
        char hbuf[512];
        snprintf(hbuf, sizeof(hbuf), "        builder.header(%s, %s);\n", $2, $4);
        headers_append(hbuf);
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
    TEST IDENTIFIER LBRACE {
        fprintf(out, "\n    @Test\n");
        fprintf(out, "    public void %s() throws Exception {\n", $2);
        fprintf(out, "        String method = null;\n");
        fprintf(out, "        String url = null;\n");
        fprintf(out, "        String body = null;\n");
        fprintf(out, "        int expectedStatus = 200;\n");
        fprintf(out, "        HttpRequest.Builder builder = HttpRequest.newBuilder();\n");
        fprintf(out, "%s", default_headers);
        asserts_reset();
    } test_body RBRACE {
        fprintf(out, "        if (url != null) builder.uri(URI.create(url));\n");
        fprintf(out, "        if (\"GET\".equalsIgnoreCase(method)) {\n");
        fprintf(out, "            builder = builder.GET();\n");
        fprintf(out, "        } else if (body != null) {\n");
        fprintf(out, "            builder = builder.method(method, HttpRequest.BodyPublishers.ofString(body));\n");
        fprintf(out, "        } else {\n");
        fprintf(out, "            builder = builder.method(method, HttpRequest.BodyPublishers.noBody());\n");
        fprintf(out, "        }\n");
        fprintf(out, "        HttpRequest request = builder.build();\n");
        fprintf(out, "        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());\n");
        fprintf(out, "        Assertions.assertEquals(expectedStatus, response.statusCode());\n");
        fprintf(out, "        String bodyStr = response.body();\n");
        fprintf(out, "%s", assert_buffer);
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
        fprintf(out, "        method = \"%s\";\n", $3);
    }
;

url_decl:
    URL COLON STRING {
        char* expr = build_url_expr_from_string($3);
        fprintf(out, "        url = %s;\n", expr);
        free(expr);
    }
  | URL COLON VARREF {
        fprintf(out, "        url = %s;\n", $3);
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
        fprintf(out, "        builder.header(%s, %s);\n", $1, $3);
    }
;

/* ---------- BODY SECTION ---------- */
body_opt:
    /* empty */
  | BODY COLON STRING {
        fprintf(out, "        body = %s;\n", $3);
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
    IDENTIFIER EQ NUMBER {
        if (strcmp($1, "status") == 0) {
            fprintf(out, "        expectedStatus = %d;\n", $3);
        } else {
            /* Unknown identifier in status position; emit comment */
            fprintf(out, "        // unsupported assert: %s == %d\n", $1, $3);
        }
    }
  | IDENTIFIER DOT IDENTIFIER EQ STRING {
        if (strcmp($1, "body") == 0) {
            char buf[512];
            snprintf(buf, sizeof(buf),
                     "        Assertions.assertTrue(bodyStr.contains(\"\\\"%s\\\":%s\"), \"body.%s == %s\");\n",
                     $3, $5, $3, $5);
            asserts_append(buf);
        } else {
            fprintf(out, "        // unsupported assert scope: %s.%s == %s\n", $1, $3, $5);
        }
    }
  | IDENTIFIER DOT IDENTIFIER CONTAINS STRING {
        if (strcmp($1, "body") == 0) {
            char buf[512];
            snprintf(buf, sizeof(buf),
                     "        Assertions.assertTrue(bodyStr.contains(\"\\\"%s\\\"\"), \"body has field %s\");\n",
                     $3, $3);
            asserts_append(buf);
            snprintf(buf, sizeof(buf),
                     "        Assertions.assertTrue(bodyStr.contains(%s), \"body.%s contains %s\");\n",
                     $5, $3, $5);
            asserts_append(buf);
        } else {
            fprintf(out, "        // unsupported assert scope: %s.%s contains %s\n", $1, $3, $5);
        }
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

    out = fopen("Samples/src/GeneratedTests.java", "w");
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
    printf("Parsing complete. Generated: Samples/src/GeneratedTests.java\n");

    fclose(file);
    fclose(out);
    return 0;
}

