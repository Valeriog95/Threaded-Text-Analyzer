#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include "word.h"
#include "map.h"

/**
 * @brief Reads the next valid token and encapsulates it into a word_t.
 * @param file Input stream.
 * @param out_word Pointer to the word_t structure to fill.
 * @return true if a word/punctuation was successfully parsed, false on EOF.
 */
bool parser_get_next_word(FILE *file, word_t *out_word);

/**
 * @brief Exports the nested RB_Tree structure to a CSV format.
 * @details Format: word,successor1,freq1,successor2,freq2...
 * @param out The output stream (file or stdout).
 * @param main_tree The populated nested frequency table.
 */
void parser_export_csv(FILE *out, RB_Tree *main_tree);

/**
 * @brief Imports a frequency table from a CSV file into the RB-Tree.
 * @details Validates that the sum of frequencies for each line is 1.0.
 * @return 0 on success, or an error code if the file is invalid.
 */
int parser_import_csv(FILE* input, RB_Tree* tree);

#endif