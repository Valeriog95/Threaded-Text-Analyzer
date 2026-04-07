#include "parser.h"
#include "analyzer.h"
#include "map_utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LEN 4096

bool parser_get_next_word(FILE *file, word_t *out_word) {
    if (!file || !out_word) return false;

    int c;
    char temp_buffer[MAX_WORD_SIZE];
    int pos;

    while ((c = fgetc(file)) != EOF) {
        pos = 0;

        // Special characters (., ?, !)
        if (c == '.' || c == '?' || c == '!') {
            temp_buffer[0] = (char)c;
            temp_buffer[1] = '\0';
            *out_word = word_create(temp_buffer);
            return true;
        }

        // word starting with alphanumeric or '
        if (isalnum(c) || c == '\'') {
            temp_buffer[pos++] = (char)tolower(c);

            while ((c = fgetc(file)) != EOF) {
                if (isalnum(c) || c == '\'') {
                    if (pos < MAX_WORD_SIZE - 1) 
                        temp_buffer[pos++] = (char)tolower(c);                    
                } else {
                    ungetc(c, file); 
                    break;
                }
            }
            temp_buffer[pos] = '\0';
            *out_word = word_create(temp_buffer);
            return true;
        }
    }
    return false;
}

void parser_export_csv(FILE *out, RB_Tree *main_tree) {
    if (!out || !main_tree) return;
    
    for (RB_Tree_Iterator it_main = rb_tree_it_begin(main_tree); 
         it_main != rb_tree_it_end(main_tree); 
         it_main = rb_tree_it_increment(main_tree, it_main)) 
    {
        SuccessorContext *ctx = (SuccessorContext*)it_main->value;
        RB_Tree *successor_tree = ctx->tree;
        size_t total_for_this_word = ctx->total_entries; 
        
        fprintf(out, "%s", it_main->key.value);
        
        for (RB_Tree_Iterator it_sub = rb_tree_it_begin(successor_tree); 
             it_sub != rb_tree_it_end(successor_tree); 
             it_sub = rb_tree_it_increment(successor_tree, it_sub)) 
        {
            int count = *(int*)it_sub->value;
            
            // now is O(1)
            double frequency = (double)count / (double)total_for_this_word;
            
            fprintf(out, ",%s,%.4f", it_sub->key.value, frequency);
        }
        fprintf(out, "\n");
    }
}

int parser_import_csv(FILE* input, RB_Tree* tree) {
    char line[MAX_LINE_LEN];
    
    while (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\r\n")] = 0; // Remove newline

        char *token = strtok(line, ",");
        if (!token) continue;

        // 1. Get or create the main word node in the main tree
        RB_Node* node = rb_tree_get_or_insert(tree, token, NULL);
        
        // 2. Ensure it has a SuccessorContext (Task 1 logic reuse)
        action_ensure_successor_context(node, (node->value == NULL));
        SuccessorContext* ctx = (SuccessorContext*)node->value;

        double sum_frequencies = 0.0;

        // 3. Process pairs: successor_word, frequency
        while ((token = strtok(NULL, ",")) != NULL) {
            char *successor_word = token;
            token = strtok(NULL, ",");
            if (!token) break;

            double freq = atof(token);
            sum_frequencies += freq;

            /* Convert frequency to integer weight (scale 1000) */
            int weight = (int)(freq * 1000.0);
            
            /* * Use map_utils API to store the integer weight.
             * This function handles malloc(sizeof(int)) internally.
             */
            rb_tree_int_get_or_insert(ctx->tree, successor_word, weight);
            
            // Update total for the Markov selection logic
            ctx->total_entries += weight;
        }

        /* Integrity check: sum should be approx 1.0 */
        if (fabs(sum_frequencies - 1.0) > 0.001) {
            fprintf(stderr, "[WARNING] Integrity failed for '%s': sum is %.4f\n", 
                    node->key.value, sum_frequencies);
        }
    }
    return 0;
}