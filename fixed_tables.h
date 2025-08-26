#ifndef FIXED_TABLES_H
#define FIXED_TABLES_H
#include "auxiliary_functions_constants.h"



/**
 * struct cmd_struct - Represents a command structure with its properties.
 * @name: The name of the command as a string.
 * @funct: The function code associated with the command (integer value).
 * @opcode: The opcode of the command (integer value).
 * @source: A string representing the allowed source addressing modes.
 * @dest: A string representing the allowed destination addressing modes.
 * @opcode_bin: A string representing the binary representation of the opcode.
 * @funct_bin: A string representing the binary representation of the function code.
 *
 * This structure is used to define the properties of a command in the system,
 * including its name, opcode, function code, and addressing modes.
 */
struct cmd_struct {
    char *name;
    int funct;
    int opcode;
    char *source;
    char *dest;
    int opcode_bin;
    int funct_bin;
}; 





/**
 * @var cmd
 * @brief Array of command structures.
 *
 * This array contains 16 command structures, each representing a specific
 * command in the system. The structure and its fields are defined elsewhere.
 */
extern struct cmd_struct cmd[16];




/**
 * @fn char *get_opcode(char *word)
 * @brief Retrieves the opcode associated with a given command word.
 *
 * @param word A string representing the command word.
 * @return A pointer to a string containing the opcode if the word is valid,
 *         or NULL if the word is not recognized.
 *
 * This function searches for the given command word in the command table
 * and returns its corresponding opcode. It assumes that the input word
 * is a valid null-terminated string.
 */
int get_opcode(char *word);



/**
 * @fn int get_funct(char *word)
 * @brief Retrieves the function code (funct) associated with a given command word.
 *
 * @param word A string representing the command word.
 * @return An integer representing the function code if the word is valid,
 *         or -1 if the word is not recognized.
 *
 * This function searches for the given command word in the command table
 * and returns its corresponding function code. It assumes that the input
 * word is a valid null-terminated string.
 */

int get_funct(char *word);





/**
 * @fn int cmd_table(char *word)
 * @brief Checks if a given word exists in the command table.
 *
 * @param word A string representing the command word.
 * @return 1 if the word exists in the command table, 0 otherwise.
 *
 * This function determines whether the provided word matches any of the
 * commands in the command table. It assumes that the input word is a valid
 * null-terminated string.
 */

int cmd_table(char *word);




/**
 * @fn int reserved_word(char *word)
 * @brief Checks if a given word is a reserved word.
 *
 * @param word A string representing the word to check.
 * @return 1 if the word is a reserved word, 0 otherwise.
 *
 * This function searches for the given word in the reserved words table
 * and determines whether it is a reserved word. It assumes that the input
 * word is a valid null-terminated string.
 */

int reserved_word(char *word);



/**
 * @var reswords
 * @brief Array of reserved words.
 *
 * This 2D array contains 27 reserved words, each with a maximum length
 * of 7 characters (plus a null terminator). These words are predefined
 * and cannot be used as identifiers or commands in the system.
 */

extern char reswords[27][8];

#endif /* FIXED_TABLES_H */