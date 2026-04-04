#include "analyzer.h"
#include "parser.h"
#include "map_utils.h"
#include <stdlib.h>


void action_ensure_successor_tree(RB_Node* node, bool was_inserted) {

    if (was_inserted) {
        RB_Tree* sub_tree = rb_tree_int_create();
        sub_tree->free_value = free;        
        node->value = sub_tree;
    }
}

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

void analyzer_process_text(FILE *input, RB_Tree *main_tree) {
    if (!input || !main_tree) return;

    word_t current_word, next_word, first_word;
    bool has_first = false;

    // Get the starting word of the text 
    if (!parser_get_next_word(input, &current_word)) return;
    
    first_word = current_word;
    has_first = true;

    // Iterate through the text, processing word pairs (current -> next) 
    while (parser_get_next_word(input, &next_word)) {
        
        // 1. Ensure current_word has a dedicated successor tree in main_tree 
        rb_tree_get_or_insert_execute(main_tree, current_word.value, action_ensure_successor_tree);
        
        // 2. Retrieve the successor tree (Safe because step 1 guaranteed its existence) 
        RB_Node* parent_node = rb_tree_at(main_tree, current_word.value);
        RB_Tree* successor_tree = (RB_Tree*)parent_node->value;

        // 3. Increment the frequency of next_word in current_word's context 
        rb_tree_get_or_insert_execute(successor_tree, next_word.value, action_increment_count);

        // Advance: the next word becomes the current context 
        current_word = next_word;
    }

    // Requirement: The last word of the text is followed by the first word 
    if (has_first) {
        rb_tree_get_or_insert_execute(main_tree, current_word.value, action_ensure_successor_tree);
        RB_Node* last_node = rb_tree_at(main_tree, current_word.value);
        rb_tree_get_or_insert_execute((RB_Tree*)last_node->value, first_word.value, action_increment_count);
    }
}