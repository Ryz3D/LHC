#include "g_include.h"

int main()
{
    int z = 2;
    z--;
    putchar('0' + 5 + 1 - 3 + z + z);
    putchar(' ');

    for (int x = 0; x <= 9; x++)
    {
        for (int y = 2; y <= 5; y += 3)
        {
            putchar('0' + x);
            putchar(' ');
            putchar('0' + y);
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
