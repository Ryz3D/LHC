#include "g_include.h"

int main()
{
    int i = 'A';

inc:
    putchar(i);
    i++;
    goto inc;

    return 0;
}
