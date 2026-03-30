#include <stdio.h>
#include <stdlib.h>
#include "map_utils.h"

/* --- Static Helpers (Private to this file) --- */

/**
 * @brief Callback to free heap-allocated integers.
 */
static void free_int_value(void* value) {
    if (value) {
        free(value);
    }
}

/**
 * @brief Callback to convert integer pointers to strings for printing.
 * @note Uses a static buffer; not thread-safe for concurrent printing.
 */
static const char* int_value_to_string(void* value) {
    static char buf[12];
    if (!value) return "0";
    sprintf(buf, "%d", *(int*)value);
    return buf;
}

/* --- Public Functions --- */

RB_Tree* rb_tree_int_create() {
    RB_Tree* tree = rb_tree_create();
    if (tree) {
        tree->free_value = free_int_value;
        tree->value_as_string = int_value_to_string;
    }
    return tree;
}

void rb_tree_int_get_or_insert(RB_Tree *tree, const char *key, int value) {
    
    int* value_ = (int*)malloc(sizeof(int));
    
    if (!value_) return;
    *value_ = value;
    
    RB_Node* node = rb_tree_get_or_insert(tree, key, value_);
    
    // Check if previous pointer is the same of the preiovus one
    // to understand if it was only get or inserted
    if (node->value != value_) free(value_); 
    
}