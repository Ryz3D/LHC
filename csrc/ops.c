#include "lhc.h"

int main()
{
    int a1 = 1 < 2;  // 1
    int a2 = 2 < 2;  // 0
    int a3 = 3 < 2;  // 0
    int b1 = 1 == 2; // 0
    int b2 = 2 == 2; // 1
    int b3 = 3 == 2; // 0
    int c1 = 1 > 2;  // 0
    int c2 = 2 > 2;  // 0
    int c3 = 3 > 2;  // 1
    int d1 = 1 <= 2; // 1
    int d2 = 2 <= 2; // 1
    int d3 = 3 <= 2; // 0
    int e1 = 1 >= 2; // 0
    int e2 = 2 >= 2; // 1
    int e3 = 3 >= 2; // 1

    putchar('0' + a1);
    putchar('0' + a2);
    putchar('0' + a3);
    putchar('0' + b1);
    putchar('0' + b2);
    putchar('0' + b3);
    putchar('0' + c1);
    putchar('0' + c2);
    putchar('0' + c3);
    putchar('0' + d1);
    putchar('0' + d2);
    putchar('0' + d3);
    putchar('0' + e1);
    putchar('0' + e2);
    putchar('0' + e3);

    return 0;
}
