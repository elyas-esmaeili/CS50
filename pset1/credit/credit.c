#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt the user to enter the CARD NUMBER
    long long cardNumber;
    cardNumber = get_long_long("Number: ");

    //sum : sum of the cards digit's
    //cardId : To find card is for VISA or AMX or MASTERCARD
    //counter : This will be shown the number of card digits
    int sum, cardId, counter = 0;
    sum = cardId = counter = 0;

    //calculate the some of digits with Luhn’s algorithm
    while (cardNumber != 0)
    {
        if ((cardNumber < 100) && (cardNumber > 9))
        {
            cardId = cardNumber;
        }
        if (cardNumber == 4)
        {
            cardId = 4;
        }

        int rem = cardNumber % 10;
        cardNumber /= 10;
        if ((counter % 2) != 0)
        {
            rem *= 2;
            rem = (rem % 10) + (rem / 10);
        }
        sum += rem;
        counter++;
    }

    if ((sum % 10) == 0)
    {
        switch (cardId)
        {
            case 4:
                if (counter == 13 || counter == 16)
                {
                    printf("VISA\n");
                }
                else
                {
                    printf("INVALID\n");
                }
                break;

            case 34:
            case 37:
                if (counter == 15)
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
                break;

            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                if (counter == 16)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
                break;

            default:
                printf("INVALID\n");
                break;
        }
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}