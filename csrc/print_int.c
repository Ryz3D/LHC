#include "lhc.h"

int main()
{
    int i = 13;

    int c = 0;
    // log10
    for (int b = i; b > 0; b /= 10)
    {
        c++;
    }
    // iterate digits
    for (int x = c - 1; x >= 0; x--)
    {
        int j = i;
        // divide by 10^x
        for (int y = 0; y < x; y++)
        {
            j /= 10;
        }
        putchar('0' + j % 10);
    }

    return 0;
}

/*
int main()
{
    int i = 132;

    int a;
    int b;
    int c;
    int d;
    int e;
    int f;

    a = i;
    if (a < 0) { putchar('-'); a = 0 - i; }
    c = 0;
    for (b = a; b > 0; b /= 10) { c++; }
    for (d = c - 1; d >= 0; d--) { f = a; for (e = 0; e < d; e++) { f /= 10; } putchar('0' + f % 10); }

    return 0;
}
*/
