#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;

    do
    {
        n = get_int("Height: ");
    }
    while ((n < 0) || (n > 23));

    for (int i = 0; i < n; i++)
    {
        //prints spaces
        for (int j = 0; j < (n - (i + 1)); j++)
        {
            printf(" ");
        }

        //prints #
        for (int j = 0; j < (i + 2); j++)
        {
            printf("#");
        }

        printf("\n");
    }
}