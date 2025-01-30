#include <stdio.h>
#include <ctype.h>

int peekchar() {
    int c = getchar();
    return (c == EOF) ? EOF : ungetc(c, stdin);
}

int skipws() {
    int c = peekchar();
    if (c == EOF) return EOF;
    return isspace(c) ? (getchar(), skipws()) : 0;
}

int getIntHelper(int num) {
    int c = peekchar();
    if (!isdigit(c)) return num;
    return getIntHelper(num * 10 + (getchar() - '0'));
}

int getInt() {
    if (skipws() == EOF) return -1;
    if (!isdigit(peekchar())) return -1;
    return getIntHelper(0);
}

int sumInts() {
    int n = getInt();
    return (n == -1) ? 0 : (n + sumInts());
}

int main(void) {
    printf("%d\n", sumInts());
    return 0;
}