#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Create different permutation
int permutation(char [], int, int, char *);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    string hash = argv[1];
    char createdPass[6];
    //This loop is use for create permutaion of password
    //i = 1 for one character password and so forth
    for (int i = 1; i < 6; i++)
    {
        if (permutation(createdPass, 0, i, hash) == 0)
        {
            break;
        }
    }
    return 0;
}

/*
createdPass : For guessed password
k : Is used for create different permutaion
n : Number of character in the password
hash = hash that send with command line argument
*/
int permutation(char createdPass[], int k, int n, char *hash)
{
    static int flag = 1;
    if (k == n)
    {
        char salt[3];
        strncpy(salt, hash, 2);

        //obtain creaded password's hash
        char *newHash = crypt(createdPass, salt);
        if (strcmp(newHash, hash) == 0)
        {
            printf("%s\n", createdPass);
            flag = 0;
        }
    }
    else
    {
        //Create diffrent permutation
        for (int i = 'A'; i  <= 'z' ; i++)
        {
            if (flag == 0)
            {
                return flag;
            }
            if (i == 'Z')
            {
                i = 'a';
            }
            createdPass[k] = i;
            permutation(createdPass, k + 1, n, hash);
        }
    }
    return flag;
}