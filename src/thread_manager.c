#include "thread_manager.h"
#include "parser.h"
#include "analyzer.h"
#include "word.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Initialize the synchronized queue.
 */
static void queue_init(word_queue_t* q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    q->finished = false;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond_not_full, NULL);
    pthread_cond_init(&q->cond_not_empty, NULL);
}

/**
 * @brief Push a word_t into the queue (blocks if full).
 */
static void queue_push(word_queue_t* q, word_t word) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == QUEUE_MAX_CAPACITY) {
        pthread_cond_wait(&q->cond_not_full, &q->mutex);
    }

    // deep copy of the struct can be optimized by passing a pointer
    q->words[q->tail] = word; 
    q->tail = (q->tail + 1) % QUEUE_MAX_CAPACITY;
    q->count++;

    pthread_cond_signal(&q->cond_not_empty);
    pthread_mutex_unlock(&q->mutex);
}

/**
 * @brief Pop a word_t from the queue (blocks if empty).
 * @return true if a word was retrieved, false if queue is finished and empty.
 */
static bool queue_pop(word_queue_t* q, word_t* out_word) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0 && !q->finished) {
        pthread_cond_wait(&q->cond_not_empty, &q->mutex);
    }
    if (q->count == 0 && q->finished) {
        pthread_mutex_unlock(&q->mutex);
        return false;
    }
    *out_word = q->words[q->head];
    q->head = (q->head + 1) % QUEUE_MAX_CAPACITY;
    q->count--;
    pthread_cond_signal(&q->cond_not_full);
    pthread_mutex_unlock(&q->mutex);
    return true;
}

// --- THREAD LOGIC ---

void* thread_reader(void* arg) {
    pipeline_config_t* config = (pipeline_config_t*)arg;
    FILE* f = fopen(config->input_path, "r");
    if (!f) return NULL;

    word_t w;
    // parsing will fill the struct word_t (MAX_WORD_SIZE)
    while (parser_get_next_word(f, &w)) {
        queue_push(config->queue, w);
    }

    pthread_mutex_lock(&config->queue->mutex);
    config->queue->finished = true;
    pthread_cond_broadcast(&config->queue->cond_not_empty);
    pthread_mutex_unlock(&config->queue->mutex);

    fclose(f);
    return NULL;
}

void* thread_analyzer(void* arg) {
    pipeline_config_t* config = (pipeline_config_t*)arg;
    word_t current_word, last_word;
    word_t first_word;
    bool is_first = true;
    bool has_last = false;

    while (queue_pop(config->queue, &current_word)) {
        
        // store first word for the cirucal buffer link
        if (is_first) {
            first_word = current_word;
            is_first = false;
        }

        if (has_last) {

            // (Task 1)
            // 1. Find or insert first word in the tree
            RB_Node* main_node = rb_tree_get_or_insert(config->main_tree, last_word.value, NULL);
            
            // 2. Ensure there is the SuccessorContext
            action_ensure_successor_context(main_node, (main_node->value == NULL));
            
            SuccessorContext* ctx = (SuccessorContext*)main_node->value;
            
            // 3. Insert or increment successor in the subtree
            rb_tree_get_or_insert_execute(ctx->tree, current_word.value, action_increment_count);
            
            // 4. Update the tree in O(1)
            ctx->total_entries++;
        }
        
        last_word = current_word;
        has_last = true;
    }

    // Last circular link: last word -> first word
    if (has_last && !is_first) {

        RB_Node* main_node = rb_tree_get_or_insert(config->main_tree, last_word.value, NULL);
        
        action_ensure_successor_context(main_node, (main_node->value == NULL));
        
        SuccessorContext* ctx = (SuccessorContext*)main_node->value;
        
        rb_tree_get_or_insert_execute(ctx->tree, first_word.value, action_increment_count);
        
        ctx->total_entries++;
    }

    return NULL;
}

void* thread_exporter(void* arg) {
    pipeline_config_t* config = (pipeline_config_t*)arg;
    if (!config->do_export || !config->export_path) return NULL;

    FILE* out = fopen(config->export_path, "w");
    if (out) {
        parser_export_csv(out, config->main_tree);
        fclose(out);
    }
    return NULL;
}

void run_multi_threaded_pipeline(
    const char* input,
    const char* output,
    RB_Tree* tree,
    bool do_export) {
   
    word_queue_t queue;
    queue_init(&queue);

    pipeline_config_t config = {
        .input_path = input,
        .export_path = output,
        .main_tree = tree,
        .do_export = do_export,
        .queue = &queue
    };

    pthread_t reader_tid, analyzer_tid, exporter_tid;

    pthread_create(&reader_tid, NULL, thread_reader, &config);
    pthread_create(&analyzer_tid, NULL, thread_analyzer, &config);

    // Wait for the first 2 stages
    pthread_join(reader_tid, NULL);
    pthread_join(analyzer_tid, NULL);

    // Exporting
    if (do_export) {
        pthread_create(&exporter_tid, NULL, thread_exporter, &config);
        pthread_join(exporter_tid, NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.cond_not_full);
    pthread_cond_destroy(&queue.cond_not_empty);
}