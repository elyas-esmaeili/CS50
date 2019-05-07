// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Number of words in dictionary
int cnt_word = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *cursor = root;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        // Determine index of character in chidren[27] array
        int index;
        if (word[i] == '\'')
        {
            // Put '\'' at index 26
            index = word[i] - 13;
        }
        else if (word[i] >= 'a')
        {
            index = word[i] - 'a';
        }
        else
        {
            index = word[i] - 'A';
        }

        if (cursor->childern[index] == NULL)
        {
            return false;
        }

        cursor = cursor->childern[index];
    }

    if (cursor->is_word == true)
    {
        return true;
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

    root = calloc(1, sizeof(node));

    // word keeps words in dictionary
    char *word = malloc((LENGTH + 1) * sizeof(char));
    while (fscanf(indic, "%s", word) != EOF)
    {
        node *cursor = root;
        for (int i = 0, n = strlen(word); i < n; i++)
        {
            // Determine index of word's character
            int index = (word[i] == '\'') ? word[i] - 13 : word[i] - 'a';

            if (cursor->childern[index] == NULL)
            {
                cursor->childern[index] = calloc(1, sizeof(node));
                if (cursor->childern[index] == NULL)
                {
                    unload();
                    return false;
                }
            }
            cursor = cursor->childern[index];
        }

        cnt_word++;
        cursor->is_word = true;
    }

    fclose(indic);
    free(word);

    return true;
}

/*void count(node* rt,int *cnt_word)
{
    if (rt == NULL)
    {
        return;
    }
    if (rt->is_word == true)
    {
        (*cnt_word)++;
    }

    for (int i = 0; i < 27; i++)
    {
        count(rt->childern[i] , cnt_word);
    }

}
*/

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //int cnt_word = 0 ;
    //count(root, &cnt_word);

    return cnt_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free_trie(root);

    return true;
}

// Recursive function for free trie data structure
void free_trie(node *rt)
{
    if (rt == NULL)
    {
        return;
    }
    for (int i = 0; i < 27; i++)
    {
        free_trie(rt->childern[i]);
    }
    free(rt);
}
