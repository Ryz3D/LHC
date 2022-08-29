#include "g_include.h"

int main()
{
    int x;
    int y;
    int d;

    for (x = 0; x <= 9; x++)
    {
        for (y = 2; y <= 5; y += 3)
        {
            d = x + '0';
            putchar(d);
            putchar(' ');
            d = y + '0';
            putchar(d);
            putchar(' ');
            if (x == 6)
            {
                putchar('X');
                putchar(' ');
                putchar('I');
                putchar('S');
                putchar('T');
                putchar(' ');
                putchar('6');
                putchar('!');
                putchar(' ');
            }
        }
    }

    return 0;
}
