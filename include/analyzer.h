/**
 * @file analyzer.h
 * @author Valeriog95
 * @brief Header for text analysis logic (Task 1).
 * @details This module manages the construction of a nested Red-Black Tree 
 * frequency table. Each word in the main tree points to another RB-Tree 
 * containing its successors and their relative counts.
 */

#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdio.h>
#include "map.h"

/**
 * @brief Main entry point for Task 1 of the Text Anlyzewr. 
 * @details Reads tokens from the input file using the parser and updates 
 * the nested frequency table. It ensures the last word points back to the first.
 * @param input Open file stream for the source text.
 * @param main_tree The primary RB-Tree where keys are words and values are RB-Trees for counters(int).
 */
void analyzer_process_text(FILE *input, RB_Tree *main_tree);

/**
 * @brief Atomic action to initialize a successor tree.
 * @details Used as a callback for rb_tree_get_or_insert_execute on the main_tree.
 * @param node The node representing a word in the main tree.
 * @param was_inserted Boolean flag indicating if the node is new.
 */
void action_ensure_successor_tree(RB_Node* node, bool was_inserted);

/**
 * @brief Atomic action to update a successor count.
 * @details Used as a callback for rb_tree_get_or_insert_execute on the successor tree.
 * @param node The node representing a successor word.
 * @param was_inserted Boolean flag; if true, initializes count to 1, else increments existing count.
 */
void action_increment_count(RB_Node* node, bool was_inserted);

#endif