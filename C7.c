#include <stdio.h>
#include "array.h" 

typedef struct {
    int top;     // Top index in the array
    int start;   // Start index of this stack
} Stack;

// please note this is not a separate array, but rather just some info about the end and size of each stack 
Stack stacks[3]; 
int size = 0;  

void shift_right(int start) {
    for (int i = size; i > start; i--) {
        int val = get(i - 1);
        put(i, val);
    }
}

void shift_left(int start) {
    for (int i = start; i < size - 1; i++) {
        int val = get(i + 1);
        put(i, val);
    }
}

void push(int stack_id, int value) {
    if (size >= 21) {
        printf("Error: Out of space\n");
        return;
    }
    if (stacks[stack_id].top + 1 < stacks[(stack_id + 1) % 3].start) {
        stacks[stack_id].top++;
    } else {
        shift_right(stacks[(stack_id + 1) % 3].start);
        stacks[(stack_id + 1) % 3].start++;
        stacks[stack_id].top++;
    }
    put(stacks[stack_id].top, value);
    size++;
}

void pop(int stack_id) {
    if (stacks[stack_id].top < stacks[stack_id].start) {
        printf("Error: Stack %d is empty\n", stack_id);
        return;
    }
    int value = get(stacks[stack_id].top);
    printf("%d\n", value);
    stacks[stack_id].top--;
    size--;
    shift_left(stacks[stack_id].top + 1);
}

void init_stacks() {
    stacks[0].start = 0;
    stacks[1].start = 7;
    stacks[2].start = 14;
    stacks[0].top = -1;
    stacks[1].top = 6;
    stacks[2].top = 13;
}

int main() {
    init_stacks();
    char op;
    int stack_id, value;
    while (scanf(" %c %d", &op, &stack_id) == 2) {
        if (op == 'u') {
            scanf("%d", &value);
            push(stack_id, value);
        } else if (op == 'o') {
            pop(stack_id);
        }
    }
    return 0;
}