/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#define ALPHABET_SIZE 27                // number of letters a to z plus apostrophe -> [0-26]
#define LOWER_ASCII_OFFSET 97                 // offset to set letters plus apostrophe to numbers 97->0, 123->26
#define UPPER_ASCII_OFFSET 65                 // offset to set letters plus apostrophe to numbers 97->0, 123->26
#define INITIAL_NODES 500000            // first atempt to generate a right amount of memmory with calloc for dictionary
#define NODES_MULTIPLIER 0.33           // dictionary file's size in byte multiplier for new bigger initial memmory allocation with calloc

#include <stdbool.h>

typedef struct node{
    bool is_word;
    struct node *children[ALPHABET_SIZE];
}
node;

node *root;

unsigned int datasize;      // index of the array of addresses generated with calloc
unsigned int word_count;    // Number of words in dictionary

/**
 * Return the position index of a trie converted from letters - convert letters to index positions of trie [a to z plus apostrophe] to  [0-26].
 */
int get_index(const char c);


/**
 * Creates a new node, mallocs memmory, initializes vallues.
 */
struct node *getNode(node* nodes);

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

#endif // DICTIONARY_H
