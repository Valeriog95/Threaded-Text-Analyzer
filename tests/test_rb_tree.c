/**
 * @file test_rb_tree.c
 * @brief Testing the btree functionalities 
 */

 #include <stdio.h>
#include <assert.h>
#include <string.h>
#include "map.h"

void test_rb_insertion_and_search() {
    printf("Running test_rb_insertion_and_search... ");
    RB_Tree *tree = rb_tree_create();

    // Insert some fruits
    rb_tree_insert(tree, "banana", 3);
    rb_tree_insert(tree, "apple", 5);
    rb_tree_insert(tree, "orange", 2);

    //Test Search
    RB_Node *node = rb_tree_at(tree, "apple");
    assert(node != tree->nil);
    assert(node->value == 5);
    assert(strcmp(node->key.value, "apple") == 0);

    // Test non-existent key
    RB_Node *not_found = rb_tree_at(tree, "coconut");
    assert(not_found == tree->nil);

    printf("PASSED\n");
}

void test_rb_alphabetical_order() {
    printf("Running test_rb_alphabetical_order (Stress Test)... ");
    RB_Tree *tree = rb_tree_create();

    // Inserting in reverse alphabetical order to force rebalancing
    const char *words[] = {"zebra", "yak", "x-ray", "walrus", "vulture"};
    for (int i = 0; i < 5; i++) {
        rb_tree_insert(tree, words[i], i);
    }

    // If rb_tree_print uses In-Order traversal, it must show words
    // in alphabetical order regardless of insertion order.
    
    printf("\nVisual check of In-Order traversal:\n");
    rb_tree_print(tree); 

    printf("PASSED\n");
}

int main() {
    printf("--- STARTING RB_TREE TEST SUITE ---\n");

    test_rb_insertion_and_search();
    test_rb_alphabetical_order();

    printf("--- ALL RB_TREE TESTS PASSED ---\n");
    return 0;
}