#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "first_path.h"

/**
 * Reads and processes each row of the source file during the first pass.
 * 
 * @param file The name of the source file.
 * @param source The file pointer to the opened source file.
 * @param cmd_code Pointer to the command code array.
 * @param data_code Pointer to the data code array.
 * @param symbols_table Pointer to the symbols table.
 * @param symbols_table_size Pointer to the size of the symbols table.
 * @param macro_table The macro table.
 * @param macro_table_size The size of the macro table.
 * @param cmd_capacity The initial capacity of the command code array.
 * @param data_capacity The initial capacity of the data code array.
 * @return TRUE if all rows are valid, FALSE otherwise.
 */
static int read_row_first(const char *file, FILE *source, int **cmd_code, int **data_code, struct Symbol **symbols_table,
   int *symbols_table_size, struct Macro *macro_table, int macro_table_size, int cmd_capacity, int data_capacity);



   /**
 * @brief Processes a row of input and determines its type in the first pass of assembly.
 *
 * This function analyzes a given row of input during the first pass of an assembler.
 * It identifies the type of the row (e.g., command, data, or symbol declaration) and
 * updates the relevant tables and arrays accordingly.
 *
 * @param row The input row to be analyzed (as a null-terminated string).
 * @param r The current row number in the source file (used for error reporting).
 * @param symbols_table Pointer to the symbol table, which stores symbol definitions.
 * @param symbols_table_size Pointer to the size of the symbol table.
 * @param cmd_code Pointer to the array storing command code instructions.
 * @param data_code Pointer to the array storing data code instructions.
 * @param macro_table Array of macros defined in the source file.
 * @param macro_table_size The number of macros in the macro table.
 * @param cmd_capacity Pointer to the current capacity of the command code array.
 * @param data_capacity Pointer to the current capacity of the data code array.
 *
 * @return An integer indicating the success or failure of the operation:
 *         0 for success, or a non-zero error code for failure or errors in the input file.
 */
static int row_type_first(char* row, int r, struct Symbol** symbols_table, int* symbols_table_size,
   int** cmd_code, int** data_code, struct Macro* macro_table, int macro_table_size, int* cmd_capacity, int* data_capacity);

   
/**
 * @brief Processes and writes data or string declarations into the data code array.
 *
 * This function handles the parsing and validation of `.data` and `.string` directives
 * in assembly code. It ensures that the data is correctly formatted, within valid ranges,
 * and properly stored in the `data_code` array. Errors are reported with descriptive messages.
 *
 * @param row The current line of assembly code being processed.
 * @param data_code Pointer to the array where data code is stored.
 * @param data_capacity Pointer to the current capacity of the `data_code` array.
 * @param r The current line number in the source file (used for error reporting).
 * @param i The current index in the `row` string being processed.
 * @param tmp The first word of the line, expected to be a directive like `.data` or `.string`.
 * @return int Returns TRUE (1) if the operation is successful, or FALSE (0) if an error occurs or found at the input row.
 */
static int write_data_code(char* row, int** data_code, int* data_capacity, int r, int i, char* tmp);



/**
 * @brief Writes the machine code representation of a command into the command code array.
 *
 * This function processes a given command, extracts its operands, and generates the appropriate
 * machine code words. It ensures that the command code array has sufficient capacity and updates
 * the instruction counter (IC) accordingly. The function also validates the syntax of the command
 * and handles source and target labels if present.
 *
 * @param row           The current line of code being processed.
 * @param i             Pointer to the current index in the row being processed.
 * @param c             The index of the command in the command table.
 * @param cmd_code      Pointer to the array holding the generated machine code.
 * @param cmd_capacity  Pointer to the capacity of the cmd_code array.
 * @param r             The current line number in the source file (used for error reporting).
 * 
 * @return int          Returns TRUE (1) if the command was successfully processed and written,
 *                      or FALSE (0) if an error occurred or found in the line.
 */
static int write_command_code(char* row, int* i, int c, int** cmd_code, int* cmd_capacity, int r);



