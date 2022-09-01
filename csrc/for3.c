#include "lhc.h"

int main()
{
    int do_break = 0;

restart:
    for (int x = 0; x <= 9; x++)
    {
        for (int y = 2; y <= 5 + do_break; y++)
        {
            putchar('0' + x);
            putchar(' ');
            putchar('0' + y);
            putchar(' ');
            if (x == 6)
            {
                printf("X IST 6! ");
                if (do_break)
                {
                    goto end;
                }
            }
        }
    end:;
    }

    if (do_break == 0)
    {
        putchar('\n');
        putchar('\n');

        do_break = 1;
        goto restart;
    }

    return 0;
}
