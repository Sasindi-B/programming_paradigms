🧪 TestLang++: A DSL for HTTP API Testing 📘 Overview

TestLang++ is a domain-specific language (DSL) designed for describing and executing automated HTTP API tests in a simple, readable syntax. It allows testers and developers to define API test cases using keywords such as GET, POST, and ASSERT, and automatically generates executable Java test code.

The compiler is implemented using Lex (Flex) and Yacc (Bison), which parse .test scripts and generate corresponding Java test files (GeneratedTests.java). These generated tests can then be compiled and executed to validate HTTP endpoints.

🎯 Project Objectives

Design a simple DSL syntax for API testing.

Implement a scanner and parser using Lex and Bison.

Translate .test files into runnable Java test code.

Automatically execute HTTP requests (GET/POST) using Java’s HttpClient.
#refer the Makefile
In the terminal
make
make run

⚙️ Prerequisites
Before running, ensure these are installed on your system:
sudo apt update
sudo apt install flex bison gcc make default-jdk

🚀 How to Build and Run
🧩 Step 1: Build Everything

In the project root directory (~/programming_paradigms):

make

This will:

Generate scanner (lex.yy.c) and parser (testlang.tab.c)

Compile them into the executable testlang_compiler

Compile supporting Java files in Samples/src/

▶️ Step 2: Run the Parser and Generate Tests

Use:

make run

This runs your parser on sample.test and produces GeneratedTests.java.
If your .test file name is different, update the Makefile or manually run:

./testlang_compiler < yourfile.test

☕ Step 3: Compile the Generated Java Tests

Navigate into the Java source folder and compile:

cd Samples/src
javac \*.java

🧪 Step 4: Execute the Generated Tests

You can run directly from the compiled Java files:

java dsl.generated.GeneratedTests

🧹 Cleaning the Build

To remove all generated files:

make clean

🩵 Notes

Run your backend (e.g., Spring Boot) at http://localhost:8080 before executing tests.

Make sure your .test file syntax matches the grammar rules in testlang.y.

Each run overwrites the previous GeneratedTests.java.
