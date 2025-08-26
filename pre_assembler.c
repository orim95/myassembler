#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "pre_assembler.h"


/**
 * @brief Expands the macro table by doubling its size
 * 
 * This function doubles the size of the macro table by reallocating memory
 * and initializes the new memory slots to zero. If reallocation fails,
 * the program terminates with an error message.
 *
 * @param macro_table Pointer to the pointer of the macro table to be expanded
 * @param macro_table_size Pointer to the current size of the macro table
 * @return struct Macro* Pointer to the newly allocated macro table
 * @note The original macro table size is doubled
 * @warning Exits program if memory reallocation fails
 */
static struct Macro* expand_macro_table(struct Macro** macro_table, int* macro_table_size);

/**
 * @brief Manages the macro table by adding, retrieving, or performing other actions on macros.
 *
 * @param name The name of the macro to manage.
 * @param body The body of the macro (used when adding a new macro).
 * @param macro_table A pointer to the macro table (array of Macro structures).
 * @param macro_table_size A pointer to the size of the macro table.
 * @param action The action to perform (e.g., add, retrieve, etc.).
 * @param dest The destination file for writing (if applicable).
 * @return int Returns a status code indicating success or failure.
 *
 * @note Assumes that the macro table is dynamically allocated and can grow as needed.
 *       The function handles memory allocation for new macros.
 */
static int macro_table_management(char* name, char* body, struct Macro** macro_table, int* macro_table_size, int action, FILE* dest);

/**
 * @brief Adds a macro name to the macro table, and check it's validtaion.
 *
 * @param r The current row number in the source file (used for error reporting).
 * @param name The name of the macro to add.
 * @param macro_table A pointer to the macro table (array of Macro structures).
 * @param macro_table_size A pointer to the size of the macro table.
 * @return int Returns a status code indicating success or failure.
 *
 * @note Assumes that the macro name is unique and does not already exist in the table.
 *       If the table is full, it will be resized to accommodate the new macro.
 */
static int add_macro_name(int r, char* name, struct Macro** macro_table, int* macro_table_size);

/**
 * @brief Processes a row that defines a macro, extracting its name and body.
 *
 * @param row The current row from the source file.
 * @param r The current row number in the source file (used for error reporting).
 * @param macro_table A pointer to the macro table (array of Macro structures).
 * @param macro_table_size A pointer to the size of the macro table.
 * @param word A buffer to store the extracted macro name.
 * @return int Returns a status code indicating success or failure.
 *
 * @note Assumes that the row contains a valid macro definition.
 *       The function will parse the row and update the macro table accordingly.
 */
static int macro_definition_row(char* row, int r, struct Macro** macro_table, int* macro_table_size, char* word, int* isDefinition);

/**
 * @brief Writes a row to the destination file, expanding macros if necessary.
 *
 * @param row The current row from the source file.
 * @param dest The destination file for writing.
 * @param r The current row number in the source file (used for error reporting).
 * @param macro_table A pointer to the macro table (array of Macro structures).
 * @param macro_table_size A pointer to the size of the macro table.
 * @param macro_name A buffer to store the name of the macro being expanded (if applicable).
 *
 * @note Assumes that the row may contain a macro invocation.
 *       If a macro is found, its body will be written to the destination file instead of the macro name.
 */
static int write_row_pre(char* row, FILE* dest, int r, struct Macro** macro_table, int* macro_table_size, char* macro_name);


/**
 * @brief Expands the macro table by doubling its size
 * 
 * This function doubles the size of the macro table by reallocating memory
 * and initializes the new memory slots to zero. If reallocation fails,
 * the program terminates with an error message.
 *
 * @param macro_table Pointer to the pointer of the macro table to be expanded
 * @param macro_table_size Pointer to the current size of the macro table
 * @return struct Macro* Pointer to the newly allocated macro table
 * @note The original macro table size is doubled
 * @warning Exits program if memory reallocation fails
 */
static struct Macro* expand_macro_table(struct Macro** macro_table, int* macro_table_size) {
   struct Macro* new_table;
   int old_size = *macro_table_size;
   
   *macro_table_size *= 2; /* Double the table size for reallocation */
   new_table = realloc(*macro_table, (*macro_table_size) * sizeof(struct Macro)); /* Reallocate memory */
   
   if (new_table == NULL) { /* Check if reallocation failed */
      perror("Error reallocating memory for macro table");
      exit(EXIT_FAILURE);
   }
   
   /* Initialize new memory slots */
   memset(&new_table[old_size], 0, ((*macro_table_size) - old_size) * sizeof(struct Macro));
   
   return new_table;
}

