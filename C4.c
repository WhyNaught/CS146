#include <stdio.h>

int isqrt(int n) {
    if (n < 0) return -1;
    int low = 0;
    int high = 46340; 
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int square = mid * mid;
        if (square == n) return mid;
        if (square > n) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

void sumsqr(int n) {
    int lim = isqrt(n) + 1; 
    for (int x = 0; x <= lim; x++) {
        for (int y = 0; y <= x; y++) {
            if (x*x + y*y == n) {
                printf("%d %d\n", x, y); 
            }
        }
    }
}