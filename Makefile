all:
	flex testlang.l
	bison -d testlang.y
	gcc lex.yy.c testlang.tab.c -o testlang_compiler
	javac Samples/src/*.java

run:
	./testlang_compiler < sample.test
	cd Samples/src && javac GeneratedTests.java && java GeneratedTests
