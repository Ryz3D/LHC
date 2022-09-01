#include "lhc.h"

int main()
{
    int a = 3;
    int b = 2;

    int c = 0;
    b--;

    while (b >= 0)
    {
        c += a;
        b--;
    }
    c += '0';
    putchar(c);

    return 0;
}
