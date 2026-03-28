/**
 * @file test_word.c
 * @brief Some basic the cases for word implementation 
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "word.h"

void test_word_creation() {
    printf("Running test_word_creation... ");
    
    word_t w = word_create("Hello");
    assert(w.length == 5);
    assert(strcmp(w.value, "Hello") == 0);
    
    // Test truncation and null-termination
    word_t long_w = word_create("ThisIsAStringThatIsDefinitelyWayTooLongForOurBuffer");
    assert(long_w.length == MAX_WORD_SIZE - 1);
    assert(long_w.value[MAX_WORD_SIZE - 1] == '\0');
    
    printf("PASSED\n");
}

void test_word_compare() {
    printf("Running test_word_compare... ");
    
    word_t w1 = word_create("Apple");
    word_t w2 = word_create("Apple");
    word_t w3 = word_create("Orange");
    word_t w4 = word_create("Apples"); // Differenet length

    assert(word_compare(&w1, &w2) == true);
    assert(word_compare(&w1, &w3) == false);
    assert(word_compare(&w1, &w4) == false);
    
    printf("PASSED\n");
}

void test_word_clear() {
    printf("Running test_word_clear... ");
    
    word_t w = word_create("ClearMe");
    word_clear(&w);
    
    assert(w.length == 0);
    assert(w.value[0] == '\0');
    
    printf("PASSED\n");
}

int main() {
    printf("--- STARTING WORD_T TEST SUITE ---\n");
    
    test_word_creation();
    test_word_compare();
    test_word_clear();
    
    printf("--- ALL TESTS PASSED SUCCESSFULLY ---\n");
    return 0;
}