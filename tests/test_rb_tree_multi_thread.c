#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "map_utils.h"

#define NUM_THREADS 10
#define INSERTS_PER_THREAD 1000

// Thread data structure
typedef struct {
    RB_Tree *tree;
    int thread_id;
} thread_data_t;

// Test thread function
void* stress_test_worker(void* arg) {
    thread_data_t *data = (thread_data_t*)arg;
    char key[20];

    for (int i = 0; i < INSERTS_PER_THREAD; i++) {
        
        // Data creation based on therad ID
        sprintf(key, "t%d_key%d", data->thread_id, i);
        rb_tree_int_get_or_insert(data->tree, key, i);
    }
    return NULL;
}

void test_multithreaded_insertion() {
    printf("Running multithreaded stress test (% d threads)... ", NUM_THREADS);
    
    RB_Tree *tree = rb_tree_int_create();
    pthread_t threads[NUM_THREADS];
    thread_data_t tdata[NUM_THREADS];

    // Thread creation
    for (int i = 0; i < NUM_THREADS; i++) {
        tdata[i].tree = tree;
        tdata[i].thread_id = i;
        pthread_create(&threads[i], NULL, stress_test_worker, &tdata[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // TODO:: Verify that nodes number is exactly NUM_THREADS * INSERTS_PER_THREAD
    
    rb_tree_delete(tree);
    printf("PASSED\n");
}

int main() {
    test_multithreaded_insertion();
    return 0;
}