/**
 * Implements a dictionary's functionality.
 */
/**
 * Week 5, pset5, Speller. A program that spell-checks a file.
 * 
 */

#include <stdbool.h>

#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns true if word is in dictionary else false.
 */ 
bool check(const char *word)
{
    // Checks if the word is in a trie

    unsigned int level;                          // level of a trie, index of a word letter
	const int length = strlen(word);
	int index;                          // index position of a trie node/child
	node *next = root;                  // set a search position to the root

	for (level = 0; level < length; level++)        // iterate trough the letters of a word
	{
		index = get_index(word[level]);             // assign an index of a trie from a character
		if(index >= 0 && index <= 26){              // check if an index would not return an out of bound value
		    if (next->children[index] == NULL)     // if a value in a trie at the specified location does not exist - the word is not in a dictionary
			    return false; 

		    next = next->children[index];           // move to the next child
		}
		else
	        return false;
	}
	
    if (next != NULL && next->is_word)              // if a node exists and it is the end of a word - a word is in a dictionary
        return true;

	return false;

}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)       //reads dictionary file with all lower case letter words separated by new line and creates a trie
{

    unsigned int nodecount = 0;                 // counts created nodes
    unsigned int maxnodes;                      // number of nodes to allocate with calloc
    unsigned int file_size;                     // size of dictionary file
    word_count = 0;                             // word count in dictionary
    datasize = 0;                               // index of the array of addresses generated with calloc
    FILE *inptr = fopen(dictionary, "r");       // open dictionary file in read-only mode

    if(inptr == NULL) {                          // error check in dictionary file opening
        fclose(inptr); 
        return false;
    }    

// counts size of file in bytes
    fseek(inptr, 0, SEEK_END);
    file_size = ftell(inptr);
    fseek(inptr, 0, SEEK_SET);

// sets initial number of nodes to allocate with calloc
    if(file_size * NODES_MULTIPLIER < INITIAL_NODES)        
        maxnodes = INITIAL_NODES;
    
    else
        maxnodes = file_size * NODES_MULTIPLIER;
    

    node* nodes = calloc(maxnodes, sizeof(node));       // allocates a memmory to load a dictionary into
    if(nodes == NULL){                                  // error check in calloc
        fclose(inptr); 
        return false;                                  
    }

    root = getNode(nodes);                          // Initializes a node with it's children and default values.
    node *next = root;                              // set first node as root
    
    for (unsigned int c = fgetc(inptr); c != EOF; c = fgetc(inptr)){     // read character from file until end of file
        while (c != '\n'){                                      // while a valid character, until end of line
        
            if(c == '\''){                                      // convert letters to index positions of trie [a to z plus apostrophe] to  [0-26]  
                c = ALPHABET_SIZE-1 + LOWER_ASCII_OFFSET;       // set index to be the next after last alphabetic letter (z(25)) + offset
                                                                // 26th position + LOWER_ASCII_OFFSET
            }
            
            c -= LOWER_ASCII_OFFSET;
            
            if (c > 26){                       // error checking for dictionary file not to have unspecified characters only a-z plus apostrophe
                fclose(inptr); 
                free(root);
                return false;
            }
            
            if (!next->children[c]){                        // check if the next node exists
			    next->children[c] = getNode(nodes);         // if not - create it
                nodecount++;                                    
		    }
		    
		    next = next->children[c];                           // move to the next node
		    c = fgetc(inptr); 
           
        }          
        
        next->is_word = true;                                   // mark end of word
        word_count++;                                           // count words in dictionary
        next = root;                                            // set the search point back to the start of trie


//IF initaly allocated memmory is not enough for the dictionary to load - double it and read the file again from the beginning
        if((maxnodes - nodecount) < LENGTH+1){ 
            maxnodes *= 2;
            node *nodes_temp = realloc(nodes, maxnodes * sizeof(node));
            
            if(nodes_temp == NULL){                                  // error check in realloc
                free(nodes);
                fclose(inptr); 
                return false;                                  
            }
            nodes = nodes_temp;
            fseek(inptr, 0, SEEK_SET);
            datasize = 0;
            root = getNode(nodes);
            next = root;
            nodecount = 0;
            word_count = 0;
        }
    }
    
    fclose(inptr);                                              // close dictionary file
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // Returns a word count while loading a dictionary
    return word_count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    
 free(root);
 return true;
 
}


int get_index(const char c){     // convert letters to index positions of trie [a to z plus apostrophe] to  [0-26] 
 
 if(islower(c)){
     return c - LOWER_ASCII_OFFSET;         // c - 97; a=>0
 }
 if(c == '\''){
     return ALPHABET_SIZE-1;                // 27-1;  '\'' => 26
 }
  else{
     return c - UPPER_ASCII_OFFSET;         //c - 65; A=>0
 }
 return -1;
}

struct node *getNode(node* nodes){      // Initializes a node with it's children and default values.

	struct node *pNode = &nodes[datasize++];            //give the next address from callocated memmory

	if (pNode){
	    
		pNode->is_word = false;
		for ( int i = 0; i < ALPHABET_SIZE; i++)
			pNode->children[i] = NULL;
	}
	
	return pNode;
}
	    
	