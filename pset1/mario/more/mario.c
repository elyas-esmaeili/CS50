#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    //Prompt the user to enter possitive number
    do
    {
        n = get_int("Height: ");
    }
    while ((n < 0) || (n > 23));

    for (int i = 0; i < n; i++)
    {
        //Print out spaces
        for (int j = 0; j < (n - (i + 1)); j++)
        {
            printf(" ");
        }

        //Print out # for first half-pyramid
        for (int j = 0; j < (i + 1); j++)
        {
            printf("#");
        }

        //Create gap
        printf("  ");

        //Print out second half-pyramid
        for (int j = 0; j < (i + 1); j++)
        {
            printf("#");
        }

        printf("\n");
    }
}