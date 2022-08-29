#include "g_include.h"

int main()
{
    int i = 2;
    int b;

    for (i = 0; i < 10; i++)
    {
        b = i + '0';
        putchar(b);
        putchar(' ');
    }

    return 0;
}
