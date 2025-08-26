#ifndef FIRST_PATH_H
#define FIRST_PATH_H
#include "auxiliary_functions_constants.h"
#include "pre_assembler.h"
#include "fixed_tables.h"




/**
 * @brief Performs the first pass of the assembler process on the given source file.
 *
 * This function reads the source file, processes its rows, and updates the symbol table,
 * command code, and data code. It also calculates the final instruction counter (ICF)
 * and data counter (DCF) values. The function handles macros, symbols, and validates
 * the input during the first pass.
 *
 * @param fileName The name of the source file (without extension) to process.
 * @param macro_table An array of Macro structures containing macro definitions.
 * @param macro_table_size The size of the macro table.
 * @param symbols_table A pointer to the symbol table, which will be updated during processing.
 * @param symbols_table_size A pointer to the size of the symbol table, which will be updated.
 * @param cmd_code A pointer to the command code array, which will be updated during processing.
 * @param cmd_capacity The capacity of the command code array.
 * @param ICF A pointer to store the final instruction counter value.
 * @param data_code A pointer to the data code array, which will be updated during processing.
 * @param data_capacity The capacity of the data code array.
 * @param DCF A pointer to store the final data counter value.
 * 
 * @return TRUE if the first pass was successful and did not find errors in the input file, FALSE otherwise.
 *
 * @note The function assumes that the source file has already been preprocessed
 *       (e.g., macros expanded) and has a ".am" extension.
 * @note The function updates the addresses of symbols in the symbol table based on their type.
 *       Symbols of type "data" are offset by (ICF + 100), and symbols of type "code" are offset by 100.
 *       If a symbol of type "entry" has an undefined address, an error is reported.
 * @note The function exits the program with an error message if the source file cannot be opened.
 */
int first_path(const char* fileName,
   struct Macro* macro_table,
   int macro_table_size,
   struct Symbol** symbols_table,
   int* symbols_table_size,
   int** cmd_code,
   int cmd_capacity,
   int* ICF,
   int** data_code,
   int data_capacity,
   int* DCF);


   
   
/**
 * @brief Manages the symbols table by performing various actions such as adding a symbol, 
 *        updating its type, finding a symbol, retrieving its address, or adding an external address.
 * 
 * @param name The name of the symbol to manage.
 * @param type The type of the symbol ("code", "data", "entry", "external").
 * @param symbols_table Pointer to the symbols table array.
 * @param symbols_table_size Pointer to the size of the symbols table.
 * @param action The action to perform (ADD_NAME, ADD_TYPE, FIND_NAME, GET_ADDRESS, ADD_EXTERNAL_ADDRESS).
 * @param address The address to assign to the symbol (used for ADD_NAME and ADD_EXTERNAL_ADDRESS actions).
 * @param r The line number in the source code for error reporting.
 * @param macro_table Pointer to the macro table array.
 * @param macro_table_size The size of the macro table.
 * @param index The index of the symbol in the table (used for GET_ADDRESS and ADD_EXTERNAL_ADDRESS actions).
 * 
 * @return int Returns TRUE (1) on success, FALSE (0) on failure; or the index of the symbol / NO (-1)  in the table for FIND_NAME.
 * 
 * @note This function handles memory allocation for the symbols table and external addresses.
 *       It also performs error checking for conflicting symbol definitions.
 */

 int symbols_table_management(char* name, char* type, struct Symbol** symbols_table, int* symbols_table_size,
   int action, int address, int r, struct Macro* macro_table, int macro_table_size, int index);


#endif /* FIRST_PATH_H */