static int macro_table_management(char* name, char* body, struct Macro** macro_table, int* macro_table_size, int action, FILE* dest) {
   int i; /* Loop index for traversing the macro table */

   if (action == ADD_NAME) { /* Handle adding a new macro name */
      for (i = 0; i < *macro_table_size; i++) {
         if ((*macro_table)[i].name == NULL) { /* Find an empty slot in the table */
            (*macro_table)[i].name = myStrdup(name); /* Duplicate the macro name */
            return TRUE; /* Successfully added the macro name */
         }
      }
      
      *macro_table = expand_macro_table(macro_table, macro_table_size); /* Update the macro table pointer */
      (*macro_table)[i].name = myStrdup(name); /* Add the new macro name */
      return TRUE; /* Successfully added the macro name */
   }

   else if (action == FIND_NAME) { /* Handle finding a macro name */
      for (i = 0; i < *macro_table_size; i++) {
         if ((*macro_table)[i].name != NULL && strcmp((*macro_table)[i].name, name) == 0) { /* Check for a match */
            if (body == NULL) { /* If no body is provided, return success */
               return TRUE;
            }
            if ((*macro_table)[i].body == NULL) { /* If the macro body is empty */
               (*macro_table)[i].body = myStrdup(body); /* Duplicate the body */
            }
            else { /* Append the new body to the existing body */
               char* new_body = malloc(strlen((*macro_table)[i].body) + strlen(body) + 1); /* Allocate memory for concatenation */
               if (new_body == NULL) { /* Check if allocation failed */
                  perror("Error allocating memory for macro body");
                  exit(EXIT_FAILURE);
               }
               strcpy(new_body, (*macro_table)[i].body); /* Copy the existing body */
               strcat(new_body, body); /* Append the new body */
               free((*macro_table)[i].body); /* Free the old body */
               (*macro_table)[i].body = new_body; /* Update the body pointer */
            }
            return TRUE; /* Successfully updated the macro body */
         }
      }
      return FALSE; /* Macro name not found */
   }

   
   else if (action == PRINT) { /* Handle printing a macro body */
      char* trimmed_name = myStrdup(name); /* Duplicate the macro name */
      if (trimmed_name != NULL) {
         trimmed_name[strcspn(trimmed_name, "\n")] = '\0'; /* Remove trailing newline */
         for (i = 0; i < *macro_table_size; i++) {
            if ((*macro_table)[i].name != NULL) { /* Check if the slot is not empty */
               if (strcmp((*macro_table)[i].name, trimmed_name) == 0) { /* Check for a match */
                  fputs((*macro_table)[i].body, dest); /* Write the macro body to the destination */
                  free(trimmed_name); /* Free the duplicated name */
                  return TRUE; /* Successfully printed the macro body */
               }
            }
         }
         free(trimmed_name); /* Free the duplicated name if no match is found */
      }
   }
   return FALSE; /* Action not successful */
}

static int add_macro_name(int r, char* name, struct Macro** macro_table, int* macro_table_size) {
   int i; /* Loop index for validating the macro name */

   /* Check if the macro name exceeds the maximum allowed length */
   if (strlen(name) > MAX_MACRO_NAME) {
      printf("Error - line %d: the macro name (%s) is too long.\n", r, name);
      return FALSE;
   }

   /* Check if the macro name is a reserved word */
   if (reserved_word(name)) {
      printf("Error - line %d: the macro name (%s) is a reserved word.\n", r, name);
      return FALSE;
   }

   /* Check if the macro name starts with a valid character (letter or underscore) */
   if (!isalpha(name[0]) && name[0] != '_') {
      printf("Error - line %d: the macro name (%s) is not valid.\n", r, name);
      return FALSE;
   }

   /* Validate the rest of the macro name (alphanumeric or underscore) */
   for (i = 1; name[i] != '\0'; i++) {
      if (!isalnum(name[i]) && name[i] != '_') {
         printf("Error - line %d: the macro name (%s) is not valid.\n", r, name);
         return FALSE;
      }
   }

   /* Add the macro name to the macro table */
   return macro_table_management(name, NULL, macro_table, macro_table_size, ADD_NAME, NULL);
}


static int macro_definition_row(char* row, int r, struct Macro** macro_table, int* macro_table_size, char* word, int* isDefinition) {
   int i; /* Index for traversing the row */
   char* pos; /* Pointer to locate the "mcro" keyword in the row */

   /* Check if the row contains the "mcro" keyword */
   pos = strstr(row, "mcro ");
   if (pos != NULL) {
      if (pos != row) { /* Ensure "mcro" starts at the beginning of the line */
         printf("Error - line %d: macro definition must start at the beginning of the line.\n", r);
         printf("The line text: %s", row);
         return FALSE;
      }

      /* Extract the macro name after "mcro" */
      i = copy_word(row, word, MCRO_LENGTH+1);
      if (row[i] != '\n') { /* Ensure no extra characters after the macro name */
         printf("Error - line %d: additional characters on a line after finishing a macro definition.\n", r);
         printf("The line text: %s", row);
         return FALSE;
      }

      /* Add the macro name to the macro table and set the definition flag */
      *isDefinition = add_macro_name(r, word, macro_table, macro_table_size);
      return *isDefinition; /* Return the status of adding the macro name */
   }
   return TRUE; /* Return TRUE if no macro definition is found (so there are no errors in macro definition)*/
}

