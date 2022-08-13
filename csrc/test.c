int i;

int main()
{
    i = 'A';
    p = "Welt seid mir gegruesst!";

inc:
    putchar(i);
    i++;
    goto inc;

    return 0;
}
