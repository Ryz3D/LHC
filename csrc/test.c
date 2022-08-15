#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int i = 2;

int main()
{
    uint8_t imabyte = 2;
    i = 'A';

inc:
    putchar(i);
    i++;
    goto inc;

    return 0;
}
