/**
 * @file map_utils.h
 * @author Valeriog95
 * @brief Utilities and specializations for Red-Black Tree mappings.
 * @details Provides specialized wrappers for common RBT types (e.g., String to Integer)
 * to handle memory allocation and type casting automatically.
 * @version 1.0
 * @date 2026-03-20
 */

#ifndef MAP_UTILS_H
#define MAP_UTILS_H

#include "map.h"

/* --- Map<word_t, int> Specialization --- */
/* --- NOTE: Print function of the tree is not thread safe at the moment--- */

/**
 * @brief Creates an RB_Tree specialized for integer values.
 * @details Automatically sets up callbacks for heap-allocated integer 
 * deallocation and string conversion.
 * @return RB_Tree* Pointer to the specialized tree.
 */
RB_Tree* rb_tree_int_create();

/**
 * @brief Helper to insert an integer value into the tree.
 * @details Handles the dynamic allocation of the integer to ensure 
 * thread-safety and persistence within the generic tree.
 * @param tree Pointer to the tree.
 * @param key String key.
 * @param value Integer value to store.
 */
void rb_tree_int_insert(RB_Tree *tree, const char *key, int value);

#endif /* MAP_UTILS_H */