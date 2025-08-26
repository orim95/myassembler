/**
 * @file asembler.h
 * @brief Header file for an assembler program that processes assembly language files.
 *
 * This file contains the definitions, constants, and function prototypes used in the assembler program.
 * The assembler performs two main passes over the input file: the first pass processes macros and symbols,
 * while the second pass generates machine code and resolves addresses.
 *
 * ## Algorithm Overview:
 * 1. **Pre-Assembler Phase**:
 *    - Handles macro definitions and expansions.
 *    - Reads the input file and replaces macro calls with their corresponding body.
 * 2. **First Pass**:
 *    - Scans the assembly file to build a symbol table and validate syntax.
 *    - Identifies labels, instructions, and directives.
 * 3. **Second Pass**:
 *    - Generates the final machine code.
 *    - Resolves addresses for symbols and handles data/code sections.
 *
 * ## Assumptions:
 * - The input file is a assembly file.
 * - Macro and symbol names are limited to 31 characters.
 * - Memory word size is 25 bits.
 * - The program uses a dynamic symbol table and macro table with initial sizes defined by constants.
 *
 * ## Constants:
 * - `INITIAL_ROW_SIZE`: Maximum size of a single row in the output file.
 * - `MAX_MACRO_NAME`: Maximum length of a macro name.
 * - `MAX_SYMBOL_NAME`: Maximum length of a symbol name.
 * - `MEM_WORD_SIZE`: Size of a memory word in bits.
 * - `INITIAL_SYMBOLS_TABLE_SIZE`: Initial size of the symbol table.
 *
 * ## Data Structures:
 * - `struct Symbol`: Represents a symbol with its name, address, and type.
 * - `struct Macro`: Represents a macro with its name and body.
 *
 * ## Function Prototypes:
 * - `void pre_assembler(const char *file)`: Processes macros in the input file.
 * - `char *strdup(const char *s)`: Duplicates a string (custom implementation).
 * - `int copy_word(char *row, char *word, int i)`: Copies a word from a row starting at index `i`.
 * - `int conversion_10to2(char *str, char *result_char, int length)`: Converts a decimal string to binary.
 * - `int conversion_2to16(char *str, char *result_char)`: Converts a binary string to hexadecimal.
 * - `int ascii_values(char *str, char *dest)`: Converts a string to its ASCII values.
 * - `int copy_word_jump_space(char *row, char *word, int i)`: Copies a word from a row, skipping spaces.
 * - `void first_path(const char *file, struct Macro *macro_table, int macro_table_size)`: Performs the first pass.
 * - `void second_path(const char *file, struct Symbol *symbols_table, int symbols_table_size, char *cmd_code, char *data_code, struct Macro *macro_table, int macro_table_size, int *L, int L_capacity)`: Performs the second pass.
 *
 * ## Notes:
 * - Ensure that memory allocation for dynamic structures (e.g., symbol table, macro table) is handled properly.
 * - The program assumes that input files are well-formed and adhere to the expected assembly language syntax.
 * - However, the program includes basic validation checks for syntax errors, undefined symbols, and invalid macro definitions.
 */

#ifndef ASEMBLER_H
#define ASEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>


#define TRUE 1
#define FALSE 0
#define NO -1

#define INITIAL_ROW_SIZE 81
#define MAX_MACRO_NAME 31
#define MAX_SYMBOL_NAME 31
#define MAX 81
#define INITIAL_SYMBOLS_TABLE_SIZE 20
#define MEM_WORD_SIZE 25
#define INITIAL_CMD_CODE_SIZE 20
#define INITIAL_DATA_CODE_SIZE 20

#define ADD_NAME 1
#define FIND_NAME 2
#define ADD_TYPE 3
#define GET_ADDRESS 4
#define ADD_EXTERNAL_ADDRESS 5


#define IMMEDIATE_ADDRESSING 0
#define DIRECT_ADDRESSING 1
#define RELATIVE_ADDRESSING 2
#define REGISTER_ADDRESSING 3

#define A 4
#define R 2
#define E 1
#define FIRST_WORD_ARE 4

#define ARE_BITS 3
#define FUNC_BITS 5
#define OPCODE_BITS 6
#define TARGET_REGISTER_BITS 3
#define SOURCE_REGISTER_BITS 3
#define SOURCE_ADDRESSING_BITS 2
#define TARGET_ADDRESSING_BITS 2


struct Symbol {
   char name[MAX];
   int address;
   char type[MAX];
   char *extern_address;
};

struct Macro {
   char* name;
   char* body;
};






/**
 * pre_assembler - Preprocesses the assembly file to handle macros and other pre-assembly tasks.
 * @file: The path to the assembly file to preprocess.
 *
 * Algorithm:
 * - Reads the assembly file line by line.
 * - Identifies and processes macros, replacing their occurrences with their definitions.
 * - Writes the preprocessed content to a temporary file for further processing.
 *
 * Assumptions:
 * - The file exists and is accessible.
 * - Macros are defined and used correctly in the assembly file.
 */
