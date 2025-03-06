#include <stdlib.h>
#include <stdio.h>

struct Node {
    int data; 
    struct Node* next; 
}; 

struct Node *reverse(struct Node* lst) {
    struct Node* acc = NULL; 
    struct Node* cur = lst; 
    while (cur != NULL) {
        struct Node* temp = cur->next; 
        cur->next = acc; 
        acc = cur; 
        cur = temp; 
    }
    return acc; 
}