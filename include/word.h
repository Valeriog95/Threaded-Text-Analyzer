/**
 * @file word.h
 * @author Valeriog95
 * @brief Definition of the word data structure and its management functions.
 * @details This module provides a fixed-size container for strings to avoid 
 * frequent dynamic memory allocations during text analysis.
 * @version 0.1
 * @date 2026-03-20
 * * @copyright Copyright (c) 2026
 */

#ifndef WORD_H
#define WORD_H

#include <stddef.h>  /* Required for size_t in C */
#include <stdbool.h>  /* Required for bool in C */

/** * @brief Maximum allowed length for a single word based on requirements.
 * @note Adjust this value if the input text contains longer tokens.
 */
#define MAX_WORD_SIZE 30

/**
 * @brief Represetation of a word within the analyzer.
 * @struct word_t
 * * This structure encapsulates a fixed-size character array and its 
 * actual logical length to provide O(1) access to the string size.
 */
typedef struct {
    size_t length;              /**< Actual number of characters in the word */
    char value[MAX_WORD_SIZE];  /**< Buffer containing the string null-terminated */
} word_t;

/**
 * @brief Initializes and returns a new word_t structure.
 * * @param string The source null-terminated string to copy.
 * @return word_t A new structure containing the copied string.
 * @warning If string exceeds MAX_WORD_SIZE, it will be truncated.
 */
word_t word_create(const char* string);

/**
 * @brief Updates the content of an existing word_t structure.
 * * @param word Pointer to the word_t instance to modify.
 * @param string The new null-terminated string to store.
 */
void word_modify(word_t* word, const char* string);

/**
 * @brief Resets the word structure by clearing the buffer and setting length to zero.
 * * @param word Pointer to the word_t instance to be cleared.
 */
void word_clear(word_t* word);

/**
 * @brief Compares two word_t structures for equality.
 * @details First checks length for O(1) mismatch detection, 
 * then performs a memory comparison.
 * * @param rhs Right-hand side word.
 * @param lhs Left-hand side word.
 * @return true if words are identical, false otherwise.
 */
bool word_compare(const word_t* rhs, const word_t* lhs);

#endif /* WORD_H */