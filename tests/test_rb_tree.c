/**
 * @file test_rb_tree.c
 * @brief Unit tests for the Red-Black Tree implementation.
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "map.h"

/**
 * @brief Helper to calculate black height and verify RBT property #5.
 * @return The black height of the subtree.
 */
int verify_black_height(RB_Tree *tree, RB_Node *node) {
    if (node == tree->nil) return 1;

    int left_bh = verify_black_height(tree, node->left);
    int right_bh = verify_black_height(tree, node->right);

    /* Property 5: Every path from a node to its descendant NIL nodes 
       must contain the same number of black nodes. */
    assert(left_bh == right_bh);

    return (node->color == BLACK) ? left_bh + 1 : left_bh;
}

/**
 * @brief Tests basic insertion and retrieval.
 */
void test_rb_insertion_and_search() {
    printf("Running test_rb_insertion_and_search... ");
    RB_Tree *tree = rb_tree_create();

    rb_tree_insert(tree, "banana", 3);
    rb_tree_insert(tree, "apple", 5);
    
    RB_Node *node = rb_tree_at(tree, "apple");
    assert(node != tree->nil);
    assert(node->value == 5);
    assert(strcmp(node->key.value, "apple") == 0);

    printf("PASSED\n");
}

/**
 * @brief Validates RBT structural invariants (Root color and Black Height).
 */
void test_rb_structural_invariants() {
    printf("Running test_rb_structural_invariants... ");
    RB_Tree *tree = rb_tree_create();
    const char *data[] = {"mela", "pera", "banana", "kiwi", "uva"};

    for (int i = 0; i < 5; i++) {
        rb_tree_insert(tree, data[i], 1);
    }

    /* Property 2: The root must be BLACK */
    assert(tree->root->color == BLACK);

    /* Property 5: Check black height consistency */
    verify_black_height(tree, tree->root);

    printf("PASSED\n");
}

/**
 * @brief Tests that duplicate keys are ignored (current implementation logic).
 */
void test_duplicate_key_policy() {
    printf("Running test_duplicate_key_policy... ");
    RB_Tree *tree = rb_tree_create();

    /* Initial insertion */
    rb_tree_insert(tree, "test", 10);
    
    /* Attempt to insert same key with different value */
    rb_tree_insert(tree, "test", 20);

    RB_Node *node = rb_tree_at(tree, "test");
    
    /* Value should still be 10 because duplicates are ignored */
    assert(node->value == 10);

    printf("PASSED\n");
}

/**
 * @brief Stress test with ordered insertions to force rotations.
 */
void test_rb_stress_rotations() {
    printf("Running test_rb_stress_rotations... ");
    RB_Tree *tree = rb_tree_create();

    const char *words[] = {"a", "b", "c", "d", "e", "f", "g"};
    for (int i = 0; i < 7; i++) {
        rb_tree_insert(tree, words[i], i);
    }

    /* Verify structural integrity after rotations */
    assert(tree->root->color == BLACK);
    verify_black_height(tree, tree->root);

    printf("PASSED\n");
}

int main() {
    printf("\n========================================\n");
    printf("     RED-BLACK TREE TEST SUITE\n");
    printf("========================================\n");

    test_rb_insertion_and_search();
    test_rb_structural_invariants();
    test_duplicate_key_policy();
    test_rb_stress_rotations();

    printf("\nALL TESTS PASSED SUCCESSFULLY!\n");
    printf("========================================\n");

    return 0;
}