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

#define INPUT_MODE 0
#define FLAG_MODE 1
#define CHECK_MODE 2
#define TRUE 1
#define FALSE 0

int cursorPos_X; // location of cursor
int cursorPos_Y;
unsigned char arrGameboard[GAME_TABLE_LENGTH][GAME_TABLE_LENGTH];// game table
int nMines; // number of remaining mine
int game_mode; // flag: input mode = 0, flag mode = 1, check mode = 2

int ShowGameEnd(int isWin); 
int uncover_blank_cell(int row, int col);
void PrintGameBoard();
void initGame(int minesNum);
int RunGame(void);


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
                arrGameboard[rows[i]][columns[i]] += 10;
		// it is a cell with 1-8 number so we need to uncover
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
            if(cursorPos_X == j && cursorPos_Y == i) {
                if(game_mode == FLAG_MODE) {
                    printf("|%sF%s",BG_COLOR_BMAG,BG_COLOR_KNRM);
                    continue;
                } else if(game_mode == CHECK_MODE) {
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
    if(game_mode == INPUT_MODE) {
        printf("f (put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
    } else if(game_mode == FLAG_MODE) {
        printf("Enter (select to put/remove Flag in cell), q (Exit selection): ");
    } else if(game_mode == CHECK_MODE) {
        printf("Enter (select to check cell), q (Exit selection): ");
    }

	
}

void initGame(int minesNum)
{
	int i,j,value, rows[8], columns[8];
	game_mode = INPUT_MODE; 
	nMines = minesNum; // the number of mines
	cursorPos_Y = 0;
	cursorPos_X = 0; // setting cursor
	for(i = 0; i < GAME_TABLE_LENGTH ; i++)
        	for(j = 0; j < GAME_TABLE_LENGTH ; j++)
          	  	arrGameboard[i][j] = 0; // initialize GameBoard -> set all cells to 0
	
    	srand (time(NULL));// random seed
	/* initialize random seed: */
	for(i = 0; i < nMines; i++) {
		int randNum_row= rand() % 10; // it generates a integer in the range 0 to 9
		int randNum_col = rand() % 10;

        // put mines
        	if(arrGameboard[randNum_row][randNum_col] != 99) {
            	    arrGameboard[randNum_row][randNum_col] = 99;

		    // Get position of adjacent cells of current cell
		    rows[0] = randNum_row - 1;
		    columns[0] = randNum_col + 1;
		    rows[1] = randNum_row;
		    columns[1] = randNum_col + 1;
		    rows[2] = randNum_row + 1;
		    columns[2] = randNum_col + 1;
		    rows[3] = randNum_row - 1;
		    columns[3] = randNum_col;
		    rows[4] = randNum_row + 1;
		    columns[4] = randNum_col;
		    rows[5] = randNum_row - 1;
		    columns[5] = randNum_col - 1;
		    rows[6] = randNum_row;
		    columns[6] = randNum_col - 1;
		    rows[7] = randNum_row + 1;
		    columns[7] = randNum_col - 1;

            for(j = 0; j < 8; j++) {
                value = arrGameboard[rows[j]][columns[j]];
                if( (0 <= rows[j] && rows[j] < GAME_TABLE_LENGTH) && 
			(0 <= columns[j] && columns[j] < GAME_TABLE_LENGTH) ) 
		{	// to prevent negative index and out of bounds
			
			if(value != 99)// to prevent remove mines
				arrGameboard[rows[j]][columns[j]] += 1;
				// sums 1 to each adjacent cell				
                }
            }

        } else {
            i--;
	    // to make sure that there are the properly number of mines in table
            continue;
        }
    }
}
int RunGame(void) 
{
 	char ch;
	char direction;
    	int value;
	int endFlag = TRUE;
	while(TRUE) {			
	        if(endFlag){
			
        		PrintGameBoard();
			ch = getch(); // cursor direction
		}
		else
			endFlag = !endFlag;
        	switch (ch) {
        	// flag
        	case 'f':
        	case 'F':
		    game_mode = FLAG_MODE;
	            do {
			PrintGameBoard();
       		        direction = getch();
               		// arrow direction
			if(direction == '8') {
 	                   // up
 	                   cursorPos_Y = (GAME_TABLE_LENGTH + --cursorPos_Y) % GAME_TABLE_LENGTH;
			}else if(direction == '2') {
 	                   // down
 	                   cursorPos_Y = ++cursorPos_Y % GAME_TABLE_LENGTH;
 	               	}else if(direction == '4') {
                    	   cursorPos_X = (GAME_TABLE_LENGTH + --cursorPos_X) % GAME_TABLE_LENGTH;
			}else if(direction == '6') {
                  	  cursorPos_X = ++cursorPos_X % GAME_TABLE_LENGTH;
	                } else if(direction == 'c' || direction == 'C') {
        	            ch = 'c';
			    endFlag= FALSE;
			    break;
        	        } else if(direction == '\n') {
                  	  	value = arrGameboard[cursorPos_Y][cursorPos_X];

                 		if (value == 99) {				// mine case
					arrGameboard[cursorPos_Y][cursorPos_X] += 1;
                        		nMines -= 1;				// mine found
             	       		}else if(value >= 0 && value <= 8) {
					// number of mines case (the next cell is a mine)
                        		arrGameboard[cursorPos_Y][cursorPos_X] += 20;
                   		}else if(value >= 20 && value <= 28) {
                        		arrGameboard[cursorPos_Y][cursorPos_X] -= 20;
                        	}
	                    	if(nMines == 0)
	                        	return ShowGameEnd(TRUE);
			}
            	    } while (direction != 'q' && direction != 'Q');
			game_mode = INPUT_MODE;
           		 break;       
            	    //continue;

        // check cell
        case 'c':
        case 'C':
            game_mode = CHECK_MODE;
            do {
                PrintGameBoard();
                direction = getch();

                // arrow direction
                if(direction == '8') {
                    // up
                    cursorPos_Y = (GAME_TABLE_LENGTH + --cursorPos_Y) % GAME_TABLE_LENGTH;
                } else if(direction == '2') {
                    // down
                    cursorPos_Y = ++cursorPos_Y % GAME_TABLE_LENGTH;
                } else if(direction == '4') {
                    cursorPos_X = (GAME_TABLE_LENGTH + --cursorPos_X) % GAME_TABLE_LENGTH;
                } else if(direction == '6') {
                    cursorPos_X = ++cursorPos_X % GAME_TABLE_LENGTH;
                } else if(direction == 'f' || direction == 'F') {
			ch = 'f';
			endFlag= FALSE;
			break;
                }

                else if(direction == '\n') {
                    value = arrGameboard[cursorPos_Y][cursorPos_X];
                    if(value == 0)						// blank case
                        uncover_blank_cell(cursorPos_Y, cursorPos_X);
                    else if(value == 99)				// mine case
                        return ShowGameEnd(FALSE);
                    else if(value > 0 && value <= 8)	// number case (the next cell is a mine)
                        arrGameboard[cursorPos_Y][cursorPos_X] += 10;

                    //	break;
                }
            } while (direction != 'q' && direction != 'Q');
            game_mode = INPUT_MODE;
            break;

		// new game
		case 'n':
		case 'N':
		    return FALSE;
		// exit
		case 'q':
		case 'Q':
		    return TRUE;
		default:
		    break;
        }
	
    	fflush(stdin);
    }
}

/*
*	function ShowGameEnd(int isWin)
*	return TRUE  -> continue game
*	return FALSE -> exit game
*/
int ShowGameEnd(int isWin)
{
	int ch;
	PrintGameBoard();
	printf("\nGAME OVER\n");
	if(isWin)
		printf("you won!!!!\n");
	else
      		printf("BOOM! you LOOSE!\n");
	
	do {
        	printf("Are you sure to exit? (y or n)? ");
		ch = getch();
	        putchar('\n');
       	 	if(ch == 'y' || ch == 'Y') {
			return FALSE;
        	} else if(ch == 'n' || ch == 'N') {
			return TRUE;
        }
        printf("Please answer y or n\n");
    } while(1);

}
int main(int argc, char *argv[]) {

    int minesNum = (argc==2)? atoi(argv[1]) : 10;
    int isQuit = FALSE;

    while(!isQuit){
    	initGame(minesNum);
    	isQuit =RunGame();
    }

    printf("See you next time!\n");
    return 0;
}