/**
 * @brief Processes and writes an operand into the provided words based on its addressing method.
 *
 * This function analyzes the given operand and determines its addressing method. It validates the operand
 * against the command's supported addressing methods and writes the appropriate data into the provided
 * word pointers. It also handles errors such as missing operands, invalid formats, or unsupported addressing methods.
 *
 * @param operand A pointer to the operand string to be processed.
 * @param c The index of the command in the command array.
 * @param word1 A pointer to the first word to be updated with operand data.
 * @param word2 A pointer to the second word to be updated with operand data.
 * @param word3 A pointer to the third word to be updated with operand data (optional).
 * @param r The current line number in the source file (used for error reporting).
 * @param operandLabel A pointer to an integer flag indicating if the operand is a label.
 * @param operandNum The operand number (1 for source operand, 2 for destination operand).
 * 
 * @return int Returns TRUE if the operand was successfully processed and written; otherwise, FALSE.
 *
 * @note The function assumes that the `cmd` array and constants such as `ARE_BITS`, `A`, `DIRECT_ADDRESSING`,
 *       `RELATIVE_ADDRESSING`, and `REGISTER_ADDRESSING` are defined elsewhere in the program.
 * @note The function modifies the `operand` string in-place for certain addressing methods (e.g., immediate and register).
 * @note Error messages are printed to `stdout` for invalid operands or unsupported addressing methods.
 */
static int write_operand(char* operand, int c, int* word1, int* word2, int* word3, int r, int* operandLabel, int operandNum);





   /**
 * @brief Validates a given symbol name based on specific criteria.
 *
 * This function checks if a symbol name adheres to the following rules:
 * - The symbol name must not exceed the maximum allowed length (`MAX_SYMBOL_NAME`).
 * - The symbol name must not be empty.
 * - The symbol name must not be a reserved word.
 * - The symbol name must not conflict with existing macro names.
 * - The symbol name must start with a letter.
 * - The symbol name must only contain alphanumeric characters.
 *
 * If any of these rules are violated, an appropriate error message is printed
 * to the standard output, indicating the line number and the specific issue.
 *
 * @param name The symbol name to validate.
 * @param r The line number where the symbol is defined (used for error reporting).
 * @param macro_table A pointer to the macro table for checking macro conflicts.
 * @param macro_table_size The size of the macro table.
 * @return TRUE if the symbol name is valid, FALSE otherwise.
 */

static int check_symbol(char* name, int r, struct Macro* macro_table, int macro_table_size);



/**
 * Checks if a given name matches any macro name in the macro table.
 *
 * This function iterates through the macro table and compares the provided
 * name with the names of the macros stored in the table. If a match is found,
 * it returns TRUE; otherwise, it returns FALSE.
 *
 * @param name The name to check against the macro table.
 * @param macro_table A pointer to the array of Macro structures.
 * @param macro_table_size The number of elements in the macro table.
 * @return TRUE if the name matches a macro in the table, FALSE otherwise.
 */
static int is_macro(char* name, struct Macro* macro_table, int macro_table_size);




/*
 * DC - Data Counter: This variable is used to keep track of the current address
 *      in the data section of the program. It is incremented as new data is added.
 *
 * IC - Instruction Counter: This variable is used to keep track of the current address
 *      in the instruction section of the program. It is incremented as new instructions are added.
 */
int DC, IC;


static int is_macro(char* name, struct Macro* macro_table, int macro_table_size) {
   int i;
   for (i = 0; i < macro_table_size; i++) {
      if (macro_table[i].name != NULL && strcmp(macro_table[i].name, name) == 0) {
         return TRUE;/*Found macro name*/
      }
   }
   return FALSE;/*Not found macro name*/
}





