/**
 * @file generator.h
 * @author Valeriog95
 * @brief Header for the text generation logic.
 * @details Uses the frequency table (RB-Tree) to generate a sequence of words
 * based on relative probabilities (Markov Chain).
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "map.h"
#include "analyzer.h"

/**
 * @brief Generates a pseudo-random sequence of words.
 * @param main_tree The analyzed frequency table.
 * @param start_word The word to begin the generation from.
 * @param num_words Maximum number of words to generate.
 */
void generator_generate_text(RB_Tree* main_tree, const char* start_word, int num_words);

/**
 * @brief Selects the next word from a successor tree based on its frequencies.
 * @param ctx The SuccessorContext containing the subtree and total count.
 * @return A pointer to the string (key) of the chosen successor.
 */
const char* generator_pick_next_word(SuccessorContext* ctx);

#endif