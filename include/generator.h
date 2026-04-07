/**
 * @file generator.h
 * @brief Header for text generation logic (Task 2).
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include "map.h"
#include "analyzer.h"
#include "word.h"

/**
 * @brief Generates a pseudo-random sequence of words.
 * @param out The output stream (e.g., stdout or a file).
 * @param main_tree The analyzed frequency table.
 * @param start_word Optional starting word. If NULL, picks random punctuation.
 * @param num_words Total number of tokens to generate.
 */
void generator_generate_text(FILE* out, RB_Tree* main_tree, const char* start_word, int num_words);

/**
 * @brief Selects the next word structure from a successor context.
 * @return A pointer to the internal word_t key of the chosen successor node.
 */
const word_t* generator_pick_next_word(SuccessorContext* ctx);

#endif