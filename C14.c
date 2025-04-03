#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct Node { // struct for dict
    int idx; 
    char* word; 
    struct Node* next; 
}; 

struct Node* cons(int idx, char* word, struct Node* nxt) { // append new word to front of dict 
    struct Node* new = malloc(sizeof(struct Node));
    new->idx = idx; 
    new->word = strdup(word); 
    new->next = nxt; 
    return new; 
}

char* get_word(int idx, struct Node* dict) {
    struct Node* cur = dict;
    while (cur != NULL) {
        if (cur->idx == idx) {
            return cur->word;
        }
        cur = cur->next;
    }
    return NULL;  
}

void free_node(struct Node* dict) {
    struct Node* cur = dict;
    while (cur != NULL) {
        struct Node* temp = cur;
        cur = cur->next;
        free(temp->word);
        free(temp);
    }
}

int main(void) {
    char buffer[81];
    char word[81];
    struct Node* dict = NULL;
    int next_idx = 0;
    
    // Process input line by line
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        int i = 0;
        int len = strlen(buffer);
        while (i < len) {
            if (isspace(buffer[i]) || (!isalnum(buffer[i]) && buffer[i] != '-')) {
                printf("%c", buffer[i]);
                i++;
                continue;
            }
            if (isdigit(buffer[i])) {
                int index = 0;
                while (i < len && isdigit(buffer[i])) {
                    index = index * 10 + (buffer[i] - '0');
                    i++;
                }
                char* lookup_word = get_word(index, dict);
                if (lookup_word != NULL) {
                    printf("%s", lookup_word);
                } else {
                    fprintf(stderr, "Invalid index: %d\n", index);
                }
                continue;
            }
            int word_len = 0;
            while (i < len && (isalnum(buffer[i]) || buffer[i] == '-')) {
                word[word_len++] = buffer[i++];
            }
            word[word_len] = '\0';
            printf("%s", word);
            dict = cons(next_idx++, word, dict);
        }
    }
    free_node(dict); 
    return 0;
}