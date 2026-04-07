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
 * @brief Helper to manually populate the RB-Tree with word pairs and counts.
 */
void add_pair(RB_Tree* tree, const char* word, const char* successor, int count) {
    RB_Node* n = rb_tree_get_or_insert(tree, (void*)word, NULL);
    action_ensure_successor_context(n, (n->value == NULL));
    
    SuccessorContext* ctx = (SuccessorContext*)n->value;
    // We force the count by calling increment multiple times or modifying it
    for(int i = 0; i < count; i++) {
        rb_tree_get_or_insert_execute(ctx->tree, (void*)successor, action_increment_count);
    }
    ctx->total_entries += count;
}

/**
 * @brief Test using the "Weather Forecast" example from the requirements.
 */
void test_generator_weather_example() {
    printf("\n--- Running Weather Forecast Example Test ---\n");

    RB_Tree* main_tree = rb_tree_create();
    main_tree->free_value = successor_context_delete;

    // Populating based on the CSV example provided
    add_pair(main_tree, "?", "cosa", 1);
    add_pair(main_tree, "?", "previsioni", 1);
    add_pair(main_tree, "!", "previsioni", 1);
    add_pair(main_tree, "cosa", "dicono", 1);
    add_pair(main_tree, "dicono", "le", 1);
    add_pair(main_tree, "le", "previsioni", 1);
    add_pair(main_tree, "previsioni", "di", 1);
    add_pair(main_tree, "previsioni", "del", 2);
    add_pair(main_tree, "del", "tempo", 1);
    add_pair(main_tree, "tempo", "incerto", 1);
    add_pair(main_tree, "tempo", "di", 1);
    add_pair(main_tree, "tempo", "?", 1);
    add_pair(main_tree, "di", "domani", 1);
    add_pair(main_tree, "di", "oggi", 1);
    add_pair(main_tree, "oggi", "tempo", 1);
    add_pair(main_tree, "incerto", "!", 1);
    add_pair(main_tree, "domani", "?", 1);

    printf("Expected: 4 random sequences of 11 words starting with '!'\n");
    for(int i = 1; i <= 4; i++) {
        printf("Run %d: ", i);
        generator_generate_text(stdout, main_tree, "!", 11);
    }

    rb_tree_delete(main_tree);
    printf("Weather Example Test: COMPLETED (Check visual output)\n");
}

/**
 * @brief Test Requirement: picking a random punctuation if no start word is given.
 */
void test_generator_random_start() {
    printf("\n--- Running Random Punctuation Start Test ---\n");

    RB_Tree* main_tree = rb_tree_create();
    main_tree->free_value = successor_context_delete;

    // Only punctuation in the tree
    add_pair(main_tree, ".", "word1", 1);
    add_pair(main_tree, "?", "word2", 1);
    add_pair(main_tree, "!", "word3", 1);

    printf("Generating with NULL start word (should pick ., ? or ! randomly):\n");
    for(int i = 0; i < 3; i++) {
        printf("Random start %d: ", i+1);
        generator_generate_text(stdout, main_tree, NULL, 5);
    }

    rb_tree_delete(main_tree);
}

int main() {
    // Fixed seed for debugging, or time(NULL) for real randomness
    srand((unsigned int)time(NULL));

    printf("=== Starting Advanced Generator Suite ===\n");
    
    test_generator_weather_example();
    test_generator_random_start();

    printf("\n=== All Generator Tests Completed! ===\n");
    return 0;
}