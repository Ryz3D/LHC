#include "lhc.h"

int main()
{
    int a = 16;
    int b = 2;

    int i = -1;

div:
    i++;
    a -= b;
    if (a < 0)
    {
        goto end;
    }
    goto div;
end:
    i += '0';
    putchar(i);

    return 0;
}
