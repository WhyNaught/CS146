#include <stdio.h>

void prime_factors(int n) {
    int factor = 2; 
    while (n > 1) {
        if (n % factor == 0) {
            printf("%d\n", factor); 
            n = n / factor; 
        } else {
            factor++; 
        }
    }
}