/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
#include <stdio.h> 
#include <cs50.h>

#include "helpers.h"

bool binary_search (int start,int end,int value,int values[],int n);

const int MAX2 = 65536;
/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if(value>0)
        return(binary_search(0,n-1,value,values,n));        //Use binnary search to find a value in an array

    return false;    
}

//Uses parameters: search start point, end point, value, array to search in, number of values in array
bool binary_search (int start, int end, int value, int values[], int n ){ 
    
    while (n > 0){                                                  //Continue while there is at least one unchecked value in an array
        int m = (end + start) / 2;                                  //Set the mid point of the search field
        if(value == values[m])                                      //If value is found return true
            return true;
    
        else if(value < values[m]){                                 //If midpoint is higher than the value, search in the left
            end = m;                                                //Set the end point to the middle
            n = end - start;                                        //Count the number of values left to check
            end--;                                                  //Shift the end point to the left of the midpoint, because midpoint is already checked.
            return(binary_search (start, end, value, values, n));
        }
        else if(value > values[m]){                                 //If midpoint is lower than the value, search in the right
            start = m;                                              //Set the start point to the middle
            n = end - start;                                        //Count the number of values left to check
            start++;                                                //Shift the start point to the right of the midpoint, because midpoint is already checked.
            return(binary_search (start, end, value, values, n));
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int C[MAX2];                                        //Define count array, all values by default gets 0.
    int indexC;                                         //define the index of count array
    
    for(int i = 0; i < n; i++){                         //Iterate trough the given values
        indexC = values[i];                             //Set the index of the count array to the value of the given values array
        C[indexC] = C[indexC] + 1;                      //Increase the specified count array value by one
    }
    
    int i = 0;                                          
    for(int j = 0; j < MAX2; j++){                      //Iterate trough count array
        if( C[j] > 0){                                  //If the value is non zero(got increased by previuos method)
            for(int m = 0; m<C[j]; m++){                //Iterate trough the value of specific count array index
            values[i]= j;                               //Set the next higher value of count array to values array
            i++;                                        //Increase counter of values set
                if(i == n)                              //End if the values set is equal to the number of values that exists in the search array from the start
                    return;
            }
        }
    }
    return;
}
