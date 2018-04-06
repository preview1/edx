/**
 * Copies a BMP piece by piece, just because.
 */
    
//Week 4, pset4, Resize, more comfortable. A program that resizes BMPs. A quality resize of pixels by a fraction of a number is not supported (2.5,0.7)

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
      if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize infile outfile\n");
        return 1;
    }

    // remember filenames
    char *fstring = argv[1];
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }
    
    // Sets resize multiplier
    float resizer = atof(fstring);

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bforg;                             //BITMAPFILEHEADER of the original file
    BITMAPFILEHEADER bf;                                //BITMAPFILEHEADER of the new resized file
    
    fread(&bforg, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    bf = bforg;                                         //Create new header as dublicate of original, then some fields will be modified
    
    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER biorg;                             
    BITMAPINFOHEADER bi;
    fread(&biorg, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi = biorg;
    
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bforg.bfType != 0x4d42 || bforg.bfOffBits != 54 || biorg.biSize != 40 || 
        biorg.biBitCount != 24 || biorg.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    int padding = (4 - (biorg.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi.biWidth = (int)(bi.biWidth*resizer);                                 //calculate new width
    bi.biHeight = (int)(bi.biHeight*resizer);                               //calculate new hight
    
    int newpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;        //calculate new padding
    
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newpadding) * abs(bi.biHeight);    //calculate new bi.biSizeImage
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);       //calculate new bf.bfSize

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    if(resizer >=1)                                       //If the resize value is greater than 1, image size will be increased or the same, else - reduced
           
        for (int i = 0, biHeight = abs(biorg.biHeight); i < biHeight; i++)         // Iterate trough rows of pixels
            for(int n = 0; n < resizer; n++){                                         // Repeat the same row the number of times equal to resizer value
        
                // iterate over pixels in scanline
                for (int j = 0; j < biorg.biWidth; j++)
                {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    for(int m = 0; m < resizer; m++)                                   // Repeat same pixel writing for the number of times as resize value.
                        // write RGB triple to outfile
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    
                }

                // skip over padding, if any
                fseek(inptr, padding, SEEK_CUR);

                // then add it back (to demonstrate how)
                for (int k = 0; k < newpadding; k++)
                    fputc(0x00, outptr);
            
                if(n < (resizer - 1))                                                  //if repeat is not over, set pointer in the input file back by the size of one row
                    fseek(inptr, -(biorg.biWidth*3+padding), SEEK_CUR);
            }
        
    else{                           //Reduce the size of new image - NO SMART algorithm to keept the quallity of the resized image, loss of data is inevitable
                                    //If resizer is 0.5, every second row and every second pixel is lost.
        
        resizer = 1 / resizer;      //Change resizer to the number of times to resize the image.
        int skippix;                //Skip pixel counter
        int skiprow = 0;            //Skip row counter

        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(biorg.biHeight); i < biHeight; i++){

            skippix = 0;
            if(skiprow % (int)resizer == 0){            //iterate only trough the every resizer'th row
                
                // iterate over pixels in scanline
                for (int j = 0; j < biorg.biWidth; j++){
                
                    // temporary storage
                    RGBTRIPLE triple;
                
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                    if(skippix % (int)resizer == 0)   //iterate only trough the every resizer'th pixel
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); // write RGB triple to outfile
                
                    skippix++;
                }
            
                // skip over padding, if any
                fseek(inptr, padding, SEEK_CUR);

                // then add it back (to demonstrate how)
                for (int k = 0; k < newpadding; k++)
                    fputc(0x00, outptr);
            
            }
            
            if(skiprow % (int)resizer != 0)         //If no read+write is done, go forward in the file, skip the position in input file by the size of a row
                fseek(inptr, (biorg.biWidth*3+padding), SEEK_CUR);
            
            skiprow++;
        }
    }
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