void pre_assembler(const char* file);



/**
 * strdup - Duplicates a given string.
 * @s: The string to duplicate.
 *
 * Algorithm:
 * - Allocates memory for a new string of the same length as the input.
 * - Copies the content of the input string to the newly allocated memory.
 * - Returns a pointer to the duplicated string.
 *
 * Assumptions:
 * - The input string is null-terminated.
 * - Memory allocation succeeds.
 */

char* myStrdup(const char* s);




/**
 * copy_word - Copies a word from a row into a separate buffer.
 * @row: The input string containing the row.
 * @word: The buffer to store the copied word.
 * @i: The starting index in the row to begin copying.
 *
 * Algorithm:
 * - Copies characters until a space or end of the row is encountered.
 * - Null-terminates the copied word.
 * - Returns the index of the next character after the copied word.
 *
 * Assumptions:
 * - The input row is null-terminated.
 * - The word buffer is large enough to hold the copied word.
 */
int copy_word(char* row, char* word, int i);





/**
 * conversion_10to2 - Converts a decimal number to its binary representation.
 * @str: The input string representing the decimal number.
 * @result_char: The buffer to store the binary representation.
 * @length: The desired length of the binary representation.
 *
 * Algorithm:
 * - Parses the input string as a decimal number.
 * - Converts the number to binary and stores it in the result buffer.
 * - Pads the binary representation with leading zeros to match the desired length.
 *
 * Assumptions:
 * - The input string is a valid decimal number.
 * - The result buffer is large enough to hold the binary representation.
 */
int conversion_10to2(char* str, char* result_char, int length);



/**
 * conversion_2to16 - Converts a binary number to its hexadecimal representation.
 * @str: The input string representing the binary number.
 * @result_char: The buffer to store the hexadecimal representation.
 *
 * Algorithm:
 * - Groups the binary digits into sets of four.
 * - Converts each group to its corresponding hexadecimal digit.
 * - Stores the resulting hexadecimal string in the result buffer.
 *
 * Assumptions:
 * - The input string is a valid binary number.
 * - The result buffer is large enough to hold the hexadecimal representation.
 */
int conversion_2to16(char* str, char* result_char);




/**
 * copy_word_jump_space - Copies a word from a row, skipping spaces.
 * @row: The input string containing the row.
 * @word: The buffer to store the copied word.
 * @i: The starting index in the row to begin copying.
 *
 * Algorithm:
 * - Skips leading spaces from the starting index.
 * - Copies characters until a space or end of the row is encountered.
 * - Null-terminates the copied word.
 * - Returns the index of the next character after the copied word.
 *
 * Assumptions:
 * - The input row is null-terminated.
 * - The word buffer is large enough to hold the copied word.
 */
int copy_word_jump_space(char* row, char* word, int i);

int copy_word_jump_space_count_coma(char* row, char* word, int i, int* coma);


/**
 * first_path - Performs the first pass of the assembler process.
 * @file: The path to the assembly file.
 * @macro_table: The table containing macro definitions.
 * @macro_table_size: The size of the macro table.
 *
 * Algorithm:
 * - Reads the assembly file line by line.
 * - Processes labels, macros, and directives.
 * - Builds a symbol table and validates syntax.
 *
 * Assumptions:
 * - The file exists and is accessible.
 * - The macro table is properly initialized.
 */
void first_path(const char* file, struct Macro* macro_table, int macro_table_size);





/**
 * second_path - Performs the second pass of the assembler process.
 * @file: The path to the assembly file.
 * @symbols_table: The table containing symbol definitions.
 * @symbols_table_size: The size of the symbols table.
 * @cmd_code: The buffer to store the machine code for commands.
 * @data_code: The buffer to store the machine code for data.
 * @macro_table: The table containing macro definitions.
 * @macro_table_size: The size of the macro table.
 * @L: The buffer to store the final machine code.
 * @L_capacity: The capacity of the final machine code buffer.
 *
 * Algorithm:
 * - Reads the assembly file line by line.
 * - Resolves symbols and generates machine code for commands and data.
 * - Combines the command and data code into the final machine code buffer.
 *
 * Assumptions:
 * - The file exists and is accessible.
 * - The symbol and macro tables are properly initialized.
 * - The buffers are large enough to hold the generated machine code.
 */
int second_path(const char* filename, struct Symbol* symbols_table, int symbols_table_size,
   int* cmd_code, int* data_code,
   struct Macro* macro_table, int macro_table_size, int* cmd_capacity, int* data_capacity, int ICF, int DCF);


void output(const char* fileName, int* cmd_code, int* data_code, struct Symbol* symbols_table, int symbols_table_size, int ICF, int DCF, int isExternal, int isEntry);

int check_number(char* word);

int check_capacity(int** code ,int* code_capacity, int num);

void free_symbols_table(struct Symbol* symbols_table, int size);

#endif /* ASEMBLER_H */
