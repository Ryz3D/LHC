#include "lhc.h"

// DOES NOT RUN:
//  - 32 bit integers

int main()
{
    for (int y = -200; y < 200; y += 2)
    {
        for (int x = -150; x < 50; x += 2)
        {
            int iter;
            int z1 = 0;
            int z2 = 0;

            for (iter = 0; iter < 250; iter++)
            {
                int z3 = z1;
                z1 = z1 * z1 - z2 * z2;
                z2 = 2 * z3 * z2;
                z1 = z1 / 100 + x;
                z2 = z2 / 100 + y;

                if (z1 > 200 || z2 > 200)
                {
                    goto iterbreak;
                }
            }
        iterbreak:

            if (iter < 4)
            {
                putchar(' ');
                goto outend;
            }
            if (iter < 6)
            {
                putchar('.');
                goto outend;
            }
            if (iter < 10)
            {
                putchar('-');
                goto outend;
            }
            if (iter < 250)
            {
                putchar('*');
                goto outend;
            }
            putchar('#');
        outend:;
        }

        putchar('\n');
    }

    return 0;
}
