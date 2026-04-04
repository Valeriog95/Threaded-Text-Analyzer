#include "generator.h"
#include "analyzer.h"
#include <stdlib.h>
#include <time.h>

const char* generator_pick_next_word(SuccessorContext* ctx) {
    if (!ctx || ctx->total_entries == 0) return NULL;

    int r = rand() % ctx->total_entries;
    int cumulative_count = 0;

    // 2. Iterate through successors and find where 'r' falls
    for (RB_Tree_Iterator it = rb_tree_it_begin(ctx->tree); 
         it != rb_tree_it_end(ctx->tree); 
         it = rb_tree_it_increment(ctx->tree, it)) 
    {
        int count = *(int*)it->value;
        cumulative_count += count;

        if (r < cumulative_count) {
            return it->key.value; // Found the winner!
        }
    }

    return NULL;
}

void generator_generate_text(RB_Tree* main_tree, const char* start_word, int num_words) {
    if (!main_tree || !start_word) return;

    // TODO: initialize random seed in main 
    srand(time(NULL));

    const char* current = start_word;
    printf("Generated Text:\n%s", current);

    for (int i = 0; i < num_words - 1; i++) {
        RB_Node* node = rb_tree_at(main_tree, (void*)current);
        
        if (node == main_tree->nil) break; // Word not found

        SuccessorContext* ctx = (SuccessorContext*)node->value;
        const char* next = generator_pick_next_word(ctx);

        if (!next) break;

        // Print with a space, unless it's punctuation
        if (next[0] == '.' || next[0] == '?' || next[0] == '!') {
            printf("%s", next);
        } else {
            printf(" %s", next);
        }

        current = next;
    }
    printf("\n");
}