/**
 * @file fixed_tables.c
 * @brief Contains the implementation of functions and data structures for handling command tables and reserved words.
 */



#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "fixed_tables.h"

struct cmd_struct cmd[] = {
   /* Command table with name, opcode, funct, source and destination addressing modes */
   {"mov", 0, 0, "013", "13", 000000, 000000},
   {"cmp", 0, 1, "013", "013", 000001, 000000},
   {"add", 1, 2, "013", "13", 000010, 000000},
   {"sub", 2, 2, "013", "13", 000010, 000010},
   {"lea", 0, 4, "1", "13", 000100, 000000},
   {"clr", 1, 5, NULL, "13", 000101, 000001},
   {"not", 2, 5, NULL, "13", 000101, 000010},
   {"inc", 3, 5, NULL, "13", 000101, 000011},
   {"dec", 4, 5, NULL, "13", 000101, 000100},
   {"jmp", 1, 9, NULL, "12", 001001, 000001},
   {"bne", 2, 9, NULL, "12", 001001, 000010},
   {"jsr", 3, 9, NULL, "12", 001001, 000011},
   {"red", 0, 12, NULL, "13", 001100, 000000},
   {"prn", 0, 13, NULL, "013", 001101, 000000},
   {"rts", 0, 14, NULL, NULL, 001110, 000000},
   {"stop", 0, 15, NULL, NULL, 001111, 000000},
};

int get_opcode(char* word) {
   int i;
   /* Iterate through the command table to find the matching command name */
   for (i = 0; i < 16; i++) {
     if (strcmp(word, cmd[i].name) == 0) {
       return cmd[i].opcode_bin; /* Return the binary opcode if found */
     }
   }
   return FALSE; /* Return FALSE if not found */
}

int get_funct(char* word) {
   int i;
   /* Iterate through the command table to find the matching command name */
   for (i = 0; i < 16; i++) {
     if (strcmp(word, cmd[i].name) == 0) {
       return cmd[i].funct; /* Return the funct value if found */
     }
   }
   return NO; /* Return NO if not found */
}

int cmd_table(char* word) {
   int i;
   /* Iterate through the command table to find the matching command name */
   for (i = 0; i < 16; i++) {
     if (strcmp(word, cmd[i].name) == 0) {
       return i; /* Return the index of the command in the table */
     }
   }
   return NO; /* Return NO if not found */
}

int reserved_word(char* word) {
   int i;
   /* Check if the word matches any command name in the command table */
   for (i = 0; i < 16; i++) {
     if (strcmp(word, cmd[i].name) == 0) {
       return TRUE; /* Return TRUE if the word is a command */
     }
   }

   /* Check if the word matches any reserved keyword */
   for (i = 0; i < 27; i++) {
     if (strcmp(word, reswords[i]) == 0) {
       return TRUE; /* Return TRUE if the word is reserved */
     }
   }
   return FALSE; /* Return FALSE if not reserved */
}

/* Array of reserved words including commands, registers, and keywords */
char reswords[27][8] = {
   "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop",
   "r1", "r2", "r3", "r4", "r5", "r6", "r7",
   "data", "string", "entry", "extern"
};
