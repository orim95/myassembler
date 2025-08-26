#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "second_path.h"
/**
 * @file second_path.c
 * @brief Implements the second pass of the assembler, processing symbols and generating machine code.
 *
 * This file contains functions for handling the second pass of the assembler. It processes rows of assembly code,
 * manages the symbols table, and generates the final machine code.
 *
 * @note This module assumes that the first pass has already been completed successfully.
 */




/**
 * @brief Reads rows from a source file during the second pass of assembly processing.
 *
 * This function processes each row of the given source file, validates its content,
 * and updates the symbols table and other relevant data structures as needed.
 *
 * @param file The name of the file being processed (used for error reporting).
 * @param source A pointer to the open file stream to read rows from.
 * @param cmd_code A pointer to an integer that stores the command code.
 * @param symbols_table A pointer to the symbols table, which is updated during processing.
 * @param symbols_table_size A pointer to the size of the symbols table.
 * @param isExternal A pointer to an integer flag indicating if the symbol is external.
 * @param isEntry A pointer to an integer flag indicating if the symbol is an entry.
 * @return int Returns TRUE if all rows are valid, otherwise FALSE.
 */
static int read_row_second(const char* file, FILE* source, int* cmd_code, struct Symbol** symbols_table,
   int* symbols_table_size, int* isExternal, int* isEntry);




/**
 * @brief Processes a row in the second pass of an assembler and determines its type.
 *
 * This function analyzes a given row of assembly code during the second pass of the assembler.
 * It identifies the type of the row (directive, command, label) and performs the necessary
 * operations, such as managing the symbols table or processing commands.
 *
 * @param row The input row of assembly code as a null-terminated string.
 * @param r The current row number in the source file.
 * @param symbols_table A pointer to the symbols table, which is an array of Symbol structures.
 * @param symbols_table_size A pointer to the size of the symbols table.
 * @param cmd_code A pointer to store the command code if the row contains a valid command.
 * @param isExternal A pointer to a flag indicating if the row references an external symbol.
 * @param isEntry A pointer to a flag indicating if the row contains an entry directive.
 * 
 * @return TRUE if the row is successfully processed or does not require further processing.
 *         Otherwise, returns an error code indicating the failure reason.
 */
static int row_type_second(char* row, int r, struct Symbol** symbols_table, int* symbols_table_size,
   int* cmd_code, int* isExternal, int* isEntry);




/**
 * @brief Processes a command row to determine the command code and updates the symbols table.
 *
 * This function analyzes a given row of assembly code, extracts symbols, and determines their
 * addressing modes. It updates the command code array and the symbols table accordingly.
 * The function also handles errors related to undefined symbols, invalid addressing modes,
 * and external symbols.
 *
 * @param row The input row of assembly code as a string.
 * @param i The starting index in the row to begin processing.
 * @param cmd_code Pointer to the command code array to be updated.
 * @param symbols_table Pointer to the symbols table array.
 * @param symbols_table_size Pointer to the size of the symbols table.
 * @param r The current line number in the source code (used for error reporting).
 * @param isExternal Pointer to a flag indicating if an external symbol was encountered.
 * 
 * @return Returns TRUE (1) if the row was processed successfully, or FALSE (0) if an error occurred.
 *
 * @note The function assumes that the symbols table and command code array are properly initialized.
 *       It also assumes that the input row is null-terminated.
 * @note The function modifies the global variable IC (Instruction Counter).
 * @note Error messages are printed to the standard output in case of invalid symbols or addressing modes.
 */
static int symbol_command_code(char* row, int i, int* cmd_code, struct Symbol** symbols_table,
   int* symbols_table_size, int r, int* isExternal);





extern int IC, DC; /* IC: Instruction Counter, DC: Data Counter - shared across modules */





static int symbol_command_code(char* row, int i, int* cmd_code, struct Symbol** symbols_table,
   int* symbols_table_size, int r, int* isExternal) {
   int word2, j, isAnd; /* word2: stores calculated address, j: index in symbols table, isAnd: flag for relative addressing */
   char word1[MAX] = { 0 }; /* word1: buffer to store extracted word */
   word2 = 0;

   IC++; /* Increment instruction counter for the current command */

   while (row[i] != '\n' && row[i] != '\0') { /* Process the row until end of line or null terminator */
      isAnd = FALSE; /* Reset relative addressing flag */
      i = copy_word_jump_space(row, word1, i); /* Extract the next word from the row */

      if (word1[0] == '#') { /* Immediate addressing mode */
         IC++; /* Increment IC for immediate value */
      }
      if (word1[0] == '\0' || word1[0] == 'r' || word1[0] == '#') { /* Skip empty words, registers, or immediate values */
         continue;
      }

      if (word1[0] == '&') { /* Relative addressing mode */
         memmove(word1, word1 + 1, strlen(word1)); /* Remove '&' from the word */
         isAnd = TRUE; /* Mark as relative addressing */
      }
      if ((j = symbols_table_management(word1, NULL, symbols_table, symbols_table_size, FIND_NAME, 0, r, NULL, 0, 0)) != NO) {
         /* Check if the symbol exists in the symbols table */
         if (((*symbols_table)[j].type != NULL) && (strstr((*symbols_table)[j].type, "external") == NULL)) {
            /* If the symbol is not external */
            if (isAnd == TRUE) { /* Handle relative addressing */
               if(strstr((*symbols_table)[j].type, "data") != NULL){ /* Check if symbol is a data symbol */
                  printf("Error - line %d: the symbol (%s) is a data symbol, and cannot be used with relative addressing.\n", r, word1);
                  return FALSE; /* Return error for invalid relative addressing */
               }
               word2 = (symbols_table_management(word1, NULL, symbols_table, symbols_table_size, GET_ADDRESS, 0, r, NULL, 0, j) - (IC + 100) + 1) << ARE_BITS;
               cmd_code[IC] = word2 + A; /* Store calculated address with absolute flag */
            }
            else { /* Direct addressing */
               word2 = symbols_table_management(word1, NULL, symbols_table, symbols_table_size, GET_ADDRESS, 0, r, NULL, 0, j);
               cmd_code[IC] = ((word2) << ARE_BITS) + R; /* Store address with relocatable flag */
            }

         }
         else { /* Handle external symbols */
            if (isAnd == TRUE) { /* Relative addressing is invalid for external symbols */
               printf("Error - line %d: the symbol (%s) is an external symbol, and cannot be used with relative addressing.\n", r, word1);
               return FALSE; /* Return error */
            }
            cmd_code[IC] = E; /* Mark as external */
            symbols_table_management(NULL, NULL, symbols_table, symbols_table_size, ADD_EXTERNAL_ADDRESS, IC + 100, r, NULL, 0, j);
            *isExternal = TRUE; /* Mark that an external symbol was encountered */
         }
      }
      else { /* Undefined symbol */
         printf("Error - line %d: One of the operands (%s) is an undefined label, or there are extraneous characters surrounding it.\n", r, word1);
         return FALSE; /* Return error for undefined label */
      }

      IC++; /* Increment instruction counter for the next word */
   }

   return TRUE; /* Successfully processed the row */
}





