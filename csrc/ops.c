#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    int a1 = 1 < 2;  // 1
    int a2 = 2 < 2;  // 0
    int a3 = 3 < 2;  // 0
    int b1 = 1 == 2; // 0
    int b2 = 2 == 2; // 1
    int b3 = 3 == 2; // 0
    int c1 = 1 > 2;  // 0
    int c2 = 2 > 2;  // 0
    int c3 = 3 > 2;  // 1
    int d1 = 1 <= 2; // 1
    int d2 = 2 <= 2; // 1
    int d3 = 3 <= 2; // 0
    int e1 = 1 >= 2; // 0
    int e2 = 2 >= 2; // 1
    int e3 = 3 >= 2; // 1

    a1 += '0';
    a2 += '0';
    a3 += '0';
    b1 += '0';
    b2 += '0';
    b3 += '0';
    c1 += '0';
    c2 += '0';
    c3 += '0';
    d1 += '0';
    d2 += '0';
    d3 += '0';
    e1 += '0';
    e2 += '0';
    e3 += '0';

    putchar(a1);
    putchar(a2);
    putchar(a3);
    putchar(b1);
    putchar(b2);
    putchar(b3);
    putchar(c1);
    putchar(c2);
    putchar(c3);
    putchar(d1);
    putchar(d2);
    putchar(d3);
    putchar(e1);
    putchar(e2);
    putchar(e3);

    return 0;
}
