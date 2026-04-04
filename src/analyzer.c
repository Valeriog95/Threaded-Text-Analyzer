#include "analyzer.h"
#include "parser.h"
#include "map_utils.h"
#include <stdlib.h>

/**
 * @brief Helper to free the context and its internal tree.
 */
void successor_context_delete(void* value) {
    if (!value) return;
    SuccessorContext* ctx = (SuccessorContext*)value;
    rb_tree_delete(ctx->tree); // Delete the nested tree
    free(ctx);                 // Delete the wrapper
}

/**
 * @brief Action to initialize the SuccessorContext wrapper.
 */
void action_ensure_successor_context(RB_Node* node, bool was_inserted) {
    if (was_inserted) {
        SuccessorContext* ctx = malloc(sizeof(SuccessorContext));
        if (ctx) {
            ctx->tree = rb_tree_int_create();
            ctx->total_entries = 0;
            node->value = ctx;
        }
    }
}

/**
 * @brief Action to manage the integer counter inside the successor tree.
 */
void action_increment_count(RB_Node* node, bool was_inserted) {

    if (was_inserted) {
        // Successor word not seen yet for this context: allocate and set to 1 
        int* count = (int*)malloc(sizeof(int));
        if (count) {
            *count = 1;
            node->value = count;
        }
    } else {
        // Successor exists: safely increment the existing counter
        int* count = (int*)node->value;
        if (count) (*count)++;
    }
}

/**
 * @brief Internal helper to update the frequency table for a word pair.
 */
static void update_frequency(RB_Tree* main_tree, const char* current, const char* next) {
    // 1. Ensure current_word has a dedicated SuccessorContext in main_tree 
    rb_tree_get_or_insert_execute(main_tree, (void*)current, action_ensure_successor_context);
    
    // 2. Retrieve the context (Safe because step 1 guaranteed its existence) 
    RB_Node* main_node = rb_tree_at(main_tree, (void*)current);
    SuccessorContext* ctx = (SuccessorContext*)main_node->value;

    // 3. Increment the frequency of next_word in current_word's context 
    rb_tree_get_or_insert_execute(ctx->tree, (void*)next, action_increment_count);

    // 4. OPTIMIZATION: Increment the global total of successors for this context
    ctx->total_entries++;
}

void analyzer_process_text(FILE *input, RB_Tree *main_tree) {
    if (!input || !main_tree) return;

    // Set the custom destructor for the main_tree to handle SuccessorContext
    main_tree->free_value = successor_context_delete;

    word_t current_word, next_word, first_word;
    bool has_first = false;

    // Get the starting word of the text 
    if (!parser_get_next_word(input, &current_word)) return;
    
    first_word = current_word;
    has_first = true;

    // Iterate through the text, processing word pairs (current -> next) 
    while (parser_get_next_word(input, &next_word)) {
        update_frequency(main_tree, current_word.value, next_word.value);
        current_word = next_word;
    }

    // Requirement: The last word of the text is followed by the first word 
    if (has_first) {
        update_frequency(main_tree, current_word.value, first_word.value);
    }
}