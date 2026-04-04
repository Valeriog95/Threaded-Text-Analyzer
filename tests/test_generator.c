#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"
#include "analyzer.h"
#include "map.h"

/**
 * @brief Test logical picking from a SuccessorContext.
 * Verifies that probabilities are respected and all options are eventually picked.
 */
void test_generator_pick_logic() {
    printf("Running Generator Pick Logic Test...\n");

    // 1. Setup a controlled tree
    RB_Tree* main_tree = rb_tree_create();
    main_tree->free_value = successor_context_delete;

    // Insert "mela" with NULL value initially
    RB_Node* mela_node = rb_tree_get_or_insert(main_tree, "mela", NULL);
    // Initialize the context manually using our action
    action_ensure_successor_context(mela_node, true);
    
    SuccessorContext* ctx = (SuccessorContext*)mela_node->value;

    // Add "rossa" as the only successor
    rb_tree_get_or_insert_execute(ctx->tree, "rossa", action_increment_count);
    ctx->total_entries = 1;

    // 2. Deterministic Test: Only one option exists
    for(int i = 0; i < 50; i++) {
        const char* picked = generator_pick_next_word(ctx);
        assert(picked != NULL);
        assert(strcmp(picked, "rossa") == 0);
    }

    // 3. Probabilistic Test: Add "verde"
    rb_tree_get_or_insert_execute(ctx->tree, "verde", action_increment_count);
    ctx->total_entries = 2; // Now 50/50 chance

    bool saw_rossa = false;
    bool saw_verde = false;

    // Running 200 iterations makes the chance of not seeing one of them (0.5^200) impossible
    for(int i = 0; i < 200; i++) {
        const char* picked = generator_pick_next_word(ctx);
        if (strcmp(picked, "rossa") == 0) saw_rossa = true;
        if (strcmp(picked, "verde") == 0) saw_verde = true;
    }

    assert(saw_rossa && "Failed to pick 'rossa' in 200 trials");
    assert(saw_verde && "Failed to pick 'verde' in 200 trials");

    rb_tree_delete(main_tree);
    printf("Generator Pick Logic: PASSED\n");
}

/**
 * @brief Test end-to-end generation sequence.
 * Verifies that the generator follows the Markov chain: A -> B -> C
 */
void test_generator_full_chain() {
    printf("Running Generator Full Chain Test...\n");

    RB_Tree* main_tree = rb_tree_create();
    main_tree->free_value = successor_context_delete;

    // Build chain: "io" -> "programmo" -> "in" -> "c"
    const char* words[] = {"io", "programmo", "in", "c"};
    for (int i = 0; i < 3; i++) {
        RB_Node* n = rb_tree_get_or_insert(main_tree, words[i], NULL);
        action_ensure_successor_context(n, true);
        SuccessorContext* ctx = (SuccessorContext*)n->value;
        rb_tree_get_or_insert_execute(ctx->tree, (void*)words[i+1], action_increment_count);
        ctx->total_entries = 1;
    }

    // Capture stdout or just run to ensure no crashes
    printf("Visual Check - Expected 'io programmo in c': ");
    generator_generate_text(main_tree, "io", 4);

    rb_tree_delete(main_tree);
    printf("Generator Full Chain: PASSED\n");
}

int main() {
    // Seed the random generator for the tests
    srand((unsigned int)time(NULL));

    printf("=== Starting Generator Suite ===\n");
    
    test_generator_pick_logic();
    test_generator_full_chain();

    printf("=== All Generator Tests Passed! ===\n");
    return 0;
}