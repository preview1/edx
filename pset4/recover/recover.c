//Week 4, pset4, Recover. A program that recovers JPEGs from a forensic image

//Data in the 512 byte blocks inbetween the images is copied to new image as it is, the end jpegs are not checked

#include <stdio.h>
#include <stdlib.h>
#define BUZZ_SIZE 1024

int main(int argc, char *argv[]){
    
    char *infile = argv[1];         //Save input file name
    
    if(argc != 2){                                      //Check the number of arguments
        fprintf(stderr,"Usage: ./recover infile\n");
        return 1;
    }
    
    FILE *inptr = fopen(infile, "r");
    if(inptr == NULL){                                  //Check if the file was opened correctly
        fprintf(stderr,"Could not open %s.\n", infile);
        return 2;
    }
        
    char filename[15];                  //storage for file names
    int findfirst = 0;                  //checked is for the first jpeg found
    unsigned char buffer[512];          //buffer
    FILE *outptr;
    int i = 0;                          //jpeg number counter
    
    while(fread(&buffer, 512, 1, inptr) == 1){      //while the full block of 512 bytes was readed successfully
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0){  //if the block first bytes indicates the start of jpeg
            
            if(findfirst){
                fclose(outptr);         //Start closing the output files after the second rotation
                i++;
            }
            
            findfirst = 1;
            sprintf(filename, "%03i.jpg",i);        //Define new filename
            outptr = fopen(filename, "w");
            fwrite(&buffer, 512, 1, outptr);        //Write the first jpeg block

        }
        else if(findfirst){
            fwrite(&buffer, 512, 1, outptr);        //Write the rest of the jpeg's blocks
        }
    }
}