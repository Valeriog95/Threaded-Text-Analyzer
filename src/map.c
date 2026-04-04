#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

extern void insert_fixup(RB_Tree *tree, RB_Node *z);
extern RB_Node* rb_node_create(RB_Tree *tree, const char *key, void* value);

static void rb_tree_delete_recursive(RB_Tree *tree, RB_Node *node);
static void rb_tree_print_recursive(RB_Tree *tree, RB_Node *node);
static RB_Node* rb_tree_search(RB_Tree *tree, RB_Node *node, const char *key);
static RB_Node* rb_tree_search_recursive(RB_Tree *tree, RB_Node *node, const char *key);
static RB_Node* rb_tree_create_and_insert(RB_Tree *tree, const char *key, void* value);
static RB_Node* rb_tree_minimum(RB_Tree* tree, RB_Node* x);

// TODO: static creation of tree->nil here

RB_Tree* rb_tree_create() {
    
    RB_Tree *tree = (RB_Tree*)malloc(sizeof(RB_Tree));
    
    if(!tree) return NULL;

    // Create tree->nil and protect it by pointing to itself to avoid segmentation error
    tree->nil = (RB_Node*)malloc(sizeof(RB_Node));
    tree->nil->left = tree->nil->right = tree->nil->parent = tree->nil;
    tree->nil->color = BLACK;    
    tree->nil->value = NULL; // Set to NULL to avoid free() during node deletion ...    
    tree->root = tree->nil;
    tree->free_value = NULL;
    tree->value_as_string = NULL;
    tree->size = 0;

    if (pthread_mutex_init(&tree->lock, NULL) != 0) {
        free(tree->nil);
        free(tree);
        return NULL;
    }
    
    return tree;
}


void rb_tree_delete(RB_Tree *tree){

    if(tree){
        rb_tree_delete_recursive(tree,tree->root);
        pthread_mutex_destroy(&tree->lock);
        free(tree->nil);
        free(tree);    
    }
}

void rb_tree_insert(RB_Tree *tree, const char *key, void* value) {
    
    if(!tree) return;

    pthread_mutex_lock(&tree->lock);

    if (rb_tree_search_recursive(tree,tree->root,key) == tree->nil)

        rb_tree_create_and_insert(tree, key, value);
    
    
    pthread_mutex_unlock(&tree->lock);

}


RB_Node* rb_tree_at(RB_Tree *tree,const char *key)
{
    // Recursive search in aproximatively log(n)
    return tree ? rb_tree_search(tree,tree->root,key) : NULL;
}

RB_Node* rb_tree_get_or_insert(RB_Tree *tree, const char *key, void* value) {

    if (!tree) return NULL;

    pthread_mutex_lock(&tree->lock);

    // Search for the key using the internal recursive function (no lock) 
    RB_Node *node = rb_tree_search_recursive(tree, tree->root, key);

    // Node not found, create it
    if (node == tree->nil) 
        node = rb_tree_create_and_insert(tree, key, value);

    pthread_mutex_unlock(&tree->lock);

    return node;
}


void rb_tree_print(RB_Tree *tree)
{
    if(tree){

        pthread_mutex_lock(&tree->lock);

        rb_tree_print_recursive(tree,tree->root);

        pthread_mutex_unlock(&tree->lock);
    }
}


void rb_tree_print_recursive(RB_Tree *tree, RB_Node *node) {
    
    // visiting tree in-order
    if (node != tree->nil) {
        rb_tree_print_recursive(tree, node->left);
        printf(
            "%s => %s (%s)\n",
            node->key.value,
            tree->value_as_string != NULL ? tree->value_as_string(node->value) : "",
            node->color == RED ? "RED" : "BLACK");
        rb_tree_print_recursive(tree, node->right);
    }
}

void rb_tree_get_or_insert_execute(RB_Tree *tree, const char *key, RB_Tree_Atomic_Action action) {
    
    if (!tree || !action) return;

    pthread_mutex_lock(&tree->lock);

    // Internal search without re-locking
    RB_Node *node = rb_tree_search_recursive(tree, tree->root, key);

    // Create if not exists and execute action while still holding the lock
    if (node == tree->nil) {
        node = rb_tree_create_and_insert(tree, key, NULL);
        action(node, true);
    } else{
        action(node, false);
    }

    pthread_mutex_unlock(&tree->lock);
}

RB_Tree_Iterator rb_tree_it_begin(RB_Tree* tree) {
    if (!tree || tree->root == tree->nil) 
        return tree ? tree->nil : NULL;
        
    // The start of an in-order traversal is the leftmost node of the root
    return rb_tree_minimum(tree, tree->root);
}

RB_Tree_Iterator rb_tree_it_end(RB_Tree* tree) {
    return tree ? tree->nil : NULL;
}

RB_Tree_Iterator rb_tree_it_increment(RB_Tree* tree, RB_Tree_Iterator it) {
    if (!tree || it == tree->nil) return tree->nil;

    /* * Case 1: If the right subtree is not empty, the successor is the 
     * minimum node in the right subtree.
     */
    if (it->right != tree->nil) {
        return rb_tree_minimum(tree, it->right);
    }

    /* * Case 2: If the right subtree is empty, we go up the tree until 
     * we find a node that is the LEFT child of its parent.
     */
    RB_Node* y = it->parent;
    while (y != tree->nil && it == y->right) {
        it = y;
        y = y->parent;
    }
    
    return y;
}

static RB_Node* rb_tree_search(RB_Tree *tree, RB_Node *node, const char *key){

    pthread_mutex_lock(&tree->lock);
    
    RB_Node* result = rb_tree_search_recursive(tree, tree->root, key);
    
    pthread_mutex_unlock(&tree->lock);
    
    return result;
}

static RB_Node* rb_tree_search_recursive(RB_Tree *tree, RB_Node *node, const char *key) {
    if (node == tree->nil || strcmp(key, node->key.value) == 0)
        return node;

    if (strcmp(key, node->key.value) < 0)
        return rb_tree_search_recursive(tree, node->left, key);
    else
        return rb_tree_search_recursive(tree, node->right, key);
}

static RB_Node* rb_tree_create_and_insert(RB_Tree *tree, const char *key, void* value){

    RB_Node *z = rb_node_create(tree, key, value);
    RB_Node *y = tree->nil;
    RB_Node *x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (strcmp(z->key.value, x->key.value) < 0)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == tree->nil)
        tree->root = z;
    else if (strcmp(z->key.value, y->key.value) < 0)
        y->left = z;
    else
        y->right = z;
    
    insert_fixup(tree, z);

    tree->size++;

    return z;
}

static void rb_tree_delete_recursive(RB_Tree *tree, RB_Node *node){
    
    // Post order visit here to firstly dealloc the children ...
    if(node != tree->nil){
        rb_tree_delete_recursive(tree,node->left);
        rb_tree_delete_recursive(tree,node->right);
        
        if(tree->free_value != NULL && node->value != NULL)
            tree->free_value(node->value);

        free(node);
    }
} 

static RB_Node* rb_tree_minimum(RB_Tree* tree, RB_Node* x) {
    if (x == tree->nil) return tree->nil;
    while (x->left != tree->nil) {
        x = x->left;
    }
    return x;
}