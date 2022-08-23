#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    int i = 'A';

inc:
    putchar(i);
    i++;
    goto inc;

    return 0;
}
