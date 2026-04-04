#include "parser.h"
#include <ctype.h>
#include <string.h>
/**
 * @brief Helper to sum all integer values in a successor tree.
 */
static int sum_total_occurrences(RB_Tree *tree);

bool parser_get_next_word(FILE *file, word_t *out_word) {
    if (!file || !out_word) return false;

    int c;
    char temp_buffer[MAX_WORD_SIZE];
    int pos = 0;

    while ((c = fgetc(file)) != EOF) {
        
        // Special characters (., ?, !)
        if (c == '.' || c == '?' || c == '!') {
            temp_buffer[0] = (char)c;
            temp_buffer[1] = '\0';
            *out_word = word_create(temp_buffer);
            return true;
        }

        // word starting or a: '
        if (isalnum(c) || c == '\'') {
            temp_buffer[pos++] = (char)tolower(c);

            while ((c = fgetc(file)) != EOF) {

                if (isalnum(c) || c == '\'') {
                    if (pos < MAX_WORD_SIZE - 1) 
                        temp_buffer[pos++] = (char)tolower(c);                    
                } else {
                    // replace unvalid char into the buffer
                    ungetc(c, file); 
                    break;
                }
            }
            temp_buffer[pos] = '\0';
            *out_word = word_create(temp_buffer);
            return true;
        }
        // Ignore the rest ...
    }

    return false;
}


void parser_export_csv(FILE *out, RB_Tree *main_tree) {
    if (!out || !main_tree) return;
    
    // Outer loop: iterate over each unique word in the text
    for (RB_Tree_Iterator it_main = rb_tree_it_begin(main_tree); 
    it_main != rb_tree_it_end(main_tree); 
    it_main = rb_tree_it_increment(main_tree, it_main)) 
    {
        fprintf(out, "%s", it_main->key.value);
        
        RB_Tree *successor_tree = (RB_Tree*)it_main->value;
        int total_for_this_word = sum_total_occurrences(successor_tree);
        
        // Inner loop: iterate over successors of the current word
        for (RB_Tree_Iterator it_sub = rb_tree_it_begin(successor_tree); 
        it_sub != rb_tree_it_end(successor_tree); 
        it_sub = rb_tree_it_increment(successor_tree, it_sub)) 
        {
            int count = *(int*)it_sub->value;
            double frequency = (double)count / total_for_this_word;
            
            // Format: ,successor,frequency
            fprintf(out, ",%s,%.4f", it_sub->key.value, frequency);
        }
        fprintf(out, "\n");
    }
}

static int sum_total_occurrences(RB_Tree *tree) {
    int total = 0;
    for (RB_Tree_Iterator it = rb_tree_it_begin(tree); 
            it != rb_tree_it_end(tree); 
            it = rb_tree_it_increment(tree, it)) 
    {
        total += *(int*)it->value;
    }
    return total;
}