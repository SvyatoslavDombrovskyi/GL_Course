// variant 5

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int LOWER = 5;
const int UPPER = 20;

int main(void)
{
    srand(time(0));
    double z1, z2, alfa;

    alfa = (rand() % (UPPER - LOWER + 1)) + LOWER;

    z1 = 1 - 1 / 4 * pow(sin(2 * alfa), 2) + cos(2 * alfa);
    z2 = pow(cos(alfa), 2) + pow(cos(alfa), 4);

    printf("\n alfa = %f\n", alfa);
    printf(" z1 = %f\n", z1);
    printf(" z2 = %f\n", z2);
    return 0;
}