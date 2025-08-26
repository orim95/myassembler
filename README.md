# MyAssembler

An assembler program written in C as part of the "Systems Programming Lab" course at the Open University of Israel.  
The project received a grade of 85.

## Overview
This program processes one or more assembly source files provided as command-line arguments.  
It performs the following steps for each file:
1. Reads the source file and processes macros using the pre-assembler.
2. Allocates and initializes the symbols table, command code array, and data code array.
3. Executes the first pass to analyze the source file and populate the symbols table and code arrays.
4. Executes the second pass to generate the final output files (if no errors are encountered).
5. Frees all allocated memory and handles errors appropriately.

## Usage
```bash
./myassembler file1.as file2.as ...
