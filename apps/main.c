/**
 * @file main.c
 * @brief Entry point for the Threaded Text Analyzer.
 * @details Orchestrates Task 1 (Analysis) and Task 2 (Generation) in single or multi-thread mode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "argtable_parser.h"
#include "analyzer.h"
#include "generator.h"
#include "parser.h"
#include "thread_manager.h"

int main(int argc, char *argv[]) {
    
    // 1. Initialize random seed (required for Task 2)
    srand((unsigned int)time(NULL));

    // 2. Parse command line arguments
    config_t config;
    int parse_res = parse_arguments(argc, argv, &config);

    if (parse_res != 0) {
        return (parse_res == -1) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    // 3. Setup the main data structure (Red-Black Tree)
    RB_Tree* main_tree = rb_tree_create();
    main_tree->free_value = successor_context_delete;

    // 4. TASK 1: Analysis / Loading
    /* Check if the input file is a CSV (for Task 2 directly) or a text file */
    const char* ext = strrchr(config.input_file, '.');
    bool is_csv = (ext && strcmp(ext, ".csv") == 0);

    if (is_csv) {
        printf("[INFO] Loading frequency table from CSV: %s\n", config.input_file);
        FILE* csv_f = fopen(config.input_file, "r");
        if (!csv_f) {
            perror("Error opening CSV file");
            rb_tree_delete(main_tree);
            return EXIT_FAILURE;
        }
        parser_import_csv(csv_f, main_tree);
        fclose(csv_f);
    } 
    else if (config.is_multi) {
        printf("[INFO] Running Task 1 in Multi-thread mode...\n");
        /* Note: the pipeline handles analysis and optional export internally */
        run_multi_threaded_pipeline(config.input_file, config.export_file, main_tree, config.do_export);
    } 
    else {
        printf("[INFO] Running Task 1 in Single-thread mode...\n");
        FILE* f = fopen(config.input_file, "r");
        if (!f) {
            perror("Error opening input file");
            rb_tree_delete(main_tree);
            return EXIT_FAILURE;
        }
        analyzer_process_text(f, main_tree);
        fclose(f);

        // Manual export if requested in single-thread mode
        if (config.do_export) {
            printf("[INFO] Exporting to CSV: %s\n", config.export_file);
            FILE* out = fopen(config.export_file, "w");
            if (out) {
                parser_export_csv(out, main_tree);
                fclose(out);
            } else {
                perror("Error opening export file");
            }
        }
    }

    // 5. TASK 2: Text Generation
    if (config.do_task2) {
        
        // Decide output stream: if -e is provided, we might want to write there 
        // or just stick to stdout as per common CLI behavior. 
        // Let's use stdout for the user to see, and optionally a file.
        FILE* out_stream = stdout;
        
        if (config.do_export && config.export_file) {
            out_stream = fopen(config.export_file, "a"); // "a" for append, or "w"
            if (!out_stream) out_stream = stdout; 
        }

        printf("[INFO] Task 2: Generating %d words...\n\n", config.t2_count);
        
        generator_generate_text(out_stream, main_tree, config.t2_start_word, config.t2_count);
        
        if (out_stream != stdout) fclose(out_stream);
        printf("\n");
    }

    // 6. Cleanup
    printf("[INFO] Cleaning up memory...\n");
    rb_tree_delete(main_tree);

    return EXIT_SUCCESS;
}