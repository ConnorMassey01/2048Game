//Connor Massey - April 25, 2021

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_HIGHSCORES 3

//set to 1 for testing, displays additional data not related to the game
int verbose = 0;

//global variables
int available = 16;
int open_slots[16]; //displayed as {(rowcol),(rowcol),...}
int block[4][4];

void print_available(void)
{
    printf("\n\n-----AVAILABLE SPOTS (%d)-----\n", available);
    for(int i = 0; i < available; i++){
        if(open_slots[i] < 4){
            printf("%d", 0);
        }
        printf("%d ", open_slots[i]);
    }
    printf("\n");
    printf("-----USED SPOTS (%d)-----\n", 16 - available);
    for(int i = available; i < 16; i++){
        if(open_slots[i] < 4){
            printf("%d", 0);
        }
        printf("%d ", open_slots[i]);
    }
    printf("\n\n");
}

void print_game(void)
{
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            printf("%5d", block[row][col]);
        }
        printf("\n");
    }
}

void add_block(void)
{
    int index, pos, row, col;
    srand(time(NULL));
    //index to open_slot array;
    index = rand()%available;
    //get the position it will be in the game
    pos = open_slots[index];
    //move index to end of open_slots array and repair array
    for(int i = index; i < available-1; i++){
        open_slots[i] = open_slots[i + 1];
    }
    //last index is set to element removed and available size then reduced by 1
    if (verbose) printf("add %d to index %d\n", pos, available);
    open_slots[available-1] = pos;
    available--;
    //column is stored as ones digit, row stored as tens digit
    col = (pos/1)%10;
    row = (pos/10)%10;

    if(verbose) printf("Block added at %d,%d\n", row, col);
    //set the position to 2 on the game board
    block[row][col] = 2;
}

void set_up_game(void)
{
    //initilize all positions on board to zero and add each position to the open_slots array
    int count = 0; 
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            block[row][col] = 0;
            open_slots[count++] = col + row*10;
        }
    }
}

void update_slots(void)
{
    //check each position for zeros, if they are zero add to start of array, otherwie add to end of array
    int open = 0, used = available;
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            if(block[row][col] == 0){
                open_slots[open] = row*10 + col;
                open++;
            }
            else{
                open_slots[used] = row*10 + col;
                used++;
            }
        }
    }
}

int move_blocks(char input)
{
    int row, col;
    switch (input)
    {
    case ('w')://move up
        //iterate top down row by row, ignoring row 0 as they cant be moved up
        for(int row = 1; row < 4; row++){
            for(int col = 0; col < 4; col++){
                //check if row above is free and if so move block to that position
                while(block[row-1][col] == 0 && block[row][col] != 0){
                    block[row-1][col] = block[row][col];
                    block[row][col] = 0;
                    //update open_slots 
                    update_slots();
                    //move up a row and check again
                    if(row > 1) row--; 
                } 
                //colision with another block of same value
                while(block[row-1][col] == block[row][col] && block[row][col] != 0){
                    block[row-1][col] *= 2;
                    block[row][col] = 0;
                    //add 1 to available spots
                    available++;
                    //update open_slots
                    update_slots();
                    //move up a row and check again
                    if(row > 1) row--; 
                }               
            }
        } 
    break;

    case ('a'): //move left
        //iterate top down, left to right ingnoring col 0 as blocks there cant be moved left
        for(int row = 0; row < 4; row++){
            for(int col = 1; col < 4; col++){
                //check if column left is free and if so move block to that position
                while(block[row][col-1] == 0 && block[row][col] != 0){
                    block[row][col-1] = block[row][col];
                    block[row][col] = 0;
                    //update open_slots 
                    update_slots();
                    //move left a column and check again
                    if(col > 1) col--;
                }
                //colision with another block of same value
                while(block[row][col-1] == block[row][col] && block[row][col] != 0){
                    block[row][col-1] *= 2;
                    block[row][col] = 0;
                    //add 1 to available spots
                    available++;
                    //update open_slots
                    update_slots();
                    //move left a column and check again
                    if(col > 1) col--;
                }  
            }
        }
    break;

    case ('s')://move down
        //iterate bottom up, left to right ingoring row 3 as blocks there cant be moved down
        for(int row = 2; row >= 0; row--){
            for(int col = 0; col < 4; col++){
                //check if row below is free and if so move block to that position
                while(block[row+1][col] == 0 && block[row][col] != 0){
                    block[row+1][col] = block[row][col];
                    block[row][col] = 0;
                    //update open_slots 
                    update_slots();
                    //move down a row and check again
                    if(row < 2) row++;
                }
                //colision with another block of same value
                while(block[row+1][col] == block[row][col] && block[row][col] != 0){
                    block[row+1][col] *= 2;
                    block[row][col] = 0;
                    //add 1 to available spots
                    available++;
                    //update open_slots
                    update_slots();
                    //move down a row and check again
                    if(row < 2) row++;
                } 
            }
        } 
    break;

    case ('d')://move right
        //iterate top down, right to left, ignoring col 3 as blocks there cant be moved right
        for(int row = 0; row < 4; row++){
            for(int col = 2; col >= 0; col--){
                //check if column right is free and if so move block to that position
                while(block[row][col+1] == 0 && block[row][col] != 0){
                    block[row][col+1] = block[row][col];
                    block[row][col] = 0;
                    //update open_slots 
                    update_slots();
                    //move right a column and check again
                    if(col < 2) col++;
                }
                //colision with another block of same value
                while(block[row][col+1] == block[row][col] && block[row][col] != 0){
                    block[row][col+1] *= 2;
                    block[row][col] = 0;
                    //add 1 to available spots
                    available++;
                    //update open_slots
                    update_slots();
                    //move right a column and check again
                    if(col < 2) col++;
                } 
            }
        }
    break;
    
    default:
        printf("Invalid input givin\n");
        return 0;
    break;
    }
    //after moving the blocks if there are no spaces left the games is over so return 0
    if(available == 0){
        available = -1;
        return 0;
    }
    return 1;
}

