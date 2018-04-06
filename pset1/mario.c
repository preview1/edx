#include <stdio.h>
#include <cs50.h>

//Mario.c task, version - more comfortable. Ask user for height of the pyramid and draw two half pyramids with two spaces inbetween

int getInput();
void buildPyramid();

int main(void)
{
    buildPyramid(getInput());                               //Build two half-pyramids using input of the user
}

//Ask for input, ensure it's a non-negative integer no greater than 23
int getInput()
{
int n;

    do
    {
        printf ("Height: ");    
        n = get_int();
    }
    while(n < 0 || n > 23);
    
    return n;
}


//Build two half-pyramids using input of the user as height
void buildPyramid(int height)
{
   int rows, spaces, hashes;
   
    //Iterate through the rows of the pyramid   
    for (rows = 0; rows<height; rows++)
    {
        //Print spaces - less spaces every row
        for(spaces = height-rows-1; spaces > 0; spaces--)
         {
            printf (" ");
         }
            //Print hashes of the first pyramid - more hashes every row
            for(hashes = 0; hashes <rows+1; hashes++) 
            {
                printf ("#");
            
            }
                //Print two spaces in between the half-pyramids
                printf ("  ");
                
                //Print hashes of the second pyramid - more hashes every row
                for(hashes = 0; hashes <rows+1; hashes++) 
                {
                    printf ("#");
                
                }
        
        
        printf ("\n"); 
    }   

    
}
    
    