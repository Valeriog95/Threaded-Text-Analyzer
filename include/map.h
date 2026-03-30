/**
 * @file map.h
 * @author Valeriog95
 * @brief Red-Black Tree (RBT) interface for associative mapping.
 * @details Provides a self-balancing binary search tree to ensure O(log n) 
 * performance for insertions and lookups. This implementation uses a 
 * sentinel node (NIL) to simplify boundary condition handling.
 * @version 0.2
 * @date 2026-03-20
 */

#ifndef MAP_H
#define MAP_H

#include <pthread.h>
#include "word.h"

/**
 * @brief Node color enumeration for RBT balancing properties.
 */
typedef enum { 
    RED,    /**< New nodes are usually inserted as RED to minimize black-height violations */
    BLACK   /**< Root and NIL nodes are always BLACK by definition */
} RB_Color;

/**
 * @brief A single node within the Red-Black Tree.
 * @struct RB_Node
 * @details Encapsulates a unique word_t key and a generic pointer value.
 */
typedef struct RB_Node {
    word_t key;                 /**< Fixed-size word (unique key) */
    void* value;                /**< Opaque pointer to user-defined data */
    RB_Color color;             /**< Node color (RED or BLACK) */
    struct RB_Node *left;       /**< Left child pointer (smaller keys) */
    struct RB_Node *right;      /**< Right child pointer (larger keys) */
    struct RB_Node *parent;     /**< Parent pointer (required for rotations) */
} RB_Node;

/**
 * @brief Red-Black Tree controller structure.
 * @struct RB_Tree
 * @details Management structure that holds the tree state and behavior.
 */
typedef struct {
    RB_Node *root;              /**< Root of the tree */
    RB_Node *nil;               /**< Universal NIL sentinel (black, no children) */
    pthread_mutex_t lock;       /**< tree lock  */

    /** * @brief Optional callback to free the generic 'value' during node deletion.
     * Set to NULL if the value doesn't require dynamic deallocation.
     */
    void (*free_value)(void* value); 
    
    /** * @brief Optional callback to convert 'value' to string for printing/logging.
     */
    const char* (*value_as_string)(void* value); 
} RB_Tree;

/**
 * @brief Initializes a new RB Tree with the NIL sentinel.
 * @return RB_Tree* Newly allocated tree controller.
 */
RB_Tree* rb_tree_create();

/**
 * @brief Destroys the tree and performs cleanup.
 * @details Uses post-order traversal. If free_value is set, it calls it on every node.
 */
void rb_tree_delete(RB_Tree *tree);

/**
 * @brief Inserts a key-value pair.
 * @param value Pointer to data. Note: the tree does NOT copy the data, only the pointer.
 * @note If key exists, insertion is aborted (Unique Key Policy).
 */
void rb_tree_insert(RB_Tree *tree, const char *key, void* value);

/**
 * @brief Retrieves a node by its string key.
 * @return Pointer to node or tree->nil if not found.
 */
RB_Node* rb_tree_at(RB_Tree *tree, const char *key);

/**
 * @brief Prints the tree using In-Order traversal (Alphabetical).
 */
void rb_tree_print(RB_Tree *tree);

#endif /* MAP_H */