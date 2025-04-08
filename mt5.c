#include <stdio.h>

void incCounter(int counter[], int size) {
    for (int i = 0; i < size; i++) {
        if (counter[i] == 0) {
            counter[i] = 1;
            for (int j = i - 1; j >= 0; j--) {
                counter[i] = 0; 
            }
            return; 
        }
    }
    for (int k = 0; k < size; k++) {
        counter[k] = 0; 
    }
    return; 
}

// part b: worst case is O(countersize)

// part c: n + n/2 + n/4 + n/8 + ... 
// this is n / (1 - 1/2) = 2n times on average

// part d: amortized is O(1), as for every function call that requires traversal of the full array there will be many more that
// will be instantaneous, and so the running time is on average constant, or O(1)