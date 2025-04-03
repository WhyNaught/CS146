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

int search(char* word, struct Node* dict) { // we will return -1 if not found 
    struct Node* cur = dict; 
    while (cur != NULL) {
        if (strcmp(cur->word, word) == 0) { 
            return cur->idx; // we immediately return the index number 
        }
        cur = cur->next; 
    }
    return -1; 
}

int main(void) {
    int index = 0; // start indexing at 0
    char buffer[81]; // 80 chars + null terminator
    struct Node* dict = NULL; 

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        int i = 0;
        int start = -1;

        int len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
            len--;
        }
        
        i = 0;
        while (i <= len) { 
            if (i < len && isalpha(buffer[i]) && start == -1) {
                start = i; 
            }
            if ((i == len || !isalpha(buffer[i])) && start != -1) {
                char temp = buffer[i]; 
                buffer[i] = '\0';     
                int status = search(buffer + start, dict); 
                if (status == -1) {
                    printf("%s", buffer + start);
                    dict = cons(index, buffer + start, dict); // update dict 
                    index++; 
                } else {
                    // print the index for subsequent occurrences
                    printf("%d", status); 
                }
                
                buffer[i] = temp;  
                if (i < len) {
                    printf("%c", buffer[i]);
                }
                start = -1; 
            }
            i++;
        }
        printf("\n"); 
    }
    
    struct Node* current = dict;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp->word);
        free(temp);
    }
    
    return 0;
}