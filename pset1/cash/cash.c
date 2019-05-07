#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float f;
    do
    {
        f = get_float("Change owed: ");
    }
    while (f < 0.0);
    //Convert to Cent
    f *= 100;

    int amount = round(f);

    //Number of each coin
    int numQuarters = 0;
    int numDimes = 0;
    int numNickels = 0;
    int numPennies = 0;

    numQuarters = amount / 25;
    amount %= 25;

    numDimes = amount / 10;
    amount %= 10;

    numNickels = amount / 5;
    amount %= 5;

    numPennies = amount / 1;
    amount %= 1;

    //total coins
    int numCoins = numQuarters + numDimes + numNickels + numPennies;

    printf("%i\n", numCoins);
}