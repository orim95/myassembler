/**
 * @brief Main function for the assembler program.
 *
 * This function processes one or more assembly source files provided as command-line arguments.
 * It performs the following steps for each file:
 * 1. Reads the source file and processes macros using the pre-assembler.
 * 2. Allocates and initializes the symbols table, command code array, and data code array.
 * 3. Executes the first path to analyze the source file and populate the symbols table and code arrays.
 * 4. Executes the second path to generate the final output files if no errors are encountered.
 * 5. Frees all allocated memory and handles errors appropriately.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments. Each argument after the program name is treated as a filename.
 * @return Returns 0 on successful execution, or 1 if no filename is provided.
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "auxiliary_functions_constants.h"
#include "pre_assembler.h"
#include "first_path.h"
#include "second_path.h"

#define INITIAL_MACRO_TABLE_SIZE 20


int main(int argc, char* argv[]) {
   int i, j;

   /* Check if the user provided a filename */
   if (argc < 2) {
      printf("Usage: %s <filename>\n", argv[0]);
      return 1;
   }

   for (i = 1; i < argc; i++)
   {
      char asFilename[256] = { 0 }; /* Source file name with .as extension */
      char amFilename[256] = { 0 }; /* Destination file name with .am extension */
      FILE* source, * dest; /* File pointers for source and destination files */
      int macro_table_size = INITIAL_MACRO_TABLE_SIZE; /* Initial size of the macro table */
      struct Macro* macro_table = (struct Macro*)calloc(macro_table_size, sizeof(struct Macro)); /* Macro table */
      struct Symbol* symbols_table; /* Symbols table */
      int symbols_table_size, cmd_capacity, data_capacity, ICF, DCF; /* Various capacities and counters */
      int first_path_result, pre_assembler_result; /* Results of pre-assembler and first path */
      int* cmd_code, * data_code; /* Arrays for command and data code */
      char* filename = argv[i]; /* Current file name from command-line arguments */

      /* Check memory allocation for macro table */
      if (macro_table == NULL) {
         perror("Error allocating memory for macro table");
         exit(EXIT_FAILURE);
      }

      /* Construct file names for source and destination */
      sprintf(asFilename, "%s%s", filename, ".as");
      sprintf(amFilename, "%s%s", filename, ".am");

      /* Open source file for reading */
      source = fopen(asFilename, "r");
      if (source == NULL) {
         perror("Error opening source file");
         exit(EXIT_FAILURE);
      }

      /* Open destination file for writing */
      dest = fopen(amFilename, "w");
      if (dest == NULL) {
         perror("Error opening destination file");
         fclose(source);
         exit(EXIT_FAILURE);
      }
      printf("Processing file: %s\n", filename);
      /* Process macros using the pre-assembler */
      pre_assembler_result = read_row_pre(amFilename, source, dest, &macro_table, &macro_table_size);

      /* Close source and destination files */
      fclose(source);
      fclose(dest);
   
      /* Initialize symbols table */
      symbols_table_size = INITIAL_SYMBOLS_TABLE_SIZE;
      symbols_table = (struct Symbol*)calloc(symbols_table_size, sizeof(struct Symbol));
      if (symbols_table == NULL) {
         perror("Error allocating memory for symbols table");
         exit(EXIT_FAILURE);
      }

      /* Clear symbols table entries */
      for (j = 0; j < symbols_table_size; j++) {
         memset(symbols_table[j].name, 0, sizeof(symbols_table[j].name));
         memset(symbols_table[j].type, 0, sizeof(symbols_table[j].type));
         symbols_table[j].extern_address = NULL;
      }

      /* Initialize command and data code arrays */
      cmd_capacity = INITIAL_CMD_CODE_SIZE;
      data_capacity = INITIAL_DATA_CODE_SIZE;

      cmd_code = (int*)calloc(cmd_capacity, sizeof(int));
      data_code = (int*)calloc(data_capacity, sizeof(int));
      if (cmd_code == NULL || data_code == NULL) {
         perror("Error allocating memory for cmd_code or data_code arrays");
         exit(EXIT_FAILURE);
      }
      
      /* Perform first and second paths if pre-assembler succeeded */
      if(pre_assembler_result) {
         first_path_result = first_path(filename, macro_table, macro_table_size, &symbols_table, &symbols_table_size,
            &cmd_code, cmd_capacity, &ICF, &data_code, data_capacity, &DCF);

         if (first_path_result)
            second_path(filename, symbols_table, symbols_table_size, cmd_code, data_code, ICF, DCF);
         
         else
            printf("Errors in the input file: %s, not generating its output files.\n", filename);
         
      }
      else 
         printf("Errors in the input file: %s, not generating its output files.\n", filename);
      
      /* Free allocated memory for symbols table */
      free_symbols_table(symbols_table, symbols_table_size);

      /* Free command code array */
      if (cmd_code != NULL) {
         free(cmd_code);
         cmd_code = NULL;
      }

      /* Free data code array */
      if (data_code != NULL) {
         free(data_code);
         data_code = NULL;
      }

      /* Free macro table */
      if (macro_table != NULL) {
         free(macro_table);
         macro_table = NULL;
      }

   }
  
   return FALSE; /* Return FALSE if no filename is provided */
}