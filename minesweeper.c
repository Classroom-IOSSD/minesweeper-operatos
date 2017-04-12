#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio.h"
#define GAME_TABLE_LENGTH 10

// background color
#define BG_COLOR_KNRM  "\x1B[0m"
#define BG_COLOR_BRED  "\x1B[41m"
#define BG_COLOR_BGRN  "\x1B[42m"
#define BG_COLOR_BYEL  "\x1B[43m"
#define BG_COLOR_BBLU  "\x1B[44m"
#define BG_COLOR_BMAG  "\x1B[45m"
#define BG_COLOR_BCYN  "\x1B[46m"
#define BG_COLOR_BWHT  "\x1B[47m"
// text color
#define TXT_COLOR_KRED  "\x1B[31m"
#define TXT_COLOR_KGRN  "\x1B[32m"
#define TXT_COLOR_KYEL  "\x1B[33m"
#define TXT_COLOR_KBLU  "\x1B[34m"
#define TXT_COLOR_KMAG  "\x1B[35m"
#define TXT_COLOR_KCYN  "\x1B[36m"
#define TXT_COLOR_KWHT  "\x1B[37m"


// global variables
// game table
unsigned char arrGameboard[GAME_TABLE_LENGTH][GAME_TABLE_LENGTH];
// location of cursor
int cursorPos_x=0, cursorPos_y=0;
// flag: input mode = 0, flag mode = 1, check mode = 2
int game_mode=0;

/*This is a recursive function which uncovers blank cells while they are adjacent*/
int uncover_blank_cell(int row, int col) {
    int value, rows[8], columns[8], i;

    if(arrGameboard[row][col] != 0)
        return 0; // error

    arrGameboard[row][col] += 10; // uncover current cell

    // Get position of adjacent cells of current cell
    rows[0] = row - 1;
    columns[0] = col + 1;
    rows[1] = row;
    columns[1] = col + 1;
    rows[2] = row + 1;
    columns[2] = col + 1;
    rows[3] = row - 1;
    columns[3] = col;
    rows[4] = row + 1;
    columns[4] = col;
    rows[5] = row - 1;
    columns[5] = col - 1;
    rows[6] = row;
    columns[6] = col - 1;
    rows[7] = row + 1;
    columns[7] = col - 1;

    for(i = 0; i < 8; i++) {
        value = arrGameboard[rows[i]][columns[i]];

        if( (rows[i] >= 0 && rows[i] < GAME_TABLE_LENGTH) && (columns[i] >= 0 && columns[i] < GAME_TABLE_LENGTH) ) {	// to prevent negative index and out of bounds
            if(value > 0 && value <= 8)
                arrGameboard[rows[i]][columns[i]] += 10;										// it is a cell with 1-8 number so we need to uncover
            else if(value == 0)
                uncover_blank_cell(rows[i], columns[i]);
        }

    }

    return 1; // success!
}

