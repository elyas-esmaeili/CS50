//caesar's algorithm
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("You have to pass TWO arguments!\n");
        return 1;
    }

    //convert key to int
    int k = atoi(argv[1]);
    string plainText = get_string("plaintext: ");

    for (int i = 0, n = strlen(plainText); i < n; i++)
    {
        //check if i'th character is alpha
        if (isalpha(plainText[i]))
        {
            if (isupper(plainText[i]))
            {
                //convert character to it's alphabetical index
                //and at the finish convert in to it's ASCII code
                plainText[i] = (((plainText[i] - 'A') + k) % 26) + 'A';
            }
            else if (islower(plainText[i]))
            {
                plainText[i] = (((plainText[i] - 'a') + k) % 26) + 'a';
            }
        }
    }
    //plainText now converted to chiperText
    printf("ciphertext: %s\n", plainText);
}
