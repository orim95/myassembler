#ifndef AUXILIARY_FUNCTIONS_H
#define AUXILIARY_FUNCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "pre_assembler.h"
 

#define TRUE 1
#define FALSE 0
#define NO -1

#define MACRO_END_LENGTH 7
#define MCRO_LENGTH 4
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
#define PRINT 3
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

#define SOURCE
#define TARGET

#ifndef SYMBOL_STRUCT_DEFINED
#define SYMBOL_STRUCT_DEFINED

struct Symbol {
    char name[MAX];
    int address;
    char type[MAX];
    int* extern_address;
    int extern_address_size;
};

#endif /* SYMBOL_STRUCT_DEFINED */


/**
 * @brief Duplicates a string by allocating memory and copying the input string.
 * @param s Pointer to the input string to duplicate.
 * @return Pointer to the duplicated string, or NULL if memory allocation fails.
 */
char* myStrdup(const char* s);



/**
 * @brief Copies a word from a row into a buffer until a delimiter is encountered.
 * @param row Pointer to the input string.
 * @param word Buffer to store the copied word.
 * @param i Current index in the input string.
 * @return Updated index after copying the word.
 */
int copy_word(char* row, char* word, int i);




/**
 * @brief Copies a word from a row into a buffer, skipping leading spaces and handling a trailing comma.
 * @param row Pointer to the input string.
 * @param word Buffer to store the copied word.
 * @param i Current index in the input string.
 * @return Updated index after copying the word and handling a comma.
 */
int copy_word_jump_space(char* row, char* word, int i);



/**
 * @brief Copies a word from a row, skips spaces, and validates commas before and after.
 * @param row Pointer to the input string.
 * @param word Buffer to store the copied word.
 * @param i Pointer to the current index in the string.
 * @param comaValidationBefor Expected number of commas before the word.
 * @param comaValidationAfter Expected number of commas after the word.
 * @param r Line number for error reporting.
 * @return TRUE if the operation and validations pass, FALSE otherwise.
 */
int copy_word_jump_space_count_coma(char* row, char* word, int* i, int comaValidationBefor, int comaValidationAfter, int r);



/**
 * @brief Checks if a string represents a valid number (digits, optional '+' or '-' signs).
 * @param word Pointer to the string to check.
 * @return TRUE if the string is a valid number, FALSE otherwise.
 */
int check_number(char* word);


/**
 * @brief Ensures the capacity of a dynamic integer array is sufficient, resizing if needed.
 * @param code Pointer to the pointer of the integer array.
 * @param code_capacity Pointer to the current capacity of the array.
 * @param num Required capacity.
 * @return TRUE if the operation is successful.
 */
int ensure_capacity(int** code, int* code_capacity, int num);


/**
 * @brief Frees the memory allocated for a symbols table and its external addresses.
 * @param symbols_table Pointer to the symbols table to free.
 * @param size Number of entries in the table.
 */
void free_symbols_table(struct Symbol* symbols_table, int size);


/**
 * @brief Checks for illegal extra words or characters after a specified point in a row.
 * @param row Pointer to the input string.
 * @param i Current index in the string.
 * @param r Line number for error reporting.
 * @param after Description of the context (what comes before the checked part).
 * @return TRUE if no extra words are found, FALSE otherwise.
 */
int check_extra_word(char* row, int i, int r, char* after);


/**
 * @brief Expands the size of the symbols table by doubling its capacity and initializing new entries.
 * @param symbols_table Pointer to the pointer of the symbols table.
 * @param symbols_table_size Pointer to the current size of the table.
 * @return TRUE if the operation is successful.
 */
int expand_symbols_table(struct Symbol** symbols_table, int* symbols_table_size);


#endif /* AUXILIARY_FUNCTIONS_H */