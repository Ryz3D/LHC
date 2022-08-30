#include "lhc.h"

// DOES NOT RUN:
//  - print_uint

int main()
{
    int a = 0;
    int b = 1;
    int c = 0;

loop:
    c = a;  // buffer a
    a = b;  // shift b left
    b += c; // add into b

    print_uint(b);

    if (b < 200)
    {
        putchar(' ');
        putchar('/');
        putchar(' ');
        goto loop;
    }

    return 0;
}
