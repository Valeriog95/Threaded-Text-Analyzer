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

    if (pthread_mutex_init(&tree->lock, NULL) != 0) {
        free(tree->nil);
        free(tree);
        return NULL;
    }
    
    return tree;
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

    if (rb_tree_search_recursive(tree,tree->root,key) == tree->nil){

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
    }
    
    pthread_mutex_unlock(&tree->lock);

}


RB_Node* rb_tree_at(RB_Tree *tree,const char *key)
{
    // Recursive search in aproximatively log(n)
    return tree ? rb_tree_search(tree,tree->root,key) : NULL;
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
