🧪 TestLang++: A DSL for HTTP API Testing 📘 Overview

TestLang++ is a domain-specific language (DSL) designed for describing and executing automated HTTP API tests in a simple, readable syntax. It allows testers and developers to define API test cases using keywords such as GET, POST, and ASSERT, and automatically generates executable Java test code.

The compiler is implemented using Lex (Flex) and Yacc (Bison), which parse .test scripts and generate corresponding Java test files (GeneratedTests.java). These generated tests can then be compiled and executed to validate HTTP endpoints.

🎯 Project Objectives

Design a simple DSL syntax for API testing.

Implement a scanner and parser using Lex and Bison.

Translate .test files into runnable Java test code.

Automatically execute HTTP requests (GET/POST) using Java’s HttpClient.
