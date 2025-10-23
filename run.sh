#!/bin/bash
set -e

echo "Running TestLang++ DSL compiler..."
./testlang_compiler < sample.test

echo "GeneratedTests.java created under Samples/src/"

cd Samples/src
javac GeneratedTests.java
echo "✅ Generated Java compiled successfully."
