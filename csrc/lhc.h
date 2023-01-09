// includes for gcc-compatibility

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void print_uint(int i)
{
    int c = 0;
    // log10
    for (int b = i; b > 0; b /= 10)
        c++;
    // iterate digits
    for (int x = c - 1; x >= 0; x--)
    {
        int j = i;
        // divide by 10^x
        for (int y = 0; y < x; y++)
            j /= 10;
        putchar('0' + j % 10);
    }
}

void print_int(int i)
{
    printf("%i", i);
}

void delay(int n)
{
    for (int i = 0; i < n; i++)
    {
        (void)0;
    }
}
