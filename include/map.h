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
#include <stdbool.h>
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
    size_t size;                /**< the size of the tree */

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
 * @brief Atomically retrieves an existing node or inserts a new one if the key is absent.
 * @details This function is the thread-safe "entry point" for map modifications. 
 * It prevents race conditions by holding the tree mutex throughout the entire 
 * "search-then-insert" sequence.
 * @param tree Pointer to the Red-Black Tree.
 * @param key The string key to search or insert.
 * @param value Pointer to the value to be stored if the key is missing.
 * @return RB_Node* Pointer to the node associated with the key (either existing or new).
 */
RB_Node* rb_tree_get_or_insert(RB_Tree *tree, const char *key, void* value);

/**
 * @brief Prints the tree using In-Order traversal (Alphabetical).
 */
void rb_tree_print(RB_Tree *tree);

/**
 * @brief Deallocates the entire Red-Black Tree and all its associated memory.
 * @details This function performs a post-order traversal to safely free each node,
 * the sentinel NIL node, and finally the tree controller itself.
 * @param tree Pointer to the tree to be destroyed.
 */
void rb_tree_delete(RB_Tree *tree);

/**
 * @brief Callback for atomic operations on a tree node.
 * @param node The node found or newly created.
 * @param was_inserted true if the node was just created, false if it already existed.
 */
typedef void (*RB_Tree_Atomic_Action)(RB_Node* node, bool was_inserted);

/**
 * @brief Thread-safe "Get or Create" operation that executes a custom action.
 * @details Searches for a key. If not found, creates a node with a NULL value.
 * Then executes the provided callback while holding the tree lock.
 * This ensures the entire check-insert-update sequence is atomic.
 * @param tree Pointer to the Red-Black Tree.
 * @param key The string key to search or insert.
 * @param action The function to execute on the resulting node.
 */
void rb_tree_get_or_insert_execute(RB_Tree *tree, const char *key, RB_Tree_Atomic_Action action);

/* -------------------------------------------------------------------------- */
/* ITERATOR INTERFACE                                                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief Red-Black Tree Iterator (Alias for Node pointer).
 * @warning THE ITERATOR IS NOT THREAD-SAFE. 
 * If the tree is modified (inserted/deleted) by another thread while 
 * iterating, the behavior is UNDEFINED. 
 * The caller must hold the tree lock manually if concurrent access is expected.
 */
typedef RB_Node* RB_Tree_Iterator;

/**
 * @brief Returns an iterator to the first element (smallest key).
 * @return RB_Node* pointing to the minimum node or tree->nil.
 */
RB_Tree_Iterator rb_tree_it_begin(RB_Tree* tree);

/**
 * @brief Returns an iterator representing the end of the tree.
 * @return tree->nil.
 */
RB_Tree_Iterator rb_tree_it_end(RB_Tree* tree);

/**
 * @brief Advances the iterator to the next element in alphabetical order.
 * @details Implements the In-Order Successor algorithm.
 * @param tree Pointer to the tree (needed to identify the NIL sentinel).
 * @param it Current iterator position.
 * @return The next RB_Node* in sequence or tree->nil (end()) if finished.
 */
RB_Tree_Iterator rb_tree_it_increment(RB_Tree* tree, RB_Tree_Iterator it);

#endif /* MAP_H */