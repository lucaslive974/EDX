// Implements a dictionary's functionality

#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

// Loaded words
int dict_size = 0;

void freedict(node* n);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int code = hash(word);

    node* tmp = table[code];

    while(tmp) {
        if(strcasecmp(word, tmp->word) == 0) {
            return true;
        }
        if(tmp->next) {
            tmp = tmp->next;
        }
        else break;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int code = 0, cnt = 0;
    char c = word[cnt];
    int salt = 1;

    while(c != '\0') {
        code += pow((toupper(c) - 'a'), salt++);
        c = word[++cnt];
    }

    return code % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE* dict = fopen(dictionary, "r");

    if(!dict) {
        printf("Error loading dictionary on memory");
        return false;
    }

    char* buffer = malloc((sizeof(char) * LENGTH) + 1);

    while(fscanf(dict, "%s", buffer) != EOF) {
        node* n = malloc(sizeof(node));

        if(!n) {
            printf("Out of memory");
            unload();
            free(buffer);
            return false;
        }

        strcpy(n->word, buffer);
        n->next = NULL;

        int code = hash(buffer);
        if(!table[code]) {
            table[code] = n;
        } else {
            n->next = table[code];
            table[code] = n;
        }

        dict_size++;
    }

    free(buffer);
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for(int i = 0; i < N; i++) {
        freedict(table[i]);
    }

    return true;
}

void freedict(node* n) {
    if(n == NULL) return;

    freedict(n->next);

    free(n);
}
