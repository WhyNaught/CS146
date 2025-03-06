#include <stdio.h>
#include <stdlib.h>

struct Node {
    int bigit;
    struct Node *next;
};

// appending bigit to nxt
struct Node *cons_bigit(int bgt, struct Node *nxt) {
    struct Node *result = malloc(sizeof(struct Node));
    // just for testing purposes
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    result->bigit = bgt;
    result->next = nxt;
    return result;
}

// frees all memory used by lst iteratively 
void free_num(struct Node *blst) {
    struct Node *current = blst;
    struct Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

// copies a blst and returns a brnad new copy stored on the heap 
struct Node *copy_num(struct Node *nlst) {
    if (nlst == NULL) {
        return NULL;
    }
    // create dummy node 
    struct Node dummy;
    struct Node *tail = &dummy;
    struct Node *current = nlst;
    
    while (current != NULL) {
        tail->next = cons_bigit(current->bigit, NULL);
        tail = tail->next;
        current = current->next;
    }
    
    struct Node *result = dummy.next;
    return result;
}

// prints num in human readable form 
void print_num(struct Node *nlst) {
    if (nlst == NULL) {
        printf("0");
        return;
    }
    struct Node *digits[100];  
    int count = 0;
    struct Node *current = nlst;
    while (current != NULL) {
        digits[count++] = current;
        current = current->next;
    }
    // check if number is all zeros (edge case)
    int all_zeros = 1;
    for (int i = 0; i < count; i++) {
        if (digits[i]->bigit != 0) {
            all_zeros = 0;
            break;
        }
    }
    if (all_zeros) {
        printf("0");
        return;
    }
    // find most significant non-zero bigit (eliminate leading zeroes)
    int start_index = count - 1;
    while (start_index >= 0 && digits[start_index]->bigit == 0) {
        start_index--;
    }
    // prints from most significant non-zero bigit to least significant
    for (int i = start_index; i >= 0; i--) {
        if (i == start_index) {
            // edge case for leading zeroes
            printf("%d", digits[i]->bigit);
        } else {
            // all others have leading zeroes 
            printf("%04d", digits[i]->bigit);
        }
    }
}

// adds two blsts, returns a pointer to a new list 
struct Node *add(struct Node *n1lst, struct Node *n2lst) {
    struct Node dummy;
    struct Node *tail = &dummy;
    dummy.next = NULL;
    struct Node *p1 = n1lst;
    struct Node *p2 = n2lst;
    int carry = 0;
    // loop through both lists and add together each digit 
    while (p1 != NULL || p2 != NULL || carry != 0) {
        int sum = carry;
        if (p1 != NULL) {
            sum += p1->bigit;
            p1 = p1->next;
        }
        if (p2 != NULL) {
            sum += p2->bigit;
            p2 = p2->next;
        }
        tail->next = cons_bigit(sum % 10000, NULL); 
        tail = tail->next;
        carry = sum / 10000;
    }
    return dummy.next;
}

// multiplies nlst by each singular digit, helper function for mult 
struct Node *mult_by_digit(struct Node *nlst, int digit, int shift) {
    if (digit == 0) {
        return NULL;  // res is 0
    }
    struct Node dummy;
    struct Node *tail = &dummy;
    dummy.next = NULL; 
    // add shifting zeros (like elementary school multiplication)
    for (int i = 0; i < shift; i++) {
        tail->next = cons_bigit(0, NULL);
        tail = tail->next;
    }
    struct Node *current = nlst;
    int carry = 0;
    while (current != NULL || carry != 0) {
        int product = carry;
        if (current != NULL) {
            product += current->bigit * digit;
            current = current->next;
        }
        tail->next = cons_bigit(product % 10000, NULL); 
        tail = tail->next;
        carry = product / 10000;
    }
    return dummy.next;
}

// multiplies two blsts together 
// frees all temporary blsts from the heap after 
struct Node *mult(struct Node *n1lst, struct Node *n2lst) {
    struct Node *result = NULL;
    struct Node *p2 = n2lst;
    int position = 0;
    // iterate through n2lst
    while (p2 != NULL) {
        // Multiply n1lst by this digit and shift
        struct Node *partial = mult_by_digit(n1lst, p2->bigit, position);
        // Add to the running total
        struct Node *new_result = add(result, partial);
        // free the temporary results from the heap
        free_num(result);
        free_num(partial);
        result = new_result;
        p2 = p2->next;
        position++;
    }
    
    return result ? result : cons_bigit(0, NULL);  // return 0 for null 
}