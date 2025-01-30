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

int hexToDecimal(char c) {
    if ('0' <= c && c <= '9') {return c - '0';}
    if ('a' <= c && c <= 'f') {return c - 'a' + 10;}
    if ('A' <= c && c <= 'F') {return c - 'A' + 10;}
    return -1; 
}

int getIntHelper(int num) {
    int c = peekchar();
    if (!isxdigit(c) || c == EOF) return num;
    getchar(); 
    int decimal = hexToDecimal(c); 
    return getIntHelper(num * 16 + decimal);
}

int getInt() {
    if (skipws() == EOF) return -1;
    int c = peekchar(); 
    if (!isxdigit(peekchar())) return -1;
    return getIntHelper(0);
}

int sumInts() {
    int n = getInt();
    return (n == -1) ? 0 : (n + sumInts());
}

int main(void) {
    printf("%x\n", sumInts());
    return 0;
}