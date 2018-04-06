#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//Initials task, version - more comfortable - for week 2 assignment - Pset2
// A program that, given a person’s name, prints a person’s initials, per the below.

int main(void){
    string name = get_string();
    int length = strlen(name);
    bool space_before;
    char initial;
    
    if(name[0] != ' ' ){                // If the first character is not a space, so it must be a first initial
       initial = toupper(name[0]);
       printf("%c",initial);
    }
    
    for (int i = 0; i<length; i++){     // Iterate trough a characters of string
 
        if(name[i] == ' ' ){            // Skip spaces, but record it's existance
            space_before = true;
        }
        else{
            if(space_before){           // If there's a space before a character, it means it's a first character of word
                
              initial = toupper(name[i]);  
              printf("%c", initial); 
              space_before = false;
            }
        }
    }
    printf("\n");
 
}