static int check_symbol(char* name, int r, struct Macro* macro_table, int macro_table_size) {
   int i; /* Loop index for iterating through the symbol name */

   /* Check if the symbol name exceeds the maximum allowed length */
   if (strlen(name) > MAX_SYMBOL_NAME) {
      printf("Error - line %d: the symbol (%s) is too long.\n", r, name);
      return FALSE;
   }

   /* Check if the symbol name is empty */
   if (name[0] == '\0') {
      printf("Error - line %d: missing a label name.\n", r);
      return FALSE;
   }

   /* Check if the symbol name is a reserved word */
   if (reserved_word(name)) {
      printf("Error - line %d: the symbol (%s) is a reserved word.\n", r, name);
      return FALSE;
   }

   /* Check if the symbol name conflicts with an existing macro */
   if (is_macro(name, macro_table, macro_table_size)) {
      printf("Error - line %d: the symbol (%s) is a macro.\n", r, name);
      return FALSE;
   }

   /* Ensure the symbol name starts with a letter */
   if (!isalpha(name[0])) {
      printf("Error - line %d: the symbol (%s) must start with a letter.\n", r, name);
      return FALSE;
   }

   /* Validate that the symbol name contains only alphanumeric characters */
   for (i = 1; i < strlen(name); i++) {
      if (!isalnum(name[i])) {
         printf("Error - line %d: the symbol (%s) must contain only letters and numbers.\n", r, name);
         return FALSE;
      }
   }

   /* If all checks pass, the symbol name is valid */
   return TRUE;
}




int symbols_table_management(char* name, char* type, struct Symbol** symbols_table, int* symbols_table_size,
   int action, int address, int r, struct Macro* macro_table, int macro_table_size, int index) {
   int i; /* Loop index */
   int* new_address; /* Pointer for reallocating external addresses */
   char tmp[MAX] = { 0 }; /* Temporary buffer for address formatting */

   new_address = NULL;

   /* Handle adding a new symbol name */
   if (action == ADD_NAME) {
      if(!check_symbol(name, r, macro_table, macro_table_size)) /* Validate symbol name */
         return FALSE;
      for (i = 0; i < *symbols_table_size; i++) {
         if (strcmp((*symbols_table)[i].name, name) == 0) { /* Check if symbol already exists */
            /* Check for conflicting entry and external definitions */
            if((strcmp(type, "external") == 0 && strstr((*symbols_table)[i].type, "entry") != NULL) ||
               (strcmp(type, "entry") == 0 && strstr((*symbols_table)[i].type, "external") != NULL)) {
               printf("Error - line %d: the symbol (%s) has been defined both as entry and external.\n", r, name);
               return FALSE;
            }
            if ((*symbols_table)[i].address != NO){ /* Symbol already defined */
               printf("Error - line %d: the symbol (%s) is already defined.\n", r, name);
               return FALSE;
            }
            if ((*symbols_table)[i].address == NO) { /* Update address if undefined */
               (*symbols_table)[i].address = address;
               return symbols_table_management(name, type, symbols_table, symbols_table_size, ADD_TYPE, NO, r, macro_table, macro_table_size, NO);
            }
         }
         if ((*symbols_table)[i].name[0] == 0) { /* Empty slot in symbols table */
            strcat((*symbols_table)[i].name, name); /* Add name */
            (*symbols_table)[i].address = address; /* Set address */
            strcat((*symbols_table)[i].type, type); /* Set type */
            return TRUE;
         }
      }

      /* Expand symbols table if no empty slot found */
      i = *symbols_table_size - 1;
      if (!expand_symbols_table(symbols_table, symbols_table_size))
         return FALSE;

      /* Use the last index in the expanded table */
      strcpy((*symbols_table)[i].name, name);
      (*symbols_table)[i].address = address;
      strcpy((*symbols_table)[i].type, type);
      return TRUE;
   }

   /* Handle adding a type to an existing symbol */
   if (action == ADD_TYPE) {
      for (i = 0; i < *symbols_table_size; i++) {
         if ((*symbols_table)[i].name != NULL && strcmp((*symbols_table)[i].name, name) == 0) {
            /* Check for conflicting entry and external definitions */
            if(strcmp(type, "entry") == 0 && strstr((*symbols_table)[i].type, "external") != NULL) {
               printf("Error - line %d: the symbol (%s) has been defined both as entry and external.\n", r, name);
               return FALSE;
            }
            strcat((*symbols_table)[i].type, type); /* Append type */
            return TRUE;
         }
      }
      /* If symbol not found, add it as a new symbol */
      return symbols_table_management(name, type, symbols_table, symbols_table_size, ADD_NAME, NO, r, macro_table, macro_table_size, NO);
   }

   /* Handle finding a symbol by name */
   else if (action == FIND_NAME) {
      for (i = 0; i < *symbols_table_size; i++) {
         if ((*symbols_table)[i].name != NULL && strcmp((*symbols_table)[i].name, name) == 0) {
            return i; /* Return index of the symbol */
         }
      }
      return NO; /* Symbol not found */
   }

   /* Handle retrieving the address of a symbol by index */
   else if (action == GET_ADDRESS) {
      return (*symbols_table)[index].address; /* Return address */
   }

   /* Handle adding an external address to a symbol */
   else if (action == ADD_EXTERNAL_ADDRESS) {
      sprintf(tmp, "%07d, ", address); /* Format address as a string */
      new_address = realloc((*symbols_table)[index].extern_address, ((*symbols_table)[index].extern_address_size + 1) * sizeof(int));
      if (new_address == NULL) { /* Check for memory allocation failure */
         perror("Error allocating memory for new address string");
         exit(EXIT_FAILURE);
      }
      (*symbols_table)[index].extern_address = new_address; /* Update pointer */
      (*symbols_table)[index].extern_address[(*symbols_table)[index].extern_address_size] = address; /* Add address */
      (*symbols_table)[index].extern_address_size++; /* Increment size */
      return TRUE;
   }

   return FALSE; /* Default return for unsupported actions */
}



