#ifndef OUTPUT_H
#define OUTPUT_H
#include "first_path.h"
#include "pre_assembler.h"

/**
 * @brief Outputs the assembly process results to files.
 *
 * This function generates and writes the output files for the assembly process.
 * It creates an object file (.ob) containing the command and data code, an external
 * symbols file (.ext) if external symbols exist, and an entry symbols file (.ent)
 * if entry symbols exist.
 *
 * @param fileName The base name of the output files (without extension).
 * @param cmd_code Pointer to the array containing the command code.
 * @param data_code Pointer to the array containing the data code.
 * @param symbols_table Pointer to the symbol table containing symbol information.
 * @param symbols_table_size The size of the symbol table.
 * @param ICF The final value of the instruction counter.
 * @param DCF The final value of the data counter.
 * @param isExternal A flag indicating whether there are external symbols (non-zero if true).
 * @param isEntry A flag indicating whether there are entry symbols (non-zero if true).
 */
void output(const char* fileName, int* cmd_code, int* data_code, struct Symbol* symbols_table, int symbols_table_size, int ICF, int DCF, int isExternal, int isEntry);

#endif /* OUTPUT_H */