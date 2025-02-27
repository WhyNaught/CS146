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