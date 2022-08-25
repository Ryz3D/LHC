#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// DOES NOT RUN:
//  - if statements

int ia = 15;
int a;
int b = 5;
int i = -1;

int main()
{
    a = ia;

div:
    i++;
    a -= b;
    if (a < 0)
        goto end;
    goto div;
end:
    i += '0';
    putchar(i);

    return 0;
}