static int write_operand(char* operand, int c, int* word1, int* word2, int* word3, int r, int* operandLabel, int operandNum) {
   int bits, num; /* Variables for bit manipulation and numeric conversion */
   char* endptr = NULL; /* Pointer for strtol to detect invalid characters */
   char operandNumString[MAX] = { 0 }; /* Buffer for operand number string */

   strcpy(operandNumString, operandNum == 1 ? "source" : "destination"); /* Determine operand number string */
   
   /* If word2 is not empty and word3 is provided => this command may need another data word => if need, write opperand data to the third word*/
   if (*word2 != 0 && word3 != NULL) {
       word2 = word3;
   }
 
   /* Check if the destination operand exists */
   if (cmd[c].dest != NULL) {
       
       /* Validate that the operand is not empty */
       if(operand[0] == '\0'){
         printf("Error - line %d: missing operand.\n", r);
            return FALSE;
       }

       /* Handle immediate addressing (operand starts with '#') */
       
       if (operand[0] == '#') {
         /* Validate that the command supports immediate addressing for the operand */
         if ((operandNum == 1 && strpbrk(cmd[c].source, "0") == NULL) ||
            (operandNum == 2 && strpbrk(cmd[c].dest, "0") == NULL) ) {
            printf("Error - line %d: the command does not support immediate addressing for %s operand.\n", r, operandNumString);
            return FALSE;
         }
 
         /* Remove the '#' character from the operand */
         memmove(operand, operand + 1, strlen(operand));
 
         /* Convert the operand to an integer */
         num = strtol(operand, &endptr, 10);

         /* Check if the operand is not a valid integer */
         if (*endptr != '\0'){
            printf("Error - line %d: operand in the immediate addressing method (%s) is not an integer.\n", r, operand);
            return FALSE;
         }

         /* Validate the range of the immediate value */
         if(num < -(1 << 20) || num > (1 << 20) - 1){
            printf("Error - line %d: the immediate addressing method (%s) is not a valid number (out of range).\n", r, operand);
            return FALSE;
         }

         /* Check for missing number after '#' */
         if(num == 0 && strcmp(operand, "0") != 0){
            printf("Error - line %d: missing number after '#' for immediate addressing.\n", r);
            return FALSE;
         }
 
         /* Encode the immediate value into word2 */
         *word2 = (num << ARE_BITS) + A; /* Store the immediate value with ARE bits */
         return TRUE;
       }
 
       /* Handle direct addressing (operand is alphanumeric but not a register) */
       if (isalnum(operand[0]) && operand[0] != 'r') {
          /* Validate that the command supports direct addressing for the operand */
          if((operandNum == 1 && strpbrk(cmd[c].source, "1") == NULL) ||
          (operandNum == 2 && strpbrk(cmd[c].dest, "1") == NULL)){
             printf("Error - line %d: the command does not support direct addressing  for %s operand.\n", r, operandNumString);
             return FALSE;
          }

          /* Calculate the bit position for direct addressing */
          bits = (operandNum == 1 ? 
          (ARE_BITS + FUNC_BITS + TARGET_REGISTER_BITS + TARGET_ADDRESSING_BITS + SOURCE_REGISTER_BITS) : 
          (ARE_BITS + FUNC_BITS + TARGET_REGISTER_BITS));

          /* Encode the addressing method into word1 */
          *word1 += (DIRECT_ADDRESSING << bits);

          /* Mark the operand as a label */
          *operandLabel = TRUE;
       }

       /* Handle relative addressing (operand starts with '&') */
       if (operand[0] == '&') {
          /* Validate that the command supports relative addressing for the operand */
          if((operandNum == 1 && strpbrk(cmd[c].source, "2") == NULL) ||
          (operandNum == 2 && strpbrk(cmd[c].dest, "2") == NULL)){
             printf("Error - line %d: this command does not support relative addressing  for %s operand.\n", r, operandNumString);
             return FALSE;
          }

          /* Calculate the bit position for relative addressing */
          bits = (operandNum == 1 ?
          (ARE_BITS + FUNC_BITS + TARGET_REGISTER_BITS + TARGET_ADDRESSING_BITS + SOURCE_REGISTER_BITS) :
          (ARE_BITS + FUNC_BITS + TARGET_REGISTER_BITS));

          /* Encode the addressing method into word1 */
          *word1 += (RELATIVE_ADDRESSING << bits);

          /* Mark the operand as a label */
          *operandLabel = TRUE;
       }
 
       /* Handle register addressing (operand starts with 'r') */
       if (operand[0] == 'r') {
          /* Validate that the command supports register addressing for the operand */
          if((operandNum == 1 && strpbrk(cmd[c].source, "3") == NULL) ||
          (operandNum == 2 && strpbrk(cmd[c].dest, "3") == NULL)){
             printf("Error - line %d: the command does not support register addressing for %s operand.\n", r, operandNumString);
             return FALSE;
          }

          /* Calculate the bit position for register addressing */
          bits = (operandNum == 1 ?
          (ARE_BITS + FUNC_BITS + TARGET_REGISTER_BITS + TARGET_ADDRESSING_BITS + SOURCE_REGISTER_BITS) :
          (ARE_BITS + FUNC_BITS + TARGET_REGISTER_BITS));

          /* Encode the addressing method into word1 */
          *word1 += (REGISTER_ADDRESSING << bits);  
 
          /* Remove the 'r' character from the operand */
          memmove(operand, operand + 1, strlen(operand));

          /* Validate the register number */
          if ((!(isdigit(operand[0]))) || operand[0] < '1' || operand[0] > '7') {
             printf("Error - line %d: the register number (%c) is not valid.\n", r, operand[0]);
             return FALSE;
          }

          /* Encode the register number into word1 */
          bits = (operandNum == 1 ?
          (ARE_BITS + FUNC_BITS + TARGET_REGISTER_BITS + TARGET_ADDRESSING_BITS) :
          (ARE_BITS + FUNC_BITS));
          *word1 += (atoi(operand) << bits);
       }

    return TRUE; /* Successfully processed the operand */
   }

   return FALSE;/*רק לשם הזהירות, תנאי זה לא אמור להתרחש*/
}
    


