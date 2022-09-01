#include "lhc.h"

int main()
{
    int a = 0;
    int b;
    int c = 0;
    int d;

    for (b = 1; b <= 8; b += c)
    {
        d = b + '0';
        putchar(d);
        putchar(' ');
        putchar('/');
        putchar(' ');
        c = a; // buffer a
        a = b; // shift b left
    }

    return 0;
}
