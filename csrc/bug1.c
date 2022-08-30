#include "lhc.h"

int main()
{
    for (int i = 'A'; i <= 'Z' + 1 /* TODO: this bug here */; i++)
    {
        putchar(i);
    }

    return 0;
}
