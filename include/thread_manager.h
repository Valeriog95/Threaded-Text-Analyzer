/**
 * @file thread_manager.h
 * @brief Orchestrates the multi-threaded pipeline (Reader -> Analyzer -> Exporter).
 * @details Implements a producer-consumer pattern using synchronized queues.
 * Using also condition variable when empty or ful to improve perfomance 
 */

#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>
#include <stdbool.h>
#include "word.h"
#include "map.h"

/**
 * @brief Maximum number of words the shared queue can hold.
 * This prevents the Reader from consuming too much memory if the Analyzer is slow.
 */
#define QUEUE_MAX_CAPACITY 1024

/**
 * @brief Shared buffer between Reader and Analyzer.
 */
typedef struct {
    word_t words[QUEUE_MAX_CAPACITY];
    int head;
    int tail;
    int count;
    bool finished;             // Set by Reader when EOF is reached
    pthread_mutex_t mutex;
    pthread_cond_t cond_not_full;
    pthread_cond_t cond_not_empty;
} word_queue_t;

/**
 * @brief Configuration passed to the pipeline threads.
 */
typedef struct {
    const char* input_path;    // Path to read from
    const char* export_path;   // Path to write CSV (if needed)
    RB_Tree* main_tree;        // Shared RB-Tree to update
    bool do_export;            // Whether to trigger the Exporter thread
    word_queue_t* queue;       // The shared communication buffer
} pipeline_config_t;

/**
 * @brief Main entry point to start the 3-thread pipeline.
 * @param args Configuration for the pipeline.
 */
void run_multi_threaded_pipeline(
    const char* input,
    const char* output,
    RB_Tree* tree,
    bool do_export);


/**
 * @brief Thread A: Reads tokens from file and pushes them to the queue.
 */
void* thread_reader(void* arg);

/**
 * @brief Thread B: Pops tokens from the queue and updates the RB-Tree.
 */
void* thread_analyzer(void* arg);

/**
 * @brief Thread C: Waits for analysis to finish and exports the CSV.
 */
void* thread_exporter(void* arg);

#endif