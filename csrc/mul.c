#include "lhc.h"

int main()
{
    int a = 3;
    int b = 2;

    int c = 0;
    int i = b - 1;

div:
    c += a;
    i--;
    if (i < 0)
    {
        goto end;
    }
    goto div;
end:
    c += '0';
    putchar(c);

    return 0;
}
