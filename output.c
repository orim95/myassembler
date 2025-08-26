#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "output.h"

/**
 * write_ob - Writes the object code to a specified file.
 *
 * This function generates an output file containing the object code
 * in a specific format. The file begins with a header line indicating the size of
 * the instruction code (ICF) and data code (DCF). It then writes the instruction
 * code followed by the data code, each formatted as an address and a 24-bit hexadecimal value.
 *
 * @param obFilename The name of the output file to write to.
 * @param cmd_code   An array containing the instruction code.
 * @param data_code  An array containing the data code.
 * @param ICF        The final instruction counter value (number of instructions).
 * @param DCF        The final data counter value (number of data entries).
 *
 * @return TRUE if the operation is successful.
 *
 * @note The function exits the program with an error message if the file cannot be opened.
 */
static int write_ob(const char* obFilename, int* cmd_code, int* data_code, int ICF, int DCF);



/**
 * write_ext - Writes external symbols and their addresses to a file.
 *
 * This function iterates through a symbol table to find symbols marked as
 * "external" and writes their names along with their associated addresses
 * to a specified output file. Each line in the output file contains the
 * symbol name and an address in the format: "symbol_name address".
 *
 * @param extFilename: The name of the output file to write the external symbols.
 * @param symbols_table: Pointer to the array of Symbol structures containing
 *                       the symbol table data.
 * @param symbols_table_size: The number of entries in the symbol table.
 *
 * @return TRUE (non-zero) on success.
 *
 * @note If the file cannot be opened for writing, the function prints an
 *       error message using perror, closes the file (if applicable), and
 *       terminates the program with an exit status of EXIT_FAILURE.
 */
static int write_ext(const char* extFilename, struct Symbol* symbols_table, int symbols_table_size);



/**
 * @brief Writes all symbols of type "entry" from the symbols table to a file.
 *
 * This function iterates through the provided symbols table and writes the name
 * and address of each symbol marked as "entry" to the specified file. The output
 * format for each line in the file is: `<symbol_name> <symbol_address>`, where
 * the address is zero-padded to 7 digits.
 *
 * @param entFilename The name of the file to write the "entry" symbols to.
 * @param symbols_table A pointer to the array of Symbol structures containing
 *                      the symbols to be processed.
 * @param symbols_table_size The number of elements in the symbols table.
 * 
 * @return TRUE (non-zero) if the operation completes successfully.
 * 
 * @note The function will terminate the program with an error message if:
 *       - The file cannot be opened for writing.
 *       - An error occurs while writing to the file.
 *       - The file cannot be closed properly.
 */
static int write_ent(const char* entFilename, struct Symbol* symbols_table, int symbols_table_size);




static int write_ent(const char* entFilename, struct Symbol* symbols_table, int symbols_table_size) {
   FILE* dest; /* Pointer to the output file */
   int i; /* Loop counter for iterating through the symbols table */

   /* Open the entry symbols file for writing */
   dest = fopen(entFilename, "w");
   if (dest == NULL) {
      /* Handle error if the file cannot be opened */
      perror("Error opening destination file");
      exit(EXIT_FAILURE);
   }

   /* Iterate through the symbols table to find "entry" symbols */
   for (i = 0; i < symbols_table_size; i++) {
      /* Check if the symbol type is not NULL and contains "entry" */
      if (symbols_table[i].type != NULL && strstr(symbols_table[i].type, "entry") != NULL) {
         /* Write the symbol name and zero-padded address to the file */
         if (fprintf(dest, "%s %07d\n", symbols_table[i].name, symbols_table[i].address) < 0) {
            /* Handle error if writing to the file fails */
            perror("Error writing to file");
            fclose(dest);
            exit(EXIT_FAILURE);
         }
      }
   }
   
   /* Close the file after writing */
   if (fclose(dest) != 0) {
      /* Handle error if the file cannot be closed */
      perror("Error closing file");
      exit(EXIT_FAILURE);
   }

   return TRUE; /* Indicate success */
}




static int write_ext(const char* extFilename, struct Symbol* symbols_table, int symbols_table_size) {
   FILE* dest; /* Pointer to the output file */
   int i, j; /* Loop counters */

   /* Open the external symbols file for writing */
   dest = fopen(extFilename, "w");

   if (dest == NULL) {
      /* Handle error if the file cannot be opened */
      perror("Error opening destination file");
      fclose(dest);
      exit(EXIT_FAILURE);
   }

   /* Iterate through the symbol table to find external symbols */
   for (i = 0; i < symbols_table_size; i++) {
      /* Check if the symbol type is "external" */
      if (symbols_table[i].type[0] != '\0' &&
         strcmp(symbols_table[i].type, "external") == 0) {
         /* Write each external address associated with the symbol */
         for (j = 0; j < symbols_table[i].extern_address_size; j++) {
            fprintf(dest, "%s %07d\n", symbols_table[i].name, symbols_table[i].extern_address[j]);
         }
      }
   }

   /* Close the file after writing */
   fclose(dest);
   return TRUE; /* Indicate success */
}




static int write_ob(const char* obFilename, int* cmd_code, int* data_code, int ICF, int DCF) {
   FILE* dest; /* Pointer to the output file */
   int i; /* Loop counter */

   /* Open the output file for writing */
   dest = fopen(obFilename, "w");

   if (dest == NULL) {
      /* Handle error if the file cannot be opened */
      perror("Error opening destination file");
      fclose(dest);
      exit(EXIT_FAILURE);
   }

   /* Write the header line with ICF (instruction count) and DCF (data count) */
   fprintf(dest, "     %d %d\n", ICF, DCF);

   /* Write the instruction code to the file */
   for (i = 0; i < ICF; i++) {
      /* Format: address (starting from 100) and 24-bit hexadecimal value */
      fprintf(dest, "%07d %06X\n", i + 100, cmd_code[i] & 0xFFFFFF);
   }

   /* Write the data code to the file */
   for (i = 0; i < DCF; i++) {
      /* Format: address (starting after instructions) and 24-bit hexadecimal value */
      fprintf(dest, "%07d %06X\n", i + ICF + 100, data_code[i] & 0xFFFFFF);
   }

   /* Close the file after writing */
   fclose(dest);
   return TRUE; /* Indicate success */
}



void output(const char* fileName, int* cmd_code, int* data_code, struct Symbol* symbols_table, int symbols_table_size, int ICF, int DCF, int isExternal, int isEntry) {
   char obFilename[256]; /* Buffer to store the .ob file name */
   char extFilename[256]; /* Buffer to store the .ext file name */
   char entFilename[256]; /* Buffer to store the .ent file name */

   /* Create the .ob file name by appending ".ob" to the base file name */
   sprintf(obFilename, "%s%s", fileName, ".ob");
   /* Create the .ext file name by appending ".ext" to the base file name */
   sprintf(extFilename, "%s%s", fileName, ".ext");
   /* Create the .ent file name by appending ".ent" to the base file name */
   sprintf(entFilename, "%s%s", fileName, ".ent");

   /* Write the object file (.ob) with command and data code */
   write_ob(obFilename, cmd_code, data_code, ICF, DCF);

   /* If there are external symbols, write the external file (.ext) */
   if (isExternal)
      write_ext(extFilename, symbols_table, symbols_table_size);

   /* If there are entry symbols, write the entry file (.ent) */
   if (isEntry)
      write_ent(entFilename, symbols_table, symbols_table_size);

}