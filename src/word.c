/**
 * @file word.c
 * @brief Implementation of word management functions with safety checks.
 */

#include "word.h"
#include <string.h>

word_t word_create(const char* string) {

    word_t result;

    modify_word(&result, string);
    
    return result;
}

void word_modify(word_t* word, const char* string) {

    if (!word || !string) return;

    /* * Safety first: Use strncpy to prevent buffer overflow.
     * We leave 1 byte for the null terminator '\0'.
     */
    const size_t max_copy = MAX_WORD_SIZE - 1;
    
    strncpy(word->value, string, max_copy);
    
    /* Ensure null-termination if the source was too long */
    word->value[max_copy] = '\0';
    
    /* Set logical length based on the actual copied content */
    word->length = strlen(word->value);
}

void word_clear(word_t* word) {

    if (!word) return;

    word->value[0] = '\0';
    word->length = 0;
}