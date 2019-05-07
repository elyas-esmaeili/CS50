//Vigenere's algorithm
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

    string key = argv[1];
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("There is non-alphabetical character in the key!\n");
            return 1;
        }
    }

    string plainText = get_string("plaintext: ");
    for (int i = 0, j = 0, n = strlen(plainText); i < n; i++)
    {
        //check if i'th character is alphabetic
        if (isalpha(plainText[i]))
        {
            j %= strlen(key);
            //convert key's i's character to it's alphabetical index
            int k = isupper(key[j]) ? key[j] - 'A' : key[j] - 'a';

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
            j++;
        }
    }
    //plainText now converted to chiperText
    printf("ciphertext: %s\n", plainText);
}
