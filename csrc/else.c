#include "lhc.h"

int main()
{
    for (int i = -2; i <= 15; i++)
    {
        if (i <= i % 2)
        {
            print_int(i);
            printf(" is true\n");
        }
        else
        {
            print_int(i);
            printf(" is false\n");
        }
    }

    return 0;
}
