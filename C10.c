#include <stdio.h>
#include "C10.h"

void printNums(int m, int n) {
    int arr[10];
    arr[0] = m; // current number being checked starting from m
    
LOOP:
    arr[1] = arr[0]; // copy of current number for palindrome check
    arr[2] = 0; // reverse number for palindrome check
    
ISPALINDROME:
    arr[3] = arr[1] % 10; // last digit
    arr[2] = arr[2] * 10 + arr[3]; // add digit to reverse of number
    arr[1] = arr[1] / 10; // remove last digit
    if (arr[1] != 0) goto ISPALINDROME;
    // check for palindrome
    if (arr[2] != arr[0]) goto NEXT;
    // is palindrome so check for square free
    arr[5] = 2;  // current divisor to check
    
VALIDDIVISOR:
    if (arr[5] * arr[5] > arr[0]) goto PRINTNUM; // square free
    arr[6] = arr[0] % arr[5]; // remainder when divided by current divisor
    if (arr[6] != 0) goto NEXTDIVISOR; // not divisible by this divisor
    // if divisible by arr[5] check if divisible by its square
    arr[6] = arr[0] % (arr[5] * arr[5]); // remainder when divided by square of divisor
    if (arr[6] == 0) goto NEXT;    
    
NEXTDIVISOR:
    arr[5] = arr[5] + 1;
    goto VALIDDIVISOR;
    
PRINTNUM:
    printf("%d\n", arr[0]);
    
NEXT: // move onto next part of loop 
    arr[0] = arr[0] + 1;
    if (arr[0] <= n) goto LOOP; // if we pass n were done
    return;
}