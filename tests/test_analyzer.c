#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "analyzer.h"
#include "map.h"

const char test_string[] = "Cosa dicono le previsioni del tempo? Previsioni del tempo di oggi: tempo incerto! Previsioni di domani?";

/**
 * @brief Prints the nested structure using iterators.
 * Format: [word] -> (successor: count) (successor: count)
 */
void print_frequency_table(RB_Tree* main_tree) {
    printf("\n--- Text Phrase: ---\n");
    printf("%s\n", test_string);
    printf("\n--- FREQUENCY TABLE START ---\n");
    
    for (RB_Tree_Iterator it_main = rb_tree_it_begin(main_tree); 
         it_main != rb_tree_it_end(main_tree); 
         it_main = rb_tree_it_increment(main_tree, it_main)) 
    {
        printf("[%s] -> ", it_main->key.value);
        
        RB_Tree* successor_tree = (RB_Tree*)it_main->value;
        
        for (RB_Tree_Iterator it_sub = rb_tree_it_begin(successor_tree); 
             it_sub != rb_tree_it_end(successor_tree); 
             it_sub = rb_tree_it_increment(successor_tree, it_sub)) 
        {
            int count = *(int*)it_sub->value;
            printf("(%s: %d) ", it_sub->key.value, count);
        }
        printf("\n");
    }
    printf("--- FREQUENCY TABLE END ---\n\n");
}

void test_complex_sentence_analysis() {
    printf("Running Complex Sentence Analysis Test...\n");

    // 1. Create temporary file
    FILE* tmp = fopen("test_meteo.txt", "w");
    if (!tmp) {
        perror("Failed to create test file");
        return;
    }
    fprintf(tmp, "%s", test_string);
    fclose(tmp);

    // 2. Setup Trees
    RB_Tree* main_tree = rb_tree_create();
    main_tree->free_value = (void (*)(void*))rb_tree_delete; // Cascade delete

    // 3. Run Analysis
    FILE* input = fopen("test_meteo.txt", "r");
    if (!input) {
        perror("Failed to open test file");
        return;
    }
    analyzer_process_text(input, main_tree);
    fclose(input);

    // Show the output (should be automatic tested as well...)
    printf("\n--- GENERATED CSV OUTPUT ---\n");
    parser_export_csv(stdout, main_tree);

    // 4. Visual Debug
    print_frequency_table(main_tree);

    // 5. Logical Assertions
    
    // Check "previsioni" -> "del" (appears 2 times) and "previsioni" -> "di" (1 time)
    RB_Node* node_prev = rb_tree_at(main_tree, "previsioni");
    assert(node_prev != main_tree->nil);
    RB_Tree* tree_prev = (RB_Tree*)node_prev->value;
    
    RB_Node* node_del = rb_tree_at(tree_prev, "del");
    assert(node_del != tree_prev->nil);
    assert(*(int*)node_del->value == 2);

    // Check "tempo" -> "?" (1 time) and "tempo" -> "di" (1 time)
    RB_Node* node_tempo = rb_tree_at(main_tree, "tempo");
    assert(node_tempo != main_tree->nil);
    RB_Tree* tree_tempo = (RB_Tree*)node_tempo->value;
    
    RB_Node* node_tempo_q = rb_tree_at(tree_tempo, "?");
    assert(node_tempo_q != tree_tempo->nil);
    assert(*(int*)node_tempo_q->value == 1);

    // CRITICAL REQUIREMENT: Last token is '?', it must point back to 'cosa'
    RB_Node* node_last_q = rb_tree_at(main_tree, "?");
    assert(node_last_q != main_tree->nil);
    RB_Tree* tree_last_q = (RB_Tree*)node_last_q->value;
    
    RB_Node* node_back_to_start = rb_tree_at(tree_last_q, "cosa");
    assert(node_back_to_start != tree_last_q->nil);
    assert(*(int*)node_back_to_start->value == 1);

    // 6. Cleanup
    rb_tree_delete(main_tree);
    remove("test_meteo.txt");
    printf("Test Complex Sentence: PASSED\n");
}

int main() {
    test_complex_sentence_analysis();
    return 0;
}