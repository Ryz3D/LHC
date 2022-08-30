#include "lhc.h"

int main()
{
    int a = 0;
    int b = 1;
    int c = 0;

loop:
    c = a;  // buffer a
    a = b;  // shift b left
    b += c; // add into b

    c = b + '0';
    putchar(c);

    if (b < 8)
    {
        putchar(' ');
        putchar('/');
        putchar(' ');
        goto loop;
    }

    return 0;
}
