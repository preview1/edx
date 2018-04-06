#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//Crack task for week 2 assignment - Pset2
//A program that cracks passwords hashed with C’s DES-based crypt function. Assumed password - four alphabetic characters.

int iterate(int k, char test[5], string hash);

int main(int argc, string argv[]){
 
    if (argc == 2){                     //Checks if argument number is correct.
 
        string hash = argv[1];          
        char test[5];

        for(int i = 1; i<5; i++){       //Iterate trough the number of characters
            test[i] = '\0';             //Set last character to specific symbol
            test[i-1] = '-';            //Change the previously last character to any but specific symbol
            iterate(0,test,hash);       //Iterate trough all possible combinations 'A' -> 'z'
        }
    }
    else{
        printf ("Usage: ./crack hash\n");
        return 1;
    }    
}    

                
int iterate(int k,char test[5], string hash){

int start = 65;                         //Start character number
int end = 123;                          //End character number
string result;
    
    for(int i=start; i < end; i++){     //Iterate trough ASCII symbols  
        if(isalpha(i)){                 //Skip non-alphabetic symbols between upper and lower letters in ASCII table
            test[k] = i;
            if(test[k+1]!='\0'){        //If the length of characters didn't reached i, iterate trough the next position
                iterate(k+1,test,hash);
            }
            else{
                result = crypt(test, "50");     //Hash test password
                if (strcmp(hash, result)==0){   //Compare with given hash
                    printf ("%s\n", test);
                    exit(0);                    //End program after password was found
                }
            }
        }
    }   
    return 0;
}