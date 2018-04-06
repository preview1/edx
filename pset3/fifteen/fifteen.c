/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// variables to keep position of 0 field
int bya[0];
int bxa[0];

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    
    // save the initial position of 0
    bya[0] = DIM_MAX - (DIM_MAX-d) -1;
    bxa[0] = DIM_MAX - (DIM_MAX-d) -1;
    
    
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("\033[%dm\n",33);    //set color to yellow
    printf("WELCOME TO GAME OF FIFTEEN\n");
    printf("\033[%dm",0);   //remove all color attributes
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // TODO
int temp;                           //temporary value for swaping 1 and 2
int countdown = d * d -1;           //top-first-max value of the initial array
int p = DIM_MAX - (DIM_MAX-d) -1;   //position adjustment from maximum 9/9 array
    
        for(int i = 0; i< d; i++){      //fill initial array
            for(int j = 0; j< d; j++){
                board[i][j] = countdown;
                countdown--;
            }  
        }
        if(d % 2 == 0){          //if the board contains an odd number of tiles swap numbers 1 and 2
            temp = board[p][p-1];
            board[p][p-1] = board[p][p-2];
            board[p][p-2] = temp;
        }
        
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO    
    printf("\033[%d;%dm\n",32,1);   //set color to green and bold on
    printf("-");
    
    for (int i = 0; i < d; i++)         // First line
        printf("-----");
    
    printf("\n");
        for (int i = 0; i < d; i++)
        {
            printf("|");                // Tiles seperator
            for (int j = 0; j < d; j++)
            {
                if(board[i][j] == 0){
                    printf("    ");     // 0 value tile prints as empty
                    if (j < d - 1)
                        printf("|");}   // Tile seperator
                    
                else{
                    printf(" %2i ", board[i][j]);   //Print the values
                    if (j < d - 1)
                        printf("|");    // Tile seperator
                }
            }
            printf("|\n-");
            
            for (int i = 0; i < d; i++)
                printf("-----");        //End line
                
            printf("\n");
        }
    printf("\033[%dm",0);   //remove all color attributes
    
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
int by = bya[0];            //keep track of 0 tile
int bx = bxa[0];
    
    if(tile == board[by+1][bx]){    //checks whether the tile is upper/lower/left/right of the 0
        board[by][bx] = tile;      //then, swap the tiles and remember the 0 position
        board[by+1][bx] = 0;
        bya[0]++;
        return true;
    }
    else    if(tile == board[by-1][bx]){
        board[by][bx] = tile;
        board[by-1][bx] = 0;
        bya[0]--;
        return true;
    }
    else    if(tile == board[by][bx+1]){
        board[by][bx] = tile;
        board[by][bx+1] = 0;
        bxa[0]++;
        return true;
    }
    else    if(tile == board[by][bx-1]){
        board[by][bx] = tile;
        board[by][bx-1] = 0;
        bxa[0]--;
        return true;
    }

    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
int countup = 1;
    
    for(int i = 0; i< d; i++){          //Iterates trough the array and checks if the first value is 1
        for(int j = 0; j< d; j++){
            if(board[i][j] != countup)  // If not it's definatly not a win
                return false;
            else{           //Else itarates trough the rest of the values increasing the countup value
                countup++;              
                if(countup == d*d)      //If a positive machup count is the same as the number of 
                    return true;        //tiles(actual tiles-1) -> you WIN
            }
        }  
    }
    return true;
}
