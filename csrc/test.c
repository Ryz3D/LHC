#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// DOES NOT RUN:
//  - modulo
//  - division

int main()
{
    int a = 0;
    int b = 1;
    int c = 0;

loop:
    c = a;  // buffer a
    a = b;  // shift b left
    b += c; // add into b

    c = b / 100;
    c %= 10;
    c += '0';
    putchar(c);

    c = b / 10;
    c %= 10;
    c += '0';
    putchar(c);

    c = b;
    c %= 10;
    c += '0';
    putchar(c);
    putchar(' ');

    // if (b < 80)
    goto loop;

    return 0;
}
