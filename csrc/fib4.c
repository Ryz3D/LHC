#include "lhc.h"

int main()
{
    int a = 0;
    int c = 0;

    for (int b = 1; b <= 89; b += c)
    {
        print_int(b);
        printf(" / ");
        c = a; // buffer a
        a = b; // shift b left
    }

    return 0;
}
