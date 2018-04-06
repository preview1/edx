#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//Vigenère task for week 2 assignment - Pset2
//A program that encrypts messages using Vigenère’s cipher.


bool check_code(string input);

int main(int argc, string argv[]){
    
    if (argc == 2){                             //Checks if the argument number is correct.
        
        string code = argv[1]; 
     
        if (check_code(code)){                  //Checks if the cipher argument contains only letters
           
            printf("plaintext: ");
            
            string message = get_string();
            int m_length = strlen(message);
            int c_length = strlen(code);
            string ciphertext = message;
            int j = 0;                          //Cipher character counter
            
            for (int i = 0; i<m_length; i++){   //Iterate trough the message
            
                if (isalpha(message[i])){       //If a character is a letter
                    
                    int shifter;                
            
                    if(isupper(code[j% - c_length]))        //Sets shifter values to be equal of both upper and lower case letters of cipher and starting at value - 0
                        shifter = code[j% c_length]- 65;
                    else
                        shifter = code[j% c_length]- 97;
 
                    if (isupper(message[i])){               //When messages character is an upper letter
                    
                        if ( message[i] + shifter > 90)                     // If a new value of ciphertext would be bigger than the biggest posible uppercase letter, shift additionally by the number of uppercase letters 
                            ciphertext[i]= ciphertext[i] - 26 + shifter;    // that exist in ascii table, to prevent from printing non-alphabetic symbols in ciphertext
                        else
                            ciphertext[i]= ciphertext[i] + shifter;         //Else shift normally
                    }
                    if (islower(message[i])){                               //Same as with uppercase letters
                  
                        if ( message[i] + shifter > 122)
                             ciphertext[i]= ciphertext[i]- 26 + shifter;
                         else
                            ciphertext[i]= ciphertext[i] + shifter; 
                    }
                    j++;                                                    //Increase cipher characters counter only after a message character is a letter;
                }
            }
        
            printf ("ciphertext: %s\n", ciphertext); 
            return 0;
        }
        else{
            printf ("Usage: ./vigenere k\n");
            return 1;
        }
    
    
    }
    else{
        printf ("Usage: ./vigenere k\n");
        return 1;
    }
}

//Checks if the cipher argument contains only letters
bool check_code(string code){
    
    int length = strlen(code);

        for (int i = 0; i<length; i++){
        
             if (!isalpha(code[i]))
                  return false;
               
        }
        return true;
}