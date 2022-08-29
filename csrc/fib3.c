#include "g_include.h"

// DOES NOT RUN:
//  - print_uint

int main()
{
    int a = 0;
    int b;
    int c = 0;

    for (b = 1; b < 200; b += c)
    {
        print_uint(b);
        putchar(' ');
        c = a; // buffer a
        a = b; // shift b left
    }

    return 0;
}
