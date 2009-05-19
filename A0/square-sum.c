#include <stdio.h>

int sum_square_int(int);

int y;
int z = 0;
float a;
float b = 3.0;

int sum_square_int(int n)
{
    int i=0, sum=0;

    for (i=1; i<=n; ++i)
    {
        sum += i*i;
    }

    return sum;
}

int main()
{
    int x=2;
    double c, d;

    printf("Summe der Quadratzahlen von 1 bis 4: %d\n", sum_square_int(4));

    printf("Verschiedene Variablen:\n");
    printf("&x = %p\n", (void *)&x);
    printf("&y = %p\n", (void *)&y);
    printf("&z = %p\n", (void *)&z);
    printf("&a = %p\n", (void *)&a);
    printf("&b = %p\n", (void *)&b);
    printf("&c = %p\n", (void *)&c);
    printf("&d = %p\n", (void *)&d);

    return 0;
}
