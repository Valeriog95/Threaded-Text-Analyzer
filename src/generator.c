/**
 * @file generator.c
 * @brief Implementation of the text generation logic (Task 2).
 * @details Uses Markov chains and frequency tables to produce pseudo-random text.
 */

#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Helper to print a word with an optional leading capital letter.
 * @param out Output stream.
 * @param str The string to print.
 * @param capitalize If true, the first character will be converted to uppercase.
 */
static void print_word_formatted(FILE* out, const char* str, bool capitalize) {
    if (!str || str[0] == '\0') return;

    if (capitalize && islower((unsigned char)str[0])) {
        fprintf(out, "%c%s", toupper((unsigned char)str[0]), str + 1);
    } else {
        fprintf(out, "%s", str);
    }
}

/**
 * @brief Checks if a string is a strong punctuation mark (sentence ender).
 * @param str String to check.
 * @return true if str is ".", "!", or "?".
 */
static bool is_strong_punctuation(const char* str) {
    return (strcmp(str, ".") == 0 || strcmp(str, "?") == 0 || strcmp(str, "!") == 0);
}

const word_t* generator_pick_next_word(SuccessorContext* ctx) {
    if (!ctx || ctx->total_entries == 0) return NULL;

    /* Generate a random number within the total frequency range */
    int r = rand() % (int)ctx->total_entries;
    int cumulative_count = 0;

    /* Iterate through the successor tree to find the word corresponding to the random pick */
    for (RB_Tree_Iterator it = rb_tree_it_begin(ctx->tree); 
         it != rb_tree_it_end(ctx->tree); 
         it = rb_tree_it_increment(ctx->tree, it)) 
    {
        /* The value in the successor tree is a pointer to the occurrence counter (int) */
        int count = *(int*)it->value;
        cumulative_count += count;

        if (r < cumulative_count) {
            /* Return the pointer to the word_t key stored in the RB_Node */
            return &(it->key); 
        }
    }
    return NULL;
}

void generator_generate_text(
    FILE* out,
    RB_Tree* main_tree,
    const char* start_word,
    int num_words) {

    if (!main_tree || num_words <= 0) return;
    
    const word_t* current_node_key = NULL;
    bool capitalize_next = true;

    /* 1. Find starting node (Context) */
    if (start_word != NULL) {
        RB_Node* n = rb_tree_at(main_tree, (void*)start_word);
        if (n != main_tree->nil) current_node_key = &(n->key);
    }

    /* Fallback to random punctuation if start_word is missing or not found */
    if (current_node_key == NULL) {
        const char* puncts[] = {".", "?", "!"};
        int offset = rand() % 3;
        for (int i = 0; i < 3; i++) {
            RB_Node* n = rb_tree_at(main_tree, (void*)puncts[(offset + i) % 3]);
            if (n != main_tree->nil) {
                current_node_key = &(n->key);
                break;
            }
        }
    }

    if (!current_node_key) return;

    /* If starting context is strong punctuation, the first generated word must be capitalized */
    capitalize_next = is_strong_punctuation(current_node_key->value);

    /* 2. Generation Loop */
    for (int i = 0; i < num_words; i++) {
        
        RB_Node* main_node = rb_tree_at(main_tree, current_node_key->value);
        
        if (main_node == main_tree->nil) break;

        SuccessorContext* ctx = (SuccessorContext*)main_node->value;
        
        const word_t* next = generator_pick_next_word(ctx);
        
        if (!next) break;

        /* Update current context with the newly picked successor */
        current_node_key = next;

        /* Print with leading space (except for the first word) */
        if (i > 0) fprintf(out, " ");
        print_word_formatted(out, current_node_key->value, capitalize_next);

        /* Determine if the next word should be capitalized */
        capitalize_next = is_strong_punctuation(current_node_key->value);
    }
    fprintf(out, "\n");
}