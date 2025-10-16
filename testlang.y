%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper: append text to the generated Java file
void writeToFile(const char* text) {
    FILE *f = fopen("Samples/src/GeneratedTests.java", "a");
    if (f) {
        fprintf(f, "%s\n", text);
        fclose(f);
    }
}

int yylex(void);
void yyerror(const char *s);
%}

%union {
    char* str;
}

%token GET POST ASSERT
%token <str> STRING

%%
program:
    { 
        writeToFile("import java.io.*;");
        writeToFile("import java.net.http.*;");
        writeToFile("import java.net.*;");
        writeToFile("public class GeneratedTests {");
    }
    statement_list
    { 
        writeToFile("    public static void main(String[] args) throws Exception {");
        writeToFile("        testGet();");
        writeToFile("        testPost();");
        writeToFile("    }");
        writeToFile("}");
    }
;

statement_list:
    | statement_list statement
;

statement:
      GET STRING     
        { 
            char buf[512]; 
            sprintf(buf, "    public static void testGet() throws Exception { HttpUtil.sendGet(%s); }", $2); 
            writeToFile(buf); 
        }
    | POST STRING    
        { 
            char buf[512]; 
            sprintf(buf, "    public static void testPost() throws Exception { HttpUtil.sendPost(%s, \"{}\"); }", $2); 
            writeToFile(buf); 
        }
    | ASSERT STRING  
        { 
            char buf[512]; 
            sprintf(buf, "    // Assertion: %s", $2); 
            writeToFile(buf); 
        }
;
%%

// Called when parser hits an error
void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

// Entry point
int main() {
    remove("Samples/src/GeneratedTests.java");
    printf("Parsing sample.test...\n");

    yyparse();

    printf("✅ Parsing complete! GeneratedTests.java created in Samples/src/\n");
    return 0;
}
