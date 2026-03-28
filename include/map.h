/**
 * @file map.h
 * @author Valeriog95
 * @brief Red-Black Tree (RBT) interface for associative mapping.
 * @details Provides a self-balancing binary search tree to ensure O(log n) 
 * performance for insertions and lookups. This implementation uses a 
 * sentinel node (NIL) to simplify boundary condition handling.
 * @version 0.1
 * @date 2026-03-20
 * @copyright Copyright (c) 2026
 */

#ifndef MAP_H
#define MAP_H

#include "word.h"

/**
 * @brief Node color enumeration for RBT balancing properties.
 */
typedef enum { 
    RED,    /**< New nodes are usually inserted as RED */
    BLACK   /**< Root and NIL nodes are always BLACK */
} RB_Color;

/**
 * @brief A single node within the Red-Black Tree.
 * @struct RB_Node
 * @details Encapsulates the key-value pair and structural pointers.
 * The use of a 'parent' pointer is mandatory for rebalancing after insertion.
 */
typedef struct RB_Node {
    word_t key;                 /**< Fixed-size word used as the unique key */
    int value;                  /**< Associated value (e.g., occurrence count) */
    RB_Color color;             /**< Current node color for balancing logic */
    struct RB_Node *left;       /**< Pointer to the left child */
    struct RB_Node *right;      /**< Pointer to the right child */
    struct RB_Node *parent;     /**< Pointer to the parent node */
} RB_Node;

/**
 * @brief Red-Black Tree controller structure.
 * @struct RB_Tree
 * @details Tracks the root of the tree and the shared NIL sentinel node.
 */
typedef struct {
    RB_Node *root;              /**< Entry point of the tree */
    RB_Node *nil;               /**< Shared sentinel node representing leaf nodes */
} RB_Tree;

/**
 * @brief Allocates and initializes a new Red-Black Tree.
 * @details Sets up the NIL sentinel node and an empty root.
 * @return RB_Tree* Pointer to the initialized tree controller.
 */
RB_Tree* rb_tree_create();

/**
 * @brief Inserts a new key-value pair into the tree if the key does not exist.
 * @param tree Pointer to the tree controller.
 * @param key String to be stored (converted to word_t).
 * @param value Integer value to associate with the new key.
 * @note If the key already exists, the function returns without modifying the tree.
 * @note Triggers RBT rebalancing (rotations/recoloring) to maintain O(log n) height.
 */
void rb_tree_insert(RB_Tree *tree, const char *key, int value);

/**
 * @brief Searches for a node by its key.
 * @param tree Pointer to the tree controller.
 * @param key The string to look for.
 * @return RB_Node* Pointer to the found node, or tree->nil if not found.
 */
RB_Node* rb_tree_at(RB_Tree *tree, const char *key);

/**
 * @brief Performs an In-Order traversal to print the tree content.
 * @details Useful for debugging and verifying alphabetical order.
 * @param tree Pointer to the tree controller.
 */
void rb_tree_print(RB_Tree *tree);

#endif /* MAP_H */