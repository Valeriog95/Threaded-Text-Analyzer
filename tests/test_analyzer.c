#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "analyzer.h"
#include "map.h"

const char test_string[] = "Cosa dicono le previsioni del tempo? Previsioni del tempo di oggi: tempo incerto! Previsioni di domani?";

/**
 * @brief Test expected csv
 */
const char expected_csv[] = 
    "!,previsioni,1.0000\n"
    "?,cosa,0.5000,previsioni,0.5000\n"
    "cosa,dicono,1.0000\n"
    "del,tempo,1.0000\n"
    "di,domani,0.5000,oggi,0.5000\n"
    "dicono,le,1.0000\n"
    "domani,?,1.0000\n"
    "incerto,!,1.0000\n"
    "le,previsioni,1.0000\n"
    "oggi,tempo,1.0000\n"
    "previsioni,del,0.6667,di,0.3333\n"
    "tempo,?,0.3333,di,0.3333,incerto,0.3333\n";

    /**
 * @brief Test that generated csv is the same as the required one.
 */
void test_parser_integration(RB_Tree* main_tree) {
    printf("Running CSV Integration Test...\n");

    char buffer[4096] = {0}; 
    FILE* mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    
    if (!mem_stream) {
        perror("fmemopen failed");
        return;
    }

    parser_export_csv(mem_stream, main_tree);
    fflush(mem_stream);
    fclose(mem_stream);

    if (strcmp(buffer, expected_csv) == 0) {
        printf("CSV Integration Test: PASSED\n");
    } else {
        printf("CSV Integration Test: FAILED!\n");
        printf("--- Expected ---\n%s", expected_csv);
        printf("--- Got ---\n%s", buffer);
        assert(0 && "CSV output mismatch");
    }
}

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

    // Show the output (and its automatic test)
    printf("\n--- GENERATED CSV OUTPUT ---\n");
    parser_export_csv(stdout, main_tree);
    test_parser_integration(main_tree);

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