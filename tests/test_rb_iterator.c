#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "map.h"

void test_iterator_sorting() {
    printf("Running Iterator Sorting Test... ");
    
    RB_Tree* tree = rb_tree_create();
    
    // Insert out of order
    rb_tree_insert(tree, "delta", NULL);
    rb_tree_insert(tree, "alpha", NULL);
    rb_tree_insert(tree, "charlie", NULL);
    rb_tree_insert(tree, "bravo", NULL);

    const char* expected[] = {"alpha", "bravo", "charlie", "delta"};
    int i = 0;

    // Use the iterator to traverse
    for (RB_Tree_Iterator it = rb_tree_it_begin(tree); 
         it != rb_tree_it_end(tree); 
         it = rb_tree_it_increment(tree, it)) 
    {
        assert(strcmp(it->key.value, expected[i]) == 0);
        i++;
    }

    assert(i == 4); // Ensure we visited all nodes
    assert(tree->size == 4); // Check also if size property works

    rb_tree_delete(tree);
    printf("PASSED\n");
}

void test_iterator_empty_tree() {
    printf("Running Iterator Empty Tree Test... ");
    RB_Tree* tree = rb_tree_create();
    
    RB_Tree_Iterator it = rb_tree_it_begin(tree);
    assert(it == rb_tree_it_end(tree));
    
    rb_tree_delete(tree);
    printf("PASSED\n");
}

int main() {
    test_iterator_sorting();
    test_iterator_empty_tree();
    return 0;
}