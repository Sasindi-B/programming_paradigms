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
JAVA_SRC = Samples/src/*.java
INPUT = sample.test

# Default rule
all: $(TARGET) java

$(TARGET): $(LEX_SRC) $(YACC_SRC)
	$(YACC) $(YACC_SRC)
	$(LEX) $(LEX_SRC)
	$(CC) lex.yy.c testlang.tab.c -o $(TARGET) -lfl

java:
	$(JAVAC) $(JAVA_SRC)

run:
	./$(TARGET) $(INPUT)

clean:
	rm -f lex.yy.c testlang.tab.c testlang.tab.h $(TARGET)
