#include <stdio.h>

// DOES NOT RUN:
//  - if statements
//  - 32 bit integers

int main()
{
    int x = -150;
    int y = -30;
    int z1;
    int z2;
    int z3;
    int iter;
    int buf;

forx:
    z1 = 0;
    z2 = 0;
    iter = 0;

foriter:
    z3 = z1;
    z1 *= z1;
    buf = z2 * z2;
    z1 -= buf;
    z1 /= 100;
    z1 += x;
    z2 *= z3;
    z2 /= 50;
    z2 += y;

    iter++;
    if (iter < 250)
    {
        if (z1 < 200)
        {
            if (z2 < 200)
            {
                goto foriter;
            }
        }
    }

    if (iter < 4)
    {
        putchar(' ');
        goto outend;
    }
    if (iter < 6)
    {
        putchar('.');
        goto outend;
    }
    if (iter < 10)
    {
        putchar('-');
        goto outend;
    }
    if (iter < 250)
    {
        putchar('*');
        goto outend;
    }
    putchar('#');

outend:
    x += 2;
    if (x < 50)
        goto forx;

    return 0;
}
