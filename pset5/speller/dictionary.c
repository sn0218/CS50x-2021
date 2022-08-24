// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// initialize the word count in reading the dictionary
unsigned int wordctr = 0;

// Number of buckets in hash table
const unsigned int N = 10007;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash word to obtain hash code
    int hash_value = hash(word);

    // set the cursor pointing to the first item of the linked list
    node *cursor = table[hash_value];

    // traverse the linked list
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        else
        {
            // move the cursor to the next node
            cursor = cursor->next;
        }

    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // mapping function that maps a key to an index number in the range 0 to N-1
    // include all char of string to provide better distribution of data
    // cited from https://github.com/kpractice/Hashing-In-C/blob/master/hash_calculation.c

    unsigned long h = 0;

    // **convert the word to lowercase to ensure the hash function is case insensitive
    while (*word != '\0')
    {
        h = ((h << 5) + tolower(*word++));
    }

    return h % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *input = fopen(dictionary, "r");

    if (input == NULL)
    {
        return false;
    }

    // Read string one by one from dictionary
    char word[LENGTH + 1]; // create char array as word buffer to store string one by one

    // use fscanf to read formatted string from file pointer
    while (fscanf(input, "%s", word) != EOF)
    {
        node *w = malloc(sizeof(node));

        if (w == NULL)
        {
            return false;
        }

        // copy word into the word field of the node from word buffer
        strcpy(w->word, word);
        w->next = NULL;

        // hash the word to return the hash code to place the node in the linked list
        // hash value maps the element index of hash table
        int hash_value = hash(w->word);

        // insert node into Hash Table at the beginning of the linked list
        w->next = table[hash_value];
        table[hash_value] = w;

        // word count for each loop
        wordctr++;
    }

    fclose(input);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (wordctr > 0)
    {
        return wordctr;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *tmp = NULL;
    node *cursor = NULL;

    // Free linked list in hash table
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            tmp = table[i];
            cursor = table[i];

            while (cursor != NULL)
            {
                cursor = cursor->next;
                free(tmp);
                tmp = cursor;
            }

        }
        else
        {
            continue;
        }

    }
    free(cursor);

    // Check the pointer if it is free sucessfully
    for (int i = 0; i < N; i++)
    {
        if (cursor != NULL || tmp != NULL)
        {
            return false;
        }
    }

    return true;
}