static int row_type_second(char* row, int r, struct Symbol** symbols_table, int* symbols_table_size,
   int* cmd_code, int* isExternal, int* isEntry) {
   char word1[MAX]; /* Buffer to store the first word in the row */
   int i, c; /* i: index in the row, c: command code index */

   i = 0; /* Initialize index to 0 */
   if (row[0] == '\n' || row[0] == '\0' || row[0] == ';') {
      /* Skip empty lines or comments */
      return TRUE;
   }

   i = copy_word_jump_space(row, word1, i); /* Extract the first word from the row */

   if (strcmp(word1, ".entry") == 0) {
      /* Handle .entry directive */
      i = copy_word_jump_space(row, word1, i); /* Extract the symbol name */
      *isEntry = TRUE; /* Mark that an entry symbol exists */
      return symbols_table_management(word1, "entry", symbols_table, symbols_table_size, ADD_TYPE, 0, r, NULL, 0, 0);
   }

   if ((c = cmd_table(word1)) != NO) {
      /* If the word is a valid command, process it */
      return symbol_command_code(row, i, cmd_code, symbols_table, symbols_table_size, r, isExternal);
   }

   if (isalpha(row[0])) {
      /* Check if the row starts with a label */
      if (row[i] == ':' && isspace(row[i + 1])) {
         i = copy_word_jump_space(row, word1, i + 1); /* Extract the word after the label */
         if ((strcmp(word1, ".data") == 0) || (strcmp(word1, ".string") == 0) || (strcmp(word1, ".extern") == 0)) {
            /* Skip .data, .string, and .extern directives */
            return TRUE;
         }
         if ((c = cmd_table(word1)) != NO) {
            /* If the word is a valid command, process it */
            return symbol_command_code(row, i, cmd_code, symbols_table, symbols_table_size, r, isExternal);
         }
      }
   }
   return TRUE; /* Return TRUE for rows that don't require further processing */
}




static int read_row_second(const char* file, FILE* source, int* cmd_code, struct Symbol** symbols_table,
   int* symbols_table_size, int* isExternal, int* isEntry) {
   char row[MAX]; /* Buffer to store each line of the source file */
   int r, InputValidation; /* r: current row number, InputValidation: flag for overall validation */

   r = 1; /* Initialize row number to 1 */
   InputValidation = TRUE; /* Assume input is valid initially */

   /* Read the source file line by line */
   while (fgets(row, MAX, source)) {
      /* Process the current row and update validation status */
      if (row_type_second(row, r, symbols_table, symbols_table_size, cmd_code, isExternal, isEntry) == FALSE)
         InputValidation = FALSE; /* Mark as invalid if any row fails validation */

      r++; /* Increment row number */
   }

   return InputValidation; /* Return the overall validation status */
}




int second_path(const char* filename, struct Symbol* symbols_table, int symbols_table_size,
   int* cmd_code, int* data_code, int ICF, int DCF)
{
   FILE* source; /* Pointer to the source file */
   int input_validation, isExternal, isEntry; /* Flags for validation, external symbols, and entry symbols */
   char amFilename[256]; /* Buffer to hold the filename with .am extension */

   isEntry = FALSE; /* Initialize entry flag to FALSE */
   isExternal = FALSE; /* Initialize external flag to FALSE */

   /* Construct the filename with .am extension */
   sprintf(amFilename, "%s%s", filename, ".am");
   source = fopen(amFilename, "r"); /* Open the source file for reading */
   if (source == NULL) {
      perror("Error opening source file"); /* Print error if file cannot be opened */
      exit(EXIT_FAILURE); /* Exit the program with failure status */
   }

   IC = 0, DC = 0; /* Reset instruction counter (IC) and data counter (DC) */
   
   /* Process the source file row by row during the second pass */
   input_validation = read_row_second(filename, source, cmd_code, &symbols_table, &symbols_table_size, &isExternal, &isEntry);

   fclose(source); /* Close the source file */

   if (input_validation) {
      /* If no errors, generate output files */
      printf("No errors in the input file: %s, generating its output files.\n", filename);
      output(filename, cmd_code, data_code, symbols_table, symbols_table_size, ICF, DCF, isExternal, isEntry);
      return TRUE; /* Return success */
   }
   else {
      /* If errors exist, do not generate output files */
      printf("Errors in the input file: %s, not generating its output files.\n", filename);
      return FALSE; /* Return failure */
   }


}