static int write_row_pre(char* row, FILE* dest, int r, 
   struct Macro** macro_table, int* macro_table_size, char* macro_name) {
   char* i; /* Pointer for string operations */
   int isDefinition; /* Flag to indicate if a macro definition is being processed */

   isDefinition = FALSE; /* Initialize the flag to FALSE */

   /* Check if currently inside a macro definition */
   if (macro_name[0] != '\0') {
      /* Look for the end of the macro definition ("mcroend") */
      i = strstr(row, "mcroend");
      if ((i != NULL) && (i == row || isspace(*(i - 1))) && (isspace(*(i + MACRO_END_LENGTH)))) {
         isDefinition = FALSE; /* Reset the flag as the macro definition ends */
         if (i != row) { /* Ensure "mcroend" starts at the beginning of the line */
            printf("Error - line %d: macro end must start at the beginning of the line.\n", r);
            printf("The line text: %s", row);
            return FALSE;
         }
         if (*(i + MACRO_END_LENGTH) != '\n' && *(i + MACRO_END_LENGTH) != '\0') { /* Check for extra characters after "mcroend" */
            printf("Error - line %d: additional characters on a line after 'mcroend'.\n", r);
            printf("The line text: %s", row);
            return FALSE;
         }
         macro_name[0] = '\0'; /* Clear the macro name to indicate no active macro */
         return TRUE;
      }
      /* Add the current row to the macro body in the macro table */
      return macro_table_management(macro_name, row, macro_table, macro_table_size, FIND_NAME, dest);
   }

   /* Check if the row defines a new macro */
   if (macro_definition_row(row, r, macro_table, macro_table_size, macro_name, &isDefinition) == FALSE) {
      return FALSE; /* Return FALSE if the macro definition is invalid */
   }
   if (isDefinition == TRUE) { /* If a macro definition starts, skip further processing */
      return TRUE;
   }

   /* Check if the row is a macro invocation and print its body */
   if (macro_table_management(row, NULL, macro_table, macro_table_size, PRINT, dest) == TRUE) {
      return TRUE;
   }

   /* If the row is neither a macro definition nor invocation, write it as-is */
   fputs(row, dest);
   return TRUE;
}

int read_row_pre(const char* file, FILE* source, FILE* dest, struct Macro** macro_table, int* macro_table_size) {
   int r, row_size, inputValidation; /* r: line number, row_size: buffer size for row, inputValidation: status flag */
   char macro_name[MAX_MACRO_NAME] = {0}; /* Stores the current macro name being processed */
   char* temp; /* Temporary pointer for reallocating memory */
   
   r = 1, row_size = INITIAL_ROW_SIZE, inputValidation = TRUE; /* Initialize variables */
   
   char* row = (char*)malloc(row_size * sizeof(char)); /* Allocate memory for the row buffer */
   if (row == NULL) { /* Check if memory allocation failed */
      perror("Error allocating memory for row");
      exit(EXIT_FAILURE);
   }

   while (fgets(row, row_size, source)) { /* Read each line from the source file */
      if(row[0] == '\0' || row[0] == '\n' || row[0] == ';') { /* Skip empty lines or comments */
         continue;
      }
      while (strlen(row) == row_size - 1 && row[row_size - 2] != '\n') { /* Handle long lines */
         row_size *= 2; /* Double the buffer size */
         temp = (char*)realloc(row, row_size * sizeof(char)); /* Reallocate memory for the row buffer */
         if (temp == NULL) { /* Check if memory reallocation failed */
            free(row);
            perror("Error reallocating memory for row");
            exit(EXIT_FAILURE);
         }
         row = temp;
         if (!fgets(row + row_size/2 - 1, row_size/2 + 1, source)) { /* Read remaining content */
            break;
         }
      }

      if (strlen(row) >= INITIAL_ROW_SIZE - 1) { /* Check if the line exceeds the initial buffer size */
         printf("Error - line %d: line is too long.\n", r);
         printf("The line text: %s", row);
         row[INITIAL_ROW_SIZE - 1] = '\n'; /* Truncate the line */
         row[INITIAL_ROW_SIZE] = '\0'; /* Null-terminate the string */
      }

      /* Remove trailing \r if exists */
      if (row[strlen(row)-1] == '\r')
         row[strlen(row)-1] = '\0';
      if(write_row_pre(row, dest, r, macro_table, macro_table_size, macro_name) == FALSE) { /* Process the row */
         inputValidation = FALSE; /* Update status flag if processing fails */
      }
      r++; /* Increment the line number */
   }

   free(row); /* Free the allocated memory for the row buffer */

   return inputValidation; /* Return the status of the processing */
}
