#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "auxiliary_functions_constants.h"

/**
 * @brief Validates the number of commas in a row according to specified rules.
 * @param row Pointer to the input string to check.
 * @param i Pointer to the current index in the string.
 * @param comaValidation Expected number of commas.
 * @param r Line number for error reporting.
 * @return TRUE if comma validation passes, FALSE otherwise.
 */
static int coma_validation(char* row, int* i, int comaValidation, int r);


char* myStrdup(const char* s) {
   int length;                  /* Length of input string including null terminator */
   char* d = NULL;              /* Pointer to duplicated string */
   length = strlen(s) + 1;      /* Calculate length of string */
   d = malloc(length);          /* Allocate memory */
   if (d == NULL) return NULL;  /* Return NULL if malloc fails */
   strcpy(d, s);                /* Copy the string */
   return d;                    /* Return the duplicated string */
}


int copy_word(char* row, char* word, int i) {
   int j;                       /* Index for word buffer */
   for (j = 0; j < MAX_MACRO_NAME - 1 && !isspace(row[i]) && row[i] != '\0' && row[i] != ':' && row[i] != ','; i++, j++) { /* Copy word */
      word[j] = row[i];         /* Copy character to word buffer */
   }
   word[j] = '\0';              /* Null-terminate the word */
   return i;                    /* Return updated index */
}


int copy_word_jump_space(char* row, char* word, int i) {
   int j;                       /* Index for word buffer */

   while (isspace(row[i]))      /* Skip leading spaces */
      i++;
   for (j = 0; j < MAX - 1 && !isspace(row[i]) && row[i] != '\0' && row[i] != ':' && row[i] != ','; i++, j++) { /* Copy word */
      word[j] = row[i];         /* Copy character to word buffer */
   }
   word[j] = '\0';              /* Null-terminate the word */

   if (row[i] == ',')           /* Check for trailing comma */
      i++;                      /* Skip comma */
   return i;                    /* Return updated index */
}





static int coma_validation(char* row, int* i, int comaValidation, int r) {
   int coma;                     /* Counter for commas */
   coma = 0;                     /* Initialize comma counter */

   while (*i < MAX && row[*i] != '\0' && ((isspace(row[*i]) && row[*i] != '\n' && row[*i] != '\0') 
         || row[*i] == ',')) {   /* Loop through spaces or commas */
      if (row[*i] == ',') {      /* Check for comma */
         (coma)++;               /* Increment comma counter */
      }
      (*i)++;                    /* Move to next character */
   }

   if (row[*i] == '\n' || row[*i] == '\0') { /* Check for end of line or string */
      if (coma == 0) {           /* No commas found */
         return TRUE;            /* Valid case */
      }
      printf("Error - line %d: invaild extra comma at the end of the line.\n", r); /* Report extra comma error */
      return FALSE;              /* Invalid case */
   }

   if (coma < comaValidation) {  /* Too few commas */
      printf("Error - line %d: missing a comma.\n", r); /* Report missing comma error */
      return FALSE;              /* Invalid case */
   }
   if (coma > comaValidation) {  /* Too many commas */
      printf("Error - line %d: invalid extra comma.\n", r); /* Report extra comma error */
      printf("A comma must appear only once in a command line, once between every pair of numbers in a data line, and never immediately after the first word in a line.\n"); /* Additional error details */
      return FALSE;              /* Invalid case */
   }

   return TRUE;                 /* Validation successful */
}



int copy_word_jump_space_count_coma(char* row, char* word, int* i, int comaValidationBefor, int comaValidationAfter, int r) {
   int j;                       /* Index for word buffer */

   if (coma_validation(row, i, comaValidationBefor, r) == FALSE) { /* Validate commas before word */
      return FALSE;             /* Return on validation failure */
   }

   for (j = 0; *i < MAX - 1 && j < MAX - 1 && !isspace(row[*i]) && row[*i] != '\0' && row[*i] != ':' && row[*i] != ','; (*i)++, j++) { /* Copy word */
      word[j] = row[*i];        /* Copy character to word buffer */
   }
   word[j] = '\0';              /* Null-terminate the word */

   return coma_validation(row, i, comaValidationAfter, r); /* Validate commas after word */
}



int check_extra_word(char* row, int i, int r, char* after) {
   int k; /*index*/
   char word1[MAX] = { 0 };     /* Buffer for extracted word */
   copy_word_jump_space(row, word1, i); /* Extract word from current position */

   for (k = 0; k < strlen(word1); k++) { /* Check each character in extracted word */
      if (!isspace(word1[k])) { /* Non-space character found */
         printf("Error - line %d: illegal extra characters (%s) after %s.\n", r, word1, after); /* Report error */
         return FALSE;          /* Invalid case */
      }
   }

   return TRUE;                 /* No extra words found */
}



int check_number(char* word) {
   int i = 0;                    /* Index for string traversal */
   while (word[i] != '\0') {     /* Loop until end of string */
      if (!(isdigit(word[i]) || word[i] == '-' || word[i] == '+')) { /* Check for valid characters */
         return FALSE;           /* Invalid character found */
      }
      i++;                      /* Move to next character */
   }
   return TRUE;                 /* String is a valid number */
}




int ensure_capacity(int** code, int* code_capacity, int num) {
   if (num >= *code_capacity) {  /* Check if resizing is needed */
      *code_capacity *= 2;       /* Double the capacity */
      int* new_code = realloc(*code, (*code_capacity) * sizeof(int)); /* Allocate new memory */
      if (new_code == NULL) {    /* Check for allocation failure */
         perror("Error reallocating memory for data_code array"); /* Print error */
         exit(EXIT_FAILURE);     /* Exit on failure */
      }
      *code = new_code;          /* Update pointer to new memory */
   }
   return TRUE;                  /* Operation successful */
}




void free_symbols_table(struct Symbol* symbols_table, int size) {
   int i;                        /* Loop index */
   if (symbols_table == NULL)     /* Check for NULL table */
       return;                   /* Exit if table is NULL */

   for (i = 0; i < size; i++) {  /* Loop through table */
       if (symbols_table[i].extern_address != NULL) { /* Check for allocated extern address */
           free(symbols_table[i].extern_address); /* Free extern address */
           symbols_table[i].extern_address = NULL; /* Set to NULL */
       }
   }

   free(symbols_table);          /* Free the table itself */
}



int expand_symbols_table(struct Symbol** symbols_table, int* symbols_table_size) {
   int j;                        /* Loop index */
   struct Symbol* new_table;     /* Pointer to new table */

   *symbols_table_size *= 2;     /* Double the table size */
   new_table = realloc(*symbols_table, (*symbols_table_size) * sizeof(struct Symbol)); /* Allocate new memory */
   if (new_table == NULL) {      /* Check for allocation failure */
       perror("Error reallocating memory for symbols table"); /* Print error */
       exit(EXIT_FAILURE);        /* Exit on failure */
   }
   /* Initialize the new half of the table */
   for (j = (*symbols_table_size) / 2; j < (*symbols_table_size); j++) { /* Loop through new entries */
       memset(new_table[j].name, 0, sizeof(new_table[j].name)); /* Clear name field */
       memset(new_table[j].type, 0, sizeof(new_table[j].type)); /* Clear type field */
       new_table[j].extern_address = NULL; /* Set extern address to NULL */
   }

   *symbols_table = new_table;   /* Update pointer to new table */
   return TRUE;                  /* Operation successful */
}