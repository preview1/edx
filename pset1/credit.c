#include <stdio.h>
#include <cs50.h>

//Credit task for week 1 assignment - Pset1
//A program that determines whether a provided credit card number is valid according to Luhn’s algorithm


int count_length(long long n);
void validy_check(long long n);
int lohn_check(long long n, int length);
void check_company(int start_with, int length);


int main(void)
{
    printf ("Number: ");    
    validy_check(get_long_long());
    return 0;
}

//Checks if the cc_number is valid and to which company it belongs  
void validy_check(long long n)
{
    int starts_with = 0;                                //First two digit's of the cc_number
    int length = count_length(n);                       //Set variable to the length of the entered cc_number
    
    if (length >= 13 && length <= 16)                   //Initial check if cc number is obviously invalid - longer or shorter than possible accepted values.
    {
        starts_with = lohn_check(n,length);             //Successful lohn check returns two first digits of cc_number, else - 0
        if(starts_with != 0)
        {
            check_company(starts_with,length);      //Check if the cc_number belongs to one of the acceptable company's
        }
    }
    else 
    {
        printf ("INVALID\n");
    }     
}
 
 
 
//Counts the length of the users input, All zeros in front or any negative number doesn't spin the counter
int count_length(long long n)
{
   int counter = 0;
   
   while(n>0)
   {
        n = n/10;
        counter+=1;
   }
   return counter;          
}

//Check if cc_number is valid with lohn's algorithm. IF OK - returns first two digits of cc_number, else returns - 0
int lohn_check(long long n, int length){

    int sum1 = 0;
    int sum2 = 0;
    int TOTAL = 0;
    int digit;
    int digits_digit1;
    int digits_digit2;
    int digit_number = 0;
    int starts_with_inner = 0;   
    
    for (int i = 0; i< length; i++)         //Iterate trought the digits of the cc_number
    
    {
    digit = n % 10;                         // Get last digit  
    n = n /10;                              // Remove last digit    
    digit_number++;                         // Keep track of digit's position in cc_number
    
        if(digit_number % 2 == 0)           // Iterate trough every second digit
        {
            if (digit * 2 < 10)
            {
            sum1 = sum1 + (digit * 2);
            }
            else                            // If digit times two is a number of more than one digit sum up the separate digits
            {   
                digits_digit1 = digit * 2 % 10;
                digits_digit2 = digit * 2 / 10 % 10;
                sum1 = sum1 + digits_digit1 + digits_digit2;
            }
             
        }
        else                                // Normally sum up every other digit of the cc_number
        {
            sum2 = sum2 + digit;
        }
            
        if  (length-digit_number < 1)       // At the same time check, if it's already first two digits of cc_number - get them
        {
            starts_with_inner = starts_with_inner + (digit * 10);
        }
        else       
        if(length-digit_number < 2)
        {
            starts_with_inner = starts_with_inner + digit;
        }
    }
        
      
    TOTAL = sum1+sum2;
        if(TOTAL % 10 == 0)             // If the last digit of checksum is 0, card number is valid
            return starts_with_inner;   // return first two digits of the cc_number
        else 
            return 0;                   //Else return 0
}

//Check whether the cc_number belongs to one of the three accepted company's
void check_company(int starts_with, int length)
{
    
    if (length == 15 && (starts_with == 34 || starts_with == 37))
    { 
        printf ("AMEX\n");
    }
    else
    if (length == 16 && starts_with >= 51 && starts_with <= 55)
    { 
        printf ("MASTERCARD\n");
    }
    else
    if ((length == 13 || length == 16) && starts_with / 10 == 4)
    { 
        printf ("VISA\n");
    }
    else
    {
        printf ("INVALID\n");
    }
}
