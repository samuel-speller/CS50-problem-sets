// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "dictionary.h"

//global variable to count the words in the dictionary
unsigned int counter;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table

//chosen this number as it is roughly the largest value we can get from our hash function
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    //hash the word to obtain its hash value
    int hash_value = hash(word);

    //traverse the hash table at this hash value searching for the word

    //set pointer to the head of the list at the hash location
    node *ptr = table[hash_value];

    //while loop to traverse list
    while (ptr != NULL)
    {
        //use strcasecmp to compare the words at each location in the list (if match = 0 then the strings are the same)
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
        }

        //if not a match then continue on to next node
        ptr = ptr->next;
    }

    //if there are no matches then return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO:

    // This hash function adds the ASCII values of all characters in the word together
    long sum = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }

    //use % N to make sure we dont go beyond the bounds of the hash table
    return sum % N;



}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //TODO

    //open dictionary file
    FILE *dict = fopen(dictionary, "r");

    //check that it has been opened and quit program after displaying error message if not
    if (dict == NULL)
    {
        printf("could not load dictionary file\n");
        return false;
    }

    //define a buffer array to store each word
    char buffer_word[LENGTH + 1];

    //set counter to 0
    counter = 0;

    //loop to iterate over the dictionary, scan each word and add it to memory
    while (fscanf(dict, "%s", buffer_word) != EOF)
    {
        //use malloc to allocate space for a new node
        node *n = malloc(sizeof(node));

        //check malloc found some memory and quit program if not
        if (n == NULL)
        {
            printf("failed to allocate memory for dictionary node\n");
            return 1;
        }

        //copy each word into a new node
        strcpy(n->word, buffer_word);

        //set the pointer of the new node to NULL
        n->next = NULL;

        //hash word to obtain hash value
        int hash_value = hash(buffer_word);

        //insert node into hash table at this location

        //point new node at head of the list at its hash location
        n->next = table[hash_value];

        //insert new node into the head of the list
        table[hash_value] = n;

        //add to counter to keep track of the number of words in the dictionary
        counter++;

    }

    //close file
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //TODO

    //return the counter used in the load function
    return (counter);

}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    //loop over all the linked lists in the hash table

    for (int i = 0; i < N; i++)
    {
        //set a pointer to the head of each linked list
        node *ptr = table[i];

        //free the nodes in the linked list using a tmporary pointer
        while (ptr != NULL)
        {
            //set temporary pointer to the head of the list as well
            node *tmp = ptr;

            //set pointer to the next node in the linked list
            ptr = ptr->next;

            //free first node
            free(tmp);
        }

        if (ptr == NULL && i == N - 1)
        {
            return true;
        }
    }

    return false;

}