static int write_command_code(char* row, int* i, int c, int** cmd_code, int* cmd_capacity, int r) {
   int word1 = 0, word2 = 0, word3 = 0; /* Machine code words for the command */
   int sourceLabel, targetLabel; /* Flags to indicate if source/target operands are labels */
   char operand[MAX] = { 0 }; /* Buffer to store the current operand */
   int comaValidation; /* Flag to validate comma placement between operands */

   sourceLabel = FALSE;
   targetLabel = FALSE; /* Initialize label flags to FALSE */
 
   /* Encode the opcode and funct fields into the first word */
   word1 += ((cmd[c].opcode) << (ARE_BITS + FUNC_BITS + TARGET_REGISTER_BITS + TARGET_ADDRESSING_BITS + SOURCE_REGISTER_BITS + SOURCE_ADDRESSING_BITS));
   word1 += (cmd[c].funct << ARE_BITS) + FIRST_WORD_ARE;

   /* Process the source operand if it exists */
   if (cmd[c].source != NULL) {
      /* Determine if a comma is required between operands */
      comaValidation = (cmd[c].source == NULL || cmd[c].dest == NULL) ? 0 : 1;
      
      /* Extract and validate the source operand */
      if (!(copy_word_jump_space_count_coma(row, operand, i, 0, comaValidation, r))) {
         return FALSE; 
      }
      if (!(write_operand(operand, c, &word1, &word2, NULL, r, &sourceLabel, 1))) {
         return FALSE;
      }
   }   
         
   /* Process the destination operand if it exists */
   if (cmd[c].dest != NULL) {   
      /* Extract and validate the destination operand */
      if (!copy_word_jump_space_count_coma(row, operand, i, 0, 0, r)) {
         return FALSE;
      }
      if (!write_operand(operand, c, &word1, &word2, &word3, r, &targetLabel, 2)) {
         return FALSE;
      }
   }

   /* Ensure there are no extra words after the command */
   if (!check_extra_word(row, *i, r, "finishing a command")) {
      return FALSE;
   }

   /* Store the first word in the command code array */
   ensure_capacity(cmd_code, cmd_capacity, IC);
   (*cmd_code)[IC++] = word1;

   /* Increment IC for source label if present */
   if (sourceLabel == TRUE) {
      IC++;
   }

   /* Store the second word if it exists */
   if (word2 != 0) {
      ensure_capacity(cmd_code, cmd_capacity, IC);
      (*cmd_code)[IC++] = word2;
   }

   /* Increment IC for target label if present */
   if (targetLabel == TRUE) {
      IC++;
   }

   /* Store the third word if it exists */
   if (word3 != 0) {
      ensure_capacity(cmd_code, cmd_capacity, IC);
      (*cmd_code)[IC++] = word3;
   }
   
   return TRUE; /* Successfully processed the command */
}




