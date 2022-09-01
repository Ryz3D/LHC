#include "lhc.h"

int main()
{
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
                printf("X IST 6! ");
            }
        }
    }

    return 0;
}
