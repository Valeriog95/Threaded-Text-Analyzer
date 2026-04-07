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

void generator_generate_text(FILE* out, RB_Tree* main_tree, const char* start_word, int num_words) {
    if (!main_tree || num_words <= 0) return;

    const word_t* current_word = NULL;
    bool capitalize_next = true; /* Sentences always start with a capital letter */

    /* --- INITIAL SELECTION --- */
    if (start_word != NULL) {
        RB_Node* node = rb_tree_at(main_tree, (void*)start_word);
        if (node != main_tree->nil) {
            current_word = &(node->key);
        }
    } 
    
    /* If no start_word provided or not found, pick a random punctuation from the table */
    if (current_word == NULL) {
        const char* puncts[] = {".", "?", "!"};
        int offset = rand() % 3;
        
        for (int i = 0; i < 3; i++) {
            RB_Node* node = rb_tree_at(main_tree, (void*)puncts[(offset + i) % 3]);
            if (node != main_tree->nil) {
                current_word = &(node->key);
                break;
            }
        }
    }

    /* Fallback: if tree is empty or no punctuation found, abort */
    if (current_word == NULL) {
        fprintf(stderr, "[Generator Error] Initial word or punctuation not found.\n");
        return;
    }

    /* --- GENERATION LOOP --- */
    for (int i = 0; i < num_words; i++) {
        /* 1. Spacing logic: do not add space before strong punctuation or at the very start */
        if (i > 0 && !is_strong_punctuation(current_word->value)) {
            fprintf(out, " ");
        }

        /* 2. Print the word with proper capitalization */
        print_word_formatted(out, current_word->value, capitalize_next);

        /* 3. Update capitalization flag for the next word */
        capitalize_next = is_strong_punctuation(current_word->value);

        /* 4. Exit if we reached the word limit */
        if (i == num_words - 1) break;

        /* 5. Find the current word node in the main tree to access its successors */
        RB_Node* main_node = rb_tree_at(main_tree, (void*)current_word->value);
        if (main_node == main_tree->nil) break;

        SuccessorContext* ctx = (SuccessorContext*)main_node->value;
        const word_t* next_word = generator_pick_next_word(ctx);

        if (!next_word) break;
        current_word = next_word;
    }

    fprintf(out, "\n");
}