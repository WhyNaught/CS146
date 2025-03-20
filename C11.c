#include <stdio.h>

int lowsearch(int A[], int key, int n) {
    int low = 0;
    int high = n - 1;
    /* 
    Loop invariant:
        1. 0 <= low <= n and -1 <= high <= n - 1 
        2. (forall i, 0 <= i <= high -> A[i] < key)  
        3. (forall i, low <= i <= n - 1 -> A[i] >= key or low > n - 1)
    All 3 parts of the invariant must be satisified for the entire loop invariant to be satisfied. 
    
    Precondition: A is sorted in non-decreasing order and n >= 1 and A[0] <= key <= A[n - 1]

    We now provide a proof of partial correctness (that is, a proof of correctness without proof of termination). 

    Step 1: Initialization 
        low = 0; high = n - 1; 
        We now check our invariant. 
        0 <= low <= n is true since low = 0 and n >= 1 and -1 <= high <= n - 1 is true since high = n - 1
        (forall i, low <= i <= n - 1 -> A[i] >= key OR low > n-1) is true since key <= A[n - 1]
        (forall i, 0 <= i <= high -> A[i] < key) is true since if key <= A[0] the loop sets high to -1
        and terminates with low = 0, as desired, otherwise the part of the invariant holds trivially as key > A[0]
        Since all 3 parts of the invariant hold at initialization, the invariant thus holds for the start of the loop. 

    Step 2: Maintenance 
        Suppose that the invariant holds for the start of the loop iteration 
        Case 1: Key > A[mid]
            We update low such that low = mid + 1. 
            Part 1 of the invariant holds as mid < n and so mid + 1 is at most n and since high is unchanged and already works by assumption. 
            Thus, for all i where 0 <= i <= mid, A[i] < key, preserving part 2 of the invariant. 
            Part 3 of the invariant holds as low only ever increases in this case, and we know all elements in the right of an element
            in a sorted array are greater than or equal to that element. 
            Since parts 1-3 of the invariant hold in this case, the invariant is thus maintained in this case. 
        Case 2: A[mid] >= key
            We update high such that high = mid - 1. 
            Part 1 of the invariant holds by assumption as we do not update low and since high can only be at least -1 since mid >= 0. 
            Part 2 of the invariant holds by our assumption as high is decreasing and we know all elements in the left of a sorted array 
            are less than or equal to the current element, so A[i] < key
            Part 3 of the invariant holds by assumption as low is unchanged, and we had assumed it to work for the current value of low. 
            Since parts 1-3 of the invariant hold in this case, the invariant is thus maintained in this case. 
        Thus in both cases, the invariant is maintained for each iteration of the loop, as desired. 
    
    Step 3: Termination
        We know that the loop terminates when low > high, so we first wish to examine the last iteration before termination
        to show that low = high + 1 upon termination. 

        We consider two cases on the last iteration. 
        Case 1: A[mid] < key
            We set low = mid + 1, and since mid = low + (high - low) / 2, low = (low + high) / 2 + 1
            If low == high, mid = low and so low = low + 1 = high + 1, as desired. 
            Otherwise, if low < high then mid = low and low = low + 1 == high which means that this is not the last iteration and 
            is an invalid subcase. 
            Thus, in all valid subcases of this case for the last iteration, low = high + 1, as desired. 
        Case 2: A[mid] >= key
            We set high = mid - 1, and since mid = low + (high - low) / 2, high = (low + high) / 2 - 1
            If low == high, mid = low and so high = low - 1 which implies that low = high + 1, as desired. 
            Otherwise, if high > low, then we will need at last one more iteration to make low > high which means this is not the 
            last iteration and is thus an invalid subcase. 
            Thus, in all valid subcases of this case for the last iteration, low = high + 1, as desired. 
        Therefore, low = high + 1 upon the last iteration.

        Now we show that the program is correct upon termination with this knowledge:   
            From our invariant, we know that for all i where 0 <= i <= high, A[i] < key and that for all i where low <= i <= n - 1, 
            A[i] >= key or low > n - 1. 

            If we substitute low = high + 1, we get that for all i where 0 <= i <= high, A[i] < key and that
            for all i where high + 1 <= i <= n - 1, A[i] >= key or high + 1 > n - 1. 

            This implies that for all i where low <= i <= n-1, A[i] >= key or low > n - 1. 

            Thus, low is either the smallest index where A[low] >= key or n if no such index exists, as desired. 
    
    Step 4: Postcondition
        Let Q ≡ (0 <= low <= n) and (forall i, 0 <= i < low -> A[i] < key) and (low < n -> A[low] >= key). 
        This postcondition follows immediately after the loop invariant at termination from the previous step, 
        thus proving the partial correct of the lowsearch function, as desired. 
    */
    while (low <= high) {
        int mid = low + (high - low) / 2; 
        if (A[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1; 
        }
    }
    return low; 
}