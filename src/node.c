
#include <stdlib.h>
#include "word.h"
#include "map.h"

static void left_rotate(RB_Tree *tree, RB_Node *x);
static void right_rotate(RB_Tree *tree, RB_Node *y);

void insert_fixup(RB_Tree *tree, RB_Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RB_Node *y = z->parent->parent->right;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            RB_Node *y = z->parent->parent->left;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

RB_Node* rb_node_create(RB_Tree *tree, const char *key, void* value)
{
    RB_Node *node = (RB_Node*)malloc(sizeof(RB_Node));
    node->key = word_create(key);
    node->value = value;
    node->color = RED;
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;
    return node;
}

static void left_rotate(RB_Tree *tree, RB_Node *x) {
    
    RB_Node *y = x->right;
    
    x->right = y->left;

    if (y->left != tree->nil)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == tree->nil)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void right_rotate(RB_Tree *tree, RB_Node *y) {
    RB_Node *x = y->left;
    y->left = x->right;

    if (x->right != tree->nil)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == tree->nil)
        tree->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}