static int write_data_code(char* row, int** data_code, int* data_capacity, int r, int i, char* tmp) {
   char word1[MAX] = { 0 }; /* Buffer to store the current word being processed */
   int num; /* Variable to store the parsed integer */
   char* endptr; /* Pointer for strtol to detect invalid characters */

   /* Handle `.data` directive */
   if ((strcmp(tmp, ".data")) == 0) {

      do {
         /* Extract the next word and validate comma placement */
         if (!(copy_word_jump_space_count_coma(row, word1, &i, 0, 1, r)))
            return FALSE;

         /* Convert the word to an integer */
         num = strtol(word1, &endptr, 10);

         /* Check if the number is within the valid range */
         if (num > (1 << 23) - 1 || num < -(1 << 23)) {
            printf("Error - line %d: invalid number (%d) in .data declaration (out of range).\n", r, num);
            return FALSE;
         }

         /* Check if the word contains invalid characters */
         if (*endptr != '\0') {
            printf("Error - line %d: one or more of the parameters (%s) is not an integer.\n", r, word1);
            return FALSE;
         }

         /* Check if the word is empty */
         if (word1[0] == '\0') {
            printf("Error - line %d: no numbers in .data declaration line.\n", r);
            return FALSE;
         }

         /* Ensure there is enough capacity in the data array and store the number */
         ensure_capacity(data_code, data_capacity, DC);
         (*data_code)[DC] = num;
         DC++; /* Increment the data counter */
      } while (row[i] != '\n' && row[i] != EOF); /* Continue until the end of the line */

      return TRUE; /* Successfully processed `.data` directive */
   }

   /* Handle `.string` directive */
   if (strcmp(tmp, ".string") == 0) {

      /* Skip whitespace characters */
      while (isspace(row[i])) {
         i++;
      }

      /* Check for the opening quotation mark */
      if (row[i] == EOF || row[i] != '"') {
         printf("Error - line %d: missing a quotation mark.\n", r);
         return FALSE;
      }
      i++; /* Move past the opening quotation mark */

      /* Process characters inside the string */
      while (row[i] != '\n' && row[i] != EOF && i <= MAX && row[i] != '"') {
         ensure_capacity(data_code, data_capacity, DC); /* Ensure capacity for the data array */
         (*data_code)[DC] = (int)row[i]; /* Store the ASCII value of the character */
         DC++; /* Increment the data counter */
         i++;
      }

      /* Check for the closing quotation mark */
      if (row[i] == '"') {
         check_extra_word(row, i + 1, r, "finishing a string line"); /* Validate no extra text after the string */
         ensure_capacity(data_code, data_capacity, DC); /* Ensure capacity for the null terminator */
         (*data_code)[DC] = 0; /* Add null terminator to the string */
         DC++; /* Increment the data counter */
         return TRUE; /* Successfully processed `.string` directive */
      }

      /* Missing closing quotation mark */
      printf("Error - line %d: missing a quotation mark.\n", r);
      return FALSE;
   }

   /* Invalid directive or unrecognized word */
   printf("Error - line %d: the first word (%s) is not valid: must be valid command, data declaration, label definition, or symbol directives.\n", r, tmp);
   return FALSE;
}



