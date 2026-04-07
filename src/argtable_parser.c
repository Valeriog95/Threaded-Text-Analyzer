#include "argtable_parser.h"
#include "argtable3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Parses command line arguments and populates the configuration structure.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param config Pointer to the config_t structure.
 * @return 0 on success, 1 on error, -1 if help was displayed.
 */
int parse_arguments(int argc, char* argv[], config_t* config) {
    
    /* 1. Argument Definitions */
    struct arg_file *input_file = arg_file1(NULL, NULL, "<file>",          "Input file (.txt or .csv)");
    struct arg_lit  *single     = arg_lit0(NULL, "single",                 "Force single-thread mode");
    struct arg_lit  *multi      = arg_lit0(NULL, "multi",                  "Force multi-thread mode");
    
    /* Task 2 Flags: -g (generate) is mandatory for Task 2, -s (start) is optional */
    struct arg_int  *t2_count   = arg_int0("g", "task2", "<n>",            "Enable Task 2: generate <n> words (mandatory count)");
    struct arg_str  *t2_word    = arg_str0("s", "start", "<word>",         "Optional starting word for generation");
    
    struct arg_file *export_f   = arg_file0("e", "export", "<file>",       "Export to CSV (Task 1) or Text (Task 2)");
    struct arg_lit  *help       = arg_lit0("h", "help",                    "Print this help message");
    struct arg_end  *end        = arg_end(20);

    void* argtable[] = {input_file, single, multi, t2_count, t2_word, export_f, help, end};

    /* Check for allocation errors */
    if (arg_nullcheck(argtable) != 0) {
        fprintf(stderr, "error: insufficient memory\n");
        return 1;
    }

    /* Parse arguments */
    int nerrors = arg_parse(argc, argv, argtable);

    /* Handle Help */
    if (help->count > 0) {
        printf("Usage: ThreadedTextAnalyzer");
        arg_print_syntax(stdout, argtable, "\n\nOptions:\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]));
        return -1;
    }

    /* Handle Parsing Errors */
    if (nerrors > 0) {
        arg_print_errors(stdout, end, "Analyzer");
        printf("\nTry '--help' for more information.\n");
        arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]));
        return 1;
    }

    /* 2. Mapping to config_t */
    config->input_file = input_file->filename[0];
    config->is_multi = (multi->count > 0);
    
    /* Export logic */
    config->do_export = (export_f->count > 0);
    config->export_file = config->do_export ? export_f->filename[0] : NULL;

    /* Task 2 Logic: Activated if -g / --task2 is provided with a count */
    config->do_task2 = (t2_count->count > 0);
    
    if (config->do_task2) {
        /* Word count is mandatory when Task 2 is enabled */
        config->t2_count = t2_count->ival[0];
        
        /* Starting word is optional. If -s is missing, t2_start_word becomes NULL, 
           triggering the random punctuation fallback in the generator. */
        config->t2_start_word = (t2_word->count > 0) ? t2_word->sval[0] : NULL;
    } else {
        config->t2_count = 0;
        config->t2_start_word = NULL;
    }

    /* Task 1 Logic: Default behavior if input is .txt */
    config->do_task1 = (strstr(config->input_file, ".txt") != NULL);

    /* Cleanup */
    arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]));
    return 0;
}