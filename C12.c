#include "C12.h"
#include <stdlib.h>
#include <string.h>

struct hash make_table(int s) {
    struct hash res;
    res.size = s;
    res.table = (struct anode **)malloc(s * sizeof(struct anode *));
    for (int i = 0; i < s; i++) {
        res.table[i] = NULL;
    }
    return res;
}

char *search(struct hash T, int k) {
    int index = k % T.size;
    struct anode *current = T.table[index];
    while (current != NULL) {
        if (current->key == k) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void add(struct hash T, int k, char *v) {
    int index = k % T.size;
    struct anode *current = T.table[index];
    while (current != NULL) {
        if (current->key == k) {
            free(current->value);
            current->value = strdup(v);
            return;
        }
        current = current->next;
    }
    struct anode *new_node = (struct anode *)malloc(sizeof(struct anode));
    new_node->key = k;
    new_node->value = strdup(v);
    new_node->next = T.table[index];
    T.table[index] = new_node;
}

void delete(struct hash T, int k) {
    int index = k % T.size;
    struct anode **prev_ptr = &T.table[index];
    struct anode *current = T.table[index];
    while (current != NULL) {
        if (current->key == k) {
            struct anode *to_delete = current;
            *prev_ptr = current->next;
            current = current->next;
            free(to_delete->value);
            free(to_delete);
        } else {
            prev_ptr = &current->next;
            current = current->next;
        }
    }
}

void free_table(struct hash T) {
    for (int i = 0; i < T.size; i++) {
        struct anode *current = T.table[i];
        while (current != NULL) {
            struct anode *temp = current;
            current = current->next;
            free(temp->value);
            free(temp);
        }
    }
    free(T.table);
}