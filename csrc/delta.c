#include "lhc.h"

int main()
{
    int a = 'D';
    int b = 'G';
    int delta = b - a;
    putchar('0' + delta);
    delta = delta < 4;
    putchar('0' + delta);
    return 0;
}
