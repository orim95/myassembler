#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H
#include "auxiliary_functions_constants.h"
#include "fixed_tables.h"




/**
 * @brief Defines a structure to store a macro's name and body.
 * @struct Macro
 * @param name Pointer to the macro's name string.
 * @param body Pointer to the macro's body content string.
 */
struct Macro {
    char* name; /* Pointer to macro name */
    char* body; /* Pointer to macro body */
};
 
/**
 * @brief Reads rows from the source file, processes macros, and writes the output to the destination file.
 *
 * @param file The name of the source file being processed.
 * @param source The source file pointer.
 * @param dest The destination file pointer.
 * @param macro_table A pointer to the macro table (array of Macro structures).
 * @param macro_table_size A pointer to the size of the macro table.
 *
 * @note Assumes that the source file is properly formatted and contains valid macro definitions.
 *       The function processes each row, expanding macros and writing the result to the destination file.
 */
int read_row_pre(const char* file, FILE* source, FILE* dest, struct Macro** macro_table, int* macro_table_size);

#endif /* PRE_ASSEMBLER_H */