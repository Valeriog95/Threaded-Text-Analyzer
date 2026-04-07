/**
 * @file argtable_parser.h
 * @brief Header for CLI argument parsing encapsulation.
 * @details Using an internal function to parse argument and populate the structure config_t
 *  result. The Function can return 0 (success), 1 (error), -1 (help requested)
 */

#ifndef ARGTABLE_PARSER_H
#define ARGTABLE_PARSER_H

#include <stdbool.h>

/**
 * @brief Clean structure containing all parsed options.
 */
typedef struct {
    const char* input_file;
    const char* export_file;
    const char* t2_start_word;
    int t2_count;
    bool is_multi;
    bool do_task1;
    bool do_export;
    bool do_task2;
    bool help_requested;
} config_t;

/**
 * @brief Parses command line arguments and fills the config structure.
 * @return 0 on success, 1 on error, -1 if help was displayed.
 */
int parse_arguments(int argc, char* argv[], config_t* config);

#endif