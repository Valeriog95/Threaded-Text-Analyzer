#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

extern void insert_fixup(RB_Tree *tree, RB_Node *z);

extern RB_Node* rb_node_create(RB_Tree *tree, const char *key, int value);

static RB_Node* rb_tree_search(RB_Tree *tree, RB_Node *node, const char *key);

static void rb_tree_print_(RB_Tree *tree, RB_Node *node);

// TODO: static creation of tree->nil here

RB_Tree* rb_tree_create() {
    
    RB_Tree *tree = (RB_Tree*)malloc(sizeof(RB_Tree));
    
    if(!tree) return NULL;

    // Create tree->nil and protect it by pointing to itself to avoid segmentation error
    tree->nil = (RB_Node*)malloc(sizeof(RB_Node));
    tree->nil->left = tree->nil->right = tree->nil->parent = tree->nil;
    tree->nil->color = BLACK;    
    tree->root = tree->nil;
    return tree;
}


void rb_tree_insert(RB_Tree *tree, const char *key, int value) {
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


RB_Node* rb_tree_at(RB_Tree *tree,const char *key)
{
    // Recursive search in aproximatively log(n)
    return rb_tree_search(tree,tree->root,key);
}


void rb_tree_print(RB_Tree *tree)
{
    rb_tree_print_(tree,tree->root);
}


void rb_tree_print_(RB_Tree *tree, RB_Node *node) {
    // visit tree in-order
    if (node != tree->nil) {
        rb_tree_print_(tree, node->left);
        printf("%s => %d (%s)\n", node->key.value, node->value,
               node->color == RED ? "RED" : "BLACK");
        rb_tree_print_(tree, node->right);
    }
}


RB_Node* rb_tree_search(RB_Tree *tree, RB_Node *node, const char *key) {
    if (node == tree->nil || strcmp(key, node->key.value) == 0)
        return node;

    if (strcmp(key, node->key.value) < 0)
        return rb_tree_search(tree, node->left, key);
    else
        return rb_tree_search(tree, node->right, key);
}
