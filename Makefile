# Makefile for TestLang++ compiler project

# Compiler and tools
LEX = flex
YACC = bison -d
CC = gcc
JAVAC = javac

# Files
LEX_SRC = testlang.l
YACC_SRC = testlang.y
TARGET = testlang_compiler
# Include all Java sources recursively (for package directories like org/junit/...)
JAVA_SRC = $(shell find Samples/src -name "*.java")
# Default input .test script (override with `make run INPUT=...`)
INPUT ?= Samples/sample.test

# Default rule
all: $(TARGET) java

$(TARGET): $(LEX_SRC) $(YACC_SRC)
	$(YACC) $(YACC_SRC)
	$(LEX) $(LEX_SRC)
	$(CC) lex.yy.c testlang.tab.c -o $(TARGET) -lfl

gen: $(TARGET)
	./$(TARGET) $(INPUT)

java: gen
	$(JAVAC) $(JAVA_SRC)

run: $(TARGET)
	./$(TARGET) $(INPUT)
	$(JAVAC) $(JAVA_SRC)

clean:
	rm -f lex.yy.c testlang.tab.c testlang.tab.h $(TARGET)

.PHONY: java run clean
