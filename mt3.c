#include <stdio.h>

// 1 23 456 0

int f() {
    int n; 
    scanf("%d", &n); 
    printf("%d\n", n); 
    if (n != 0) {f();}
    printf("%d\n", n); 
}

// part a:
// 1
// 23
// 456
// 0
// 0 
// 456 
// 23
// 1

#define MAX_DEPTH 100

void func() {
    int seen[MAX_DEPTH]; 
    int size = 0; 
    int n = 1; 
    while (n != 0) {
        scanf("%d", &n); 
        printf("%d\n", n); 
        seen[size] = n; 
        size++; 
    }
    for (int i = size - 1; i >= 0; i--) {
        printf("%d\n", seen[i]); 
    }
}

int main(void) {
    func(); 
}