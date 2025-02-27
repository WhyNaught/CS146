#include <stdio.h>

// return 0 for false, 1 for true 
int isRegular(int n) {
    while (n % 2 == 0) {
        n /= 2; 
    }
    while (n % 3 == 0) {
        n /= 3; 
    }
    while (n % 5 == 0) {
        n /= 5; 
    }
    if (n == 1) {
        return 1;
    }
    return 0; 
}

void regular(int n) {
    for (int i = 1; i <=n; i++) {
        if (isRegular(i) == 1) {
            printf("%d\n", i); 
        }
    }
}