void PrintGameBoard() {
    // clear screen
    system("clear");

    int i, j, value;
    for(i = 0; i < GAME_TABLE_LENGTH; i++) {
        for(j = 0; j < GAME_TABLE_LENGTH; j++) {
            if(cursorPos_x == j && cursorPos_y == i) {
                if(game_mode == 1) {
                    printf("|%sF%s",BG_COLOR_BMAG,BG_COLOR_KNRM);
                    continue;
                } else if(game_mode == 2) {
                    printf("|%sC%s",BG_COLOR_BMAG,BG_COLOR_KNRM);
                    continue;
                }

            }
            value = arrGameboard[i][j];

            if((value >= 0 && value <= 8) || value == 0 || value == 99)
                printf("|X");
            else if(value == 10) // clean area
                printf("|%s%d%s",TXT_COLOR_KCYN, value - 10,BG_COLOR_KNRM);
            else if(value == 11) // the number of near mine is 1
                printf("|%s%d%s",TXT_COLOR_KYEL, value - 10,BG_COLOR_KNRM);
            else if(value > 11 && value <= 18) // the number of near mine is greater than 1
                printf("|%s%d%s",TXT_COLOR_KRED, value - 10,BG_COLOR_KNRM);
            else if((value >= 20 && value <= 28) || value == 100)
                printf("|%sF%s",TXT_COLOR_KGRN,BG_COLOR_KNRM);
            else
                printf("ERROR"); // test purposes

        }
        printf("|\n");
    }

    printf("cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n",TXT_COLOR_KCYN,BG_COLOR_KNRM,TXT_COLOR_KGRN,BG_COLOR_KNRM);
    if(game_mode == 0) {
        printf("f (put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
    } else if(game_mode == 1) {
        printf("Enter (select to put/remove Flag in cell), q (Exit selection): ");
    } else if(game_mode == 2) {
        printf("Enter (select to check cell), q (Exit selection): ");
    }


}


int main(int argc, char *argv[]) {

    char ch;
    int nMines; // the number of the remaining mines
    int i,j,pos_X,pos_Y,value, rows[8], columns[8];
	//col == pos_X
new_game:
    // the number of mines
    nMines = 10;
    if(argc == 2) {
        nMines = atoi(argv[1]);
    }
    srand (time(NULL));						// random seed
    // setting cursor
    cursorPos_x = 0;
    cursorPos_y = 0;
    // set all cells to 0
    for(i = 0; i < 10; i++)
        for(j = 0; j < 10; j++)
            arrGameboard[i][j] = 0;

    for(i = 0; i < nMines; i++) {
        /* initialize random seed: */

        pos_Y = rand() % 10;					// it generates a integer in the range 0 to 9
        pos_X = rand() % 10;

        // put mines
        if(arrGameboard[pos_Y][pos_X] != 99) {
            arrGameboard[pos_Y][pos_X] = 99;

            // Get position of adjacent cells of current cell
            rows[0] = pos_Y - 1;
            columns[0] = pos_X + 1;
            rows[1] = pos_Y;
            columns[1] = pos_X + 1;
            rows[2] = pos_Y + 1;
            columns[2] = pos_X + 1;
            rows[3] = pos_Y - 1;
            columns[3] = pos_X;
            rows[4] = pos_Y + 1;
            columns[4] = pos_X;
            rows[5] = pos_Y - 1;
            columns[5] = pos_X - 1;
            rows[6] = pos_Y;
            columns[6] = pos_X - 1;
            rows[7] = pos_Y + 1;
            columns[7] = pos_X - 1;

            for(j = 0; j < 8; j++) {
                value = arrGameboard[rows[j]][columns[j]];
                if( (rows[j] >= 0 && rows[j] < GAME_TABLE_LENGTH) && (columns[j] >= 0 && columns[j] < GAME_TABLE_LENGTH) ) {	// to prevent negative index and out of bounds
                    if(value != 99)																// to prevent remove mines
                        arrGameboard[rows[j]][columns[j]] += 1;									// sums 1 to each adjacent cell
                }
            }

        } else {							// to make sure that there are the properly number of mines in table
            i--;
            continue;
        }
    }

    //
    while(nMines != 0) {			// when nMines becomes 0 you will win the game
        PrintGameBoard();

        ch = getch();
        // cursor direction
        char direction;
        switch (ch) {

        // flag
        case 'f':
        case 'F':


flag_mode:
            game_mode = 1;
            do {
                PrintGameBoard();
                direction = getch();
                // arrow direction
                if(direction == '8') {
                    // up
                    cursorPos_y = (GAME_TABLE_LENGTH + --cursorPos_y) % GAME_TABLE_LENGTH;
                } else if(direction == '2') {
                    // down
                    cursorPos_y = ++cursorPos_y % GAME_TABLE_LENGTH;
                } else if(direction == '4') {
                    cursorPos_x = (GAME_TABLE_LENGTH + --cursorPos_x) % GAME_TABLE_LENGTH;
                } else if(direction == '6') {
                    cursorPos_x = ++cursorPos_x % GAME_TABLE_LENGTH;
                } else if(direction == 'c' || direction == 'C') {
                    goto check_mode;
                } else if(direction == '\n') {
                    value = arrGameboard[cursorPos_y][cursorPos_x];

                    if (value == 99) {				// mine case
                        arrGameboard[cursorPos_y][cursorPos_x] += 1;
                        nMines -= 1;				// mine found
                    } else if(value >= 0 && value <= 8) {	// number of mines case (the next cell is a mine)
                        arrGameboard[cursorPos_y][cursorPos_x] += 20;
                    } else if(value >= 20 && value <= 28) {
                        arrGameboard[cursorPos_y][cursorPos_x] -= 20;
                    }

                    if(nMines == 0)
                        break;
                }
            } while (direction != 'q' && direction != 'Q');
            game_mode = 0;

            break;

        // check cell
        case 'c':
        case 'C':

check_mode:
            game_mode = 2;
            do {
                PrintGameBoard();
                direction = getch();

                // arrow direction
                if(direction == '8') {
                    // up
                    cursorPos_y = (GAME_TABLE_LENGTH + --cursorPos_y) % GAME_TABLE_LENGTH;
                } else if(direction == '2') {
                    // down
                    cursorPos_y = ++cursorPos_y % GAME_TABLE_LENGTH;
                } else if(direction == '4') {
                    cursorPos_x = (GAME_TABLE_LENGTH + --cursorPos_x) % GAME_TABLE_LENGTH;
                } else if(direction == '6') {
                    cursorPos_x = ++cursorPos_x % GAME_TABLE_LENGTH;
                } else if(direction == 'f' || direction == 'F') {
                    goto flag_mode;
                }

                else if(direction == '\n') {
                    value = arrGameboard[cursorPos_y][cursorPos_x];
                    if(value == 0)						// blank case
                        uncover_blank_cell(cursorPos_y, cursorPos_x);
                    else if(value == 99)				// mine case
                        goto end_of_game;
                    else if(value > 0 && value <= 8)	// number case (the next cell is a mine)
                        arrGameboard[cursorPos_y][cursorPos_x] += 10;

                    //	break;
                }
            } while (direction != 'q' && direction != 'Q');
            game_mode = 0;

            break;



        // jump to a new game
        case 'n':
        case 'N':
            goto new_game;
            break;

        // exit
        case 'q':
        case 'Q':
            goto end_of_game;

        default:
            break;
        }
    }

end_of_game:
    game_mode = 0;
    PrintGameBoard();
    printf("\nGAME OVER\n");

    if(nMines == 0)
        printf("you won!!!!\n");

    else
        printf("BOOM! you LOOSE!\n");

    do {
        printf("Are you sure to exit? (cursorPos_y or n)? ");
        ch = getch();
        putchar('\n');
        if(ch == 'cursorPos_y' || ch == 'Y') {
            break;
        } else if(ch == 'n' || ch == 'N') {
            goto new_game;
        }
        printf("Please answer cursorPos_y or n\n");
    } while(1);
    printf("See you next time!\n");

    fflush(stdin);

    return 0;
}
