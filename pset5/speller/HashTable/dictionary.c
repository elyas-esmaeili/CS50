// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>

#include "dictionary.h"

// Number of words in dictionary
int cnt_word = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hashfunction(word);

    node *cursor = hashtable[index];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *indic = fopen(dictionary, "r");
    if (!indic)
    {
        return false;
    }

    // word keeps words in dictionary
    char *word = malloc((LENGTH + 1) * sizeof(char));
    while (fscanf(indic, "%s", word) != EOF)
    {
        node *new_node = calloc(1, sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        strcpy(new_node->word, word);
        int index = hashfunction(new_node->word);

        new_node->next = hashtable[index];
        hashtable[index] = new_node;

        cnt_word++;
    }

    fclose(indic);
    free(word);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return cnt_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *cursor = NULL;
    for (int i = 0, n = pow(27, 5); i < n; i++)
    {
        cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}

int hashfunction(const char *key)
{
    int ratio = pow(27, 5);
    int sum = 0;
    int index = 0;

    for (int i = 0; i < strlen(key); i++)
    {
        if (i == 5)
        {
            break;
        }

        if (key[i] == '\'')
        {
            index = 26;
        }
        else if (key[i] >= 'a')
        {
            index = key[i] - 'a';
        }
        else
        {
            index = key[i] - 'A';
        }

        ratio /= 27;
        sum += (ratio * index);
    }

    return sum;
}