static int row_type_first(char* row, int r, struct Symbol** symbols_table, int* symbols_table_size,
   int** cmd_code, int** data_code, struct Macro* macro_table, int macro_table_size, int* cmd_capacity, int* data_capacity) {
   char word1[MAX], tmp[MAX]; /* Buffers for processing words in the line */
   int i, c, isLabel; /* Indices and flags for processing */

   i = 0, isLabel = FALSE; /* Initialize index and label flag */

   /* Skip empty lines and comment lines */
   if (row[0] == '\n' || row[0] == '\0' || row[0] == ';') {
      return TRUE;
   }

   /* Extract the first word from the line and check if there is extra/missing comma serounde it*/
   if (!copy_word_jump_space_count_coma(row, word1, &i, 0, 0, r))
      return FALSE;

   /* Handle `.entry` directive */
   if (strcmp(word1, ".entry") == 0) {
      i = copy_word_jump_space(row, word1, i); /* Get the next word */
      isLabel = symbols_table_management(word1, "entry", symbols_table, symbols_table_size, ADD_TYPE, 0, r, macro_table, macro_table_size, 0);
      if (isLabel) {
         return check_extra_word(row, i, r, "finishing an entry line");
      } else {
         return FALSE;
      }
   }

   /* Handle `.extern` directive */
   if (strcmp(word1, ".extern") == 0) {
      i = copy_word_jump_space(row, word1, i); /* Get the next word */
      isLabel = symbols_table_management(word1, "external", symbols_table, symbols_table_size, ADD_NAME, NO, r, macro_table, macro_table_size, 0);/*Add external symbol*/
      if (isLabel) {
         return check_extra_word(row, i, r, "finishing an extern line");/*Check if there is extra text at the end of the line*/
      } else {
         return FALSE;
      }
   }

   /* Handle data directives (`.string`, `.data`) */
   if (word1[0] == '.') {
      return write_data_code(row, data_code, data_capacity, r, i, word1);
   }

   /* Handle commands */
   if ((c = cmd_table(word1)) != -1) {
      return write_command_code(row, &i, c, cmd_code, cmd_capacity, r);
   }

   /* Handle labels followed by directives or commands */
   if (row[i] == ':' && row[i + 1] == ' ') {
      i = copy_word_jump_space(row, tmp, i + 1); /* Get the next word */
      
      /*Found data label:*/
      if (strcmp(tmp, ".string") == 0 || strcmp(tmp, ".data") == 0) {
         /* Add label to symbol table and process data directive */
         return symbols_table_management(word1, "data", symbols_table, symbols_table_size, ADD_NAME, DC, r, macro_table, macro_table_size, 0) &&
                write_data_code(row, data_code, data_capacity, r, i, tmp);
      } 
      
      /*Entry directive line:*/
      else if (strcmp(tmp, ".entry") == 0) {
         /* Warn about meaningless label before `.entry` */
         printf(" Attention - line %d: label defined at the beginning of an .entry line, is meaningless, and the assembler ignores it.\n", r);
         /* Add entry label to symbol table */
         return symbols_table_management(word1, "entry", symbols_table, symbols_table_size, ADD_TYPE, NO, r, macro_table, macro_table_size, NO);
      } 
      
      /*Extern directive line:*/
      else if (strcmp(tmp, ".extern") == 0) {
         /* Warn about meaningless label before `.extern` */
         printf(" Attention - line %d: label defined at the beginning of an .extern line, is meaningless, and the assembler ignores it.\n", r);
         return symbols_table_management(word1, "external", symbols_table, symbols_table_size, ADD_NAME, NO, r, macro_table, macro_table_size, NO);
      } 
      
      /*Found code label:*/
      else if ((c = cmd_table(tmp)) != NO) {
         /* Add code label to symbol table and process command */
         return symbols_table_management(word1, "code", symbols_table, symbols_table_size, ADD_NAME, IC, r, macro_table, macro_table_size, NO) &&
                write_command_code(row, &i, c, cmd_code, cmd_capacity, r);
      } 
      
      else {
         /* Invalid word after label */
         printf("Error - line %d: after the label must be a valid command or data declaration.\n", r);
         return FALSE;
      }
   }

   /* Invalid first word in the line */
   printf("Error - line %d: the first word (%s) is not valid: must be valid command, data declaration, label definition, or symbol directives.\n", r, word1);
   return FALSE;
}



static int read_row_first(const char *file, FILE *source, int **cmd_code, int **data_code, struct Symbol **symbols_table,
                    int *symbols_table_size, struct Macro *macro_table, int macro_table_size, int cmd_capacity, int data_capacity)
{
   char row[MAX]; /* Buffer to store the current row being read */
   int r, InputValidation;

   r = 1; /* Line counter starts at 1 */
   InputValidation = TRUE; /* Assume input is valid initially */

   while (fgets(row, MAX, source)) /* Read each line from the source file */
   {
      if (row_type_first(row, r, symbols_table, symbols_table_size, cmd_code, data_code,
                     macro_table, macro_table_size, &cmd_capacity, &data_capacity) == FALSE)
      {
         InputValidation = FALSE; /* Mark input as invalid if row processing fails */
      }
      r++; /* Increment line counter */
      row[0] = '\0'; /* Clear the row buffer for the next line */
   }

   return InputValidation; /* Return the overall validation result */
}




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
   int* DCF)
{
   FILE *source; /* Pointer to the source file */
   char amFilename[256]; /* Buffer to store the filename with ".am" extension */
   int input_validation, i; /* Variables for input validation and loop index */
   DC = 0;
   IC = 0; /* Initialize Data Counter (DC) and Instruction Counter (IC) */

   sprintf(amFilename, "%s%s", fileName, ".am"); /* Create the filename with ".am" extension */
   source = fopen(amFilename, "r"); /* Open the source file in read mode */
   if (source == NULL) { /* Check if the file failed to open */
      perror("Error opening source file"); /* Print error message */
      exit(EXIT_FAILURE); /* Exit the program with failure */
   }

   /* Read the source file and process rows in the first pass */
   input_validation = read_row_first(fileName, source, cmd_code, data_code, symbols_table,
      symbols_table_size, macro_table, macro_table_size, cmd_capacity, data_capacity);

   *ICF = IC; /* Set the final instruction counter value */
   for (i = 0; i < *symbols_table_size; i++) { /* Iterate over the symbols table */
      struct Symbol *pSymbol = &((*symbols_table)[i]); /* Pointer to the current symbol */
      if (pSymbol->type != NULL && strstr(pSymbol->type, "data") != NULL) {
         /* Adjust the address of data symbols */
         pSymbol->address += (*ICF + 100);
      } else if (pSymbol->type != NULL && strstr(pSymbol->type, "code") != NULL) {
         /* Adjust the address of code symbols */
         pSymbol->address += 100;
      } else if (pSymbol->type != NULL && strstr(pSymbol->type, "entry") != NULL) {
         /* Check if entry symbols have a defined address */
         if (pSymbol->address == NO) {
            printf("Error: the address of the entry symbol (%s) is not defined.\n", pSymbol->name);
            input_validation = FALSE; /* Mark input as invalid */
         }
      }
   }

   fclose(source); /* Close the source file */

   *DCF = DC; /* Set the final data counter value */
   return input_validation; /* Return the validation result */
}