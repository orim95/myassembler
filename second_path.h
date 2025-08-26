#ifndef SECOND_PATH_H
#define SECOND_PATH_H

#pragma once
#include "first_path.h"
#include "output.h"

/**
 * @brief Performs the second pass of the assembler process on the given source file.
 * 
 * This function reads the preprocessed assembly file, validates its content, and generates
 * the necessary output files if no errors are found. It also updates the symbol table and
 * checks for external and entry symbols.
 * 
 * @param filename The name of the source file (without extension).
 * @param symbols_table Pointer to the symbol table array.
 * @param symbols_table_size The size of the symbol table array.
 * @param cmd_code Pointer to the command code array.
 * @param data_code Pointer to the data code array.
 * @param ICF The final value of the instruction counter after the first pass.
 * @param DCF The final value of the data counter after the first pass.
 * 
 * @return TRUE (1) if the second pass is successful and output files are generated, 
 *         FALSE (0) otherwise.
 */
int second_path(const char* filename, struct Symbol* symbols_table, int symbols_table_size,
   int* cmd_code, int* data_code, int ICF, int DCF);


#endif /* SECOND_PATH_H */
