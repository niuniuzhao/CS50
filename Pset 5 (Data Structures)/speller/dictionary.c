// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    // Word
    char word[LENGTH + 1];
    // Address of next word
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 30;

// Hash table - global pointer array containing 26 note pointers
node *table[N];

// Global variable for word count
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // 1. GET INDEX: run HASH FUNCTION to get hash value i.e. index of linked list
    int index = hash(word);
    // 2. create a cursor, set it to first item in linked list
    // 3. keep moving to next item until hitting NULL
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        // compare word in text to word in dictionary
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// HASH FUNCTION: Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    // ddb said it's best to use the WHOLE WORD so
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash = (hash + toupper(word[i])) % N;
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }

    // Read each word in the file
    char read_word[LENGTH + 1];
    while (fscanf(source, "%s", read_word) != EOF)
    {
        // 1. Allocate memory for a node and store address of node in n
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // 2. Copy the word from file to our node
        strcpy(n->word, read_word);
        n->next = NULL;

        // 3. Prepend this word (node) to linked list
        int index = hash(read_word);
        n->next = table[index];
        table[index] = n;

        // Update word count
        word_count++;
    }

    // Close the dictionary file
    fclose(source);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // 1. Set cursor to first element in a list
        node *cursor = table[i];
        while (cursor != NULL)
        {
            // 2. Set tmp to be equal to cursor (tmp FOLLOWS cursor)
            node *tmp = cursor;
            // 3. Move cursor to next element
            cursor = cursor->next;
            // 4. Free whatever is at tmp
            free(tmp);
        }
    }
    return true;
}
