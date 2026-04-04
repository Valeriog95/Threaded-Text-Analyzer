#include "parser.h"
#include "analyzer.h"
#include <ctype.h>
#include <string.h>

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