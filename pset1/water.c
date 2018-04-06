#include <stdio.h>
#include <cs50.h>

//water.c task - 1 minute of shower gives 12 bottles of water, no user input error-checking

int main(void)
{
    printf("Minutes: ");
    int minutes = get_int();
    printf("Bottles: %i\n", minutes*12);
}