void game(void)
{
    char input;
    add_block();
    if(verbose) print_available();
    print_game();
    //get w,a,s,d from player
    printf("Press w, a, s, or d followed by enter to move the blocks\n");
    while(available >= 0){
        scanf("\n%c", &input);
        if(move_blocks(input)){
            add_block();
            if(verbose) print_available();
            print_game();
        }
    }
}

int score(void)
{
    int sum = 0;
    //sum up all the blocks on the board
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            sum += block[row][col];
        }
    }
    return sum;
}

//only top 3 high scores are maintained
void updateHighscores(int score){
    FILE* file;
    int highscores[NUM_HIGHSCORES];
    int newHighscore = 0;
    //open highscores.txt to read the highscores
    file = fopen("highscores.txt", "r");
    if(file == NULL){
        printf("Error: could not open file: highscores.txt\n");
        return;
    }
    //see if the new score is any better than the current highscores
    for(int i = 0; i < NUM_HIGHSCORES; i++){
        fscanf(file, "%d", &highscores[i]);
        if(score > highscores[i]){
            //shift other highscores down
            for(int j = i + 1; j < NUM_HIGHSCORES; j++){
                highscores[j] = highscores[j - 1];
                
            }
            //replace the highscore
            highscores[i] = score;
            newHighscore = 1;
            break;
        }
    }
    fclose(file);
    //if there is a new highscore to add, open the file again to write to it and update the highscores
    if(newHighscore){
        //open the file to write the new highscores
        file = fopen("highscores.txt", "w");
        if(file == NULL){
            printf("Error: could not open file: highscores.txt\n");
            return;
        }
        for(int i = 0; i < NUM_HIGHSCORES; i++){
            fprintf(file, "%d\n", highscores[i]);
        }
    }
    fclose(file);
    return;
}

void displayHighscores(){
    int score;
    FILE* file = fopen("highscores.txt", "r");
    if(file == NULL){
        printf("Error: could not open file: highscores.txt\n");
        return;
    }
    printf("-----Highscores-----\n");
    for(int i = 0; i < NUM_HIGHSCORES; i++){
        fscanf(file, "%d", &score);
        printf("%d. %d\n", i + 1, score);
    }
}

int main(void)
{
    set_up_game();
    printf("Press enter to start\n");
    getc(stdin);
    //loop to handle playing the game
    game();
    printf("Game over\n");
    //calculate and display score
    printf("Score: %d\n", score());
    //update file storing the highscores
    updateHighscores(score());
    //display the highscores
    displayHighscores();
}
