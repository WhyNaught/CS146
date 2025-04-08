#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data; 
    struct Node* next; 
}; 

struct Node* cons(int data, struct Node* next) {
    struct Node* result = malloc(sizeof(struct Node)); 
    result->data = data; 
    result->next = next; 
    return result; 
}

struct Node* append(struct Node* lst1, struct Node* lst2) {
    if (lst1 == NULL) {
        return lst2; 
    } else {
        return cons(lst1->data, append(lst1->next, lst2));
    }
}

struct Node* destructiveAppend(struct Node* l1, struct Node* l2) {
    if (l1 == NULL) {
        return l2; 
    } else if (l1->next == NULL) {
        l1->next = l2; 
    } else {
        destructiveAppend(l1->next, l2); 
    }
    return l1; 
}

struct TreeNode {
    int data; 
    struct TreeNode *left, *right; 
};

struct Node* traversal_helper(struct TreeNode* root, struct Node* acc) {
    if (!root) {
        return acc; 
    } else {
        return traversal_helper(root->left, cons(root->data, traversal_helper(root->right, acc))); 
    }
}

struct Node* inorderTraversal(struct TreeNode* root) {
    return traversal_helper(root, NULL); 
}