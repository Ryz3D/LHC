#include <stdio.h>

int x;
int y;
int z1;
int z2;
int z3;
int iter;

int main()
{
    y = -200;

fory:
    x = -170;

forx:
    z1 = 0;
    z2 = 0;
    iter = 0;

foriter:
    z3 = z1;
    z1 = (z1 * z1 - z2 * z2) / 100 + x;
    z2 = (2 * z3 * z2) / 100 + y;

    iter++;
    if (iter < 250 && z1 < 200 && z2 < 200)
    {
        goto foriter;
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
    x += 1;
    if (x < 50)
        goto forx;

    putchar('\n');

    y += 2;
    if (y < 200)
        goto fory;

    return 0;
}
