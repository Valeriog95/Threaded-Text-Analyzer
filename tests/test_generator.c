#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"
#include "analyzer.h"
#include "map.h"
#include "word.h"

/**
 * @brief Test logical picking from a SuccessorContext.
 */
void test_generator_pick_logic() {
    printf("Running Generator Pick Logic Test...\n");

    RB_Tree* main_tree = rb_tree_create();
    main_tree->free_value = successor_context_delete;

    // 1. Setup: Manual insertion of words
    // Note: ensure rb_tree_get_or_insert handles the key correctly (copying into word_t)
    RB_Node* mela_node = rb_tree_get_or_insert(main_tree, "mela", NULL);
    action_ensure_successor_context(mela_node, true);
    
    SuccessorContext* ctx = (SuccessorContext*)mela_node->value;

    // Add "rossa" as successor
    rb_tree_get_or_insert_execute(ctx->tree, "rossa", action_increment_count);
    ctx->total_entries = 1;

    // 2. Deterministic Test
    for(int i = 0; i < 50; i++) {
        const word_t* picked = generator_pick_next_word(ctx);
        assert(picked != NULL);
        assert(strcmp(picked->value, "rossa") == 0);
    }

    // 3. Probabilistic Test: Add "verde"
    rb_tree_get_or_insert_execute(ctx->tree, "verde", action_increment_count);
    ctx->total_entries = 2; 

    bool saw_rossa = false;
    bool saw_verde = false;

    for(int i = 0; i < 200; i++) {
        const word_t* picked = generator_pick_next_word(ctx);
        if (strcmp(picked->value, "rossa") == 0) saw_rossa = true;
        if (strcmp(picked->value, "verde") == 0) saw_verde = true;
    }

    assert(saw_rossa && "Failed to pick 'rossa' in 200 trials");
    assert(saw_verde && "Failed to pick 'verde' in 200 trials");

    rb_tree_delete(main_tree);
    printf("Generator Pick Logic: PASSED\n");
}

/**
 * @brief Test end-to-end generation sequence.
 */
void test_generator_full_chain() {
    printf("Running Generator Full Chain Test...\n");

    RB_Tree* main_tree = rb_tree_create();
    main_tree->free_value = successor_context_delete;

    const char* words[] = {"io", "programmo", "in", "c"};
    for (int i = 0; i < 3; i++) {
        RB_Node* n = rb_tree_get_or_insert(main_tree, words[i], NULL);
        action_ensure_successor_context(n, true);
        SuccessorContext* ctx = (SuccessorContext*)n->value;
        rb_tree_get_or_insert_execute(ctx->tree, (void*)words[i+1], action_increment_count);
        ctx->total_entries = 1;
    }

    // Capture visual check
    printf("Visual Check - Expected 'Io programmo in c':\n");
    // Added 'stdout' parameter to match new signature
    generator_generate_text(stdout, main_tree, "io", 4);

    rb_tree_delete(main_tree);
    printf("Generator Full Chain: PASSED\n");
}

int main() {
    srand((unsigned int)time(NULL));

    printf("=== Starting Generator Suite ===\n");
    
    test_generator_pick_logic();
    test_generator_full_chain();

    printf("=== All Generator Tests Passed! ===\n");
    return 0;
}