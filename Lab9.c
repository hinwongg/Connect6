/* 
 * File:   main.c
 * Author: Hin
 *
 * Created on October 30, 2014, 6:36 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
//Declares static array for the Connect6 board.
char board [21][21];

//Declares functions to be used to set up the input and "winner check" for Connect6.
void printBoard(char board[21][21], int n); //Don't change
void blockedPart(char board [21][21], int n, int blockedRow, int blockedCol); //Don't change
void makeMove(char board [21][21], int n, int turn);
int winnerCheck(char board [21][21], int n); //Don't change
int findLongest(char board [21][21], int n, int row, int col, char pieceColor); //Don't change
void score(char board [21][21], int n, int *maximumRow, int *maximumCol, char computerColor, char opponentColor);
int drawCheck(char board [21][21], int n); //Don't change.

int main(void) {

    int n;
    //Prompts user to input dimensions of the board.
    printf("Enter board dimensions (n), n>=6 and n<=21: ");
    scanf("%d", &n);

    //Initializes array.
    int row, col;
    for (row = 0; row < n; row++) {
        for (col = 0; col < n; col++)
            board[row][col] = 'U';
    }
    printBoard(board, n);

    //Declares variables for the alternating turns and declares array for computer.
    char inFirstPlayer [10];
    int turn;
    const int computerBlack = 1;
    const int humanWhite = 2;
    const int computerWhite = 3;
    const int humanBlack = 4;

    //User inputs computer color.
    printf("Computer playing B or W?: ");
    scanf("%s", inFirstPlayer);

    if (inFirstPlayer [0] == 'B')
        turn = computerBlack;

    if (inFirstPlayer [0] == 'W')
        turn = humanBlack;

    //User inputs blocked part.
    int blockedRow, blockedCol;

    printf("Enter position (ROW COL) of blocked square; (-1 -1) to finish: ");
    scanf("%d %d", &blockedRow, &blockedCol);
    blockedPart(board, n, blockedRow, blockedCol);

    //White and Black Moves
    makeMove(board, n, turn);

    return (EXIT_SUCCESS);
}

void printBoard(char board[21][21], int n) {
    //Prints the current board state.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%c", board [i][j]);
        }
        printf("\n");
    }
}

void blockedPart(char board [21][21], int n, int blockedRow, int blockedCol) {
    // Exits loop when user inputs -1 -1.
    while (blockedRow != -1 || blockedCol != -1) {
        //Prints error if input is invalid.
        if (blockedRow >= n || blockedCol >= n || blockedRow >= 21 ||
                blockedRow < 0 || blockedCol >= 21 || blockedCol < 0) {
            printf("Out of range row or column");
            printf("\n");
        } else if (board [blockedRow][blockedCol] == 'R') {
            printf("Position already occupied, try again");
            printf("\n");
        } else {
            //Changes elements in board array to R for blocked cells.
            board[blockedRow][blockedCol] = 'R';
            printBoard(board, n);
        }
        printf("Enter position (ROW COL) of blocked square; (-1 -1) to finish: ");
        scanf("%d %d", &blockedRow, &blockedCol);
    }
}

void makeMove(char board [21][21], int n, int turn) {
    //Exits loop when user inputs -1 -1.
    int moveRow, moveCol, winnerBreak = 0, drawBreak = 0;
    const int computerBlack = 1;
    const int humanWhite = 2;
    const int computerWhite = 3;
    const int humanBlack = 4;

    do {
        //Enters this if it is computer black's turn.
        if (turn == computerBlack) {
            char computerColor = 'B';
            char opponentColor = 'W';
            score(board, n, &moveRow, &moveCol, computerColor, opponentColor);
            board[moveRow][moveCol] = 'B';
            printf("Computer moves B at %d %d \n", moveRow, moveCol);
            printBoard(board, n);
            winnerBreak = winnerCheck(board, n);
            drawBreak = drawCheck(board, n);
            turn = humanWhite;

            // Enters this if is is computer white's turn.
        } else if (turn == computerWhite) {
            char computerColor = 'W';
            char opponentColor = 'B';

            score(board, n, &moveRow, &moveCol, computerColor, opponentColor);
            board[moveRow][moveCol] = 'W';
            printf("Computer moves W at %d %d \n", moveRow, moveCol);
            printBoard(board, n);
            winnerBreak = winnerCheck(board, n);
            drawBreak = drawCheck(board, n);
            turn = humanBlack;

            // Enters this if it is human black's turn.
        } else if (turn == humanBlack) {
            printf("Enter Black Move (ROW COL): ");
            scanf("%d %d", &moveRow, &moveCol);
            if (moveRow >= n || moveCol >= n || moveRow >= 21 || moveRow < 0 ||
                    moveCol >= 21 || moveCol < 0) {
                printf("Out of range row or column");
                printf("\n");
            } else if (board [moveRow][moveCol] == 'B' ||
                    board [moveRow][moveCol] == 'W' || board [moveRow][moveCol] == 'R') {
                printf("That square is already occupied or blocked");
                printf("\n");
            } else {
                board[moveRow][moveCol] = 'B';
                printBoard(board, n);
                winnerBreak = winnerCheck(board, n);
                drawBreak = drawCheck(board, n);
                turn = computerWhite;
            }

            //Enters this if is is human white's turn.
        } else if (turn == humanWhite) {
            printf("Enter White Move (ROW COL): ");
            scanf("%d %d", &moveRow, &moveCol);
            if (moveRow >= n || moveCol >= n || moveRow >= 21 || moveRow < 0 ||
                    moveCol >= 21 || moveCol < 0) {
                printf("Out of range row or column");
                printf("\n");
            } else if (board [moveRow][moveCol] == 'B' ||
                    board [moveRow][moveCol] == 'W' || board [moveRow][moveCol] == 'R') {
                printf("That square is already occupied or blocked");
                printf("\n");
            } else {
                board[moveRow][moveCol] = 'W';
                printBoard(board, n);
                winnerBreak = winnerCheck(board, n);
                drawBreak = drawCheck(board, n);
                turn = computerBlack;
            }
        }
        //If winner break is 1 or draw break is 1, the game finishes.
        if (winnerBreak == 1 || drawBreak == 1)
            break;
    } while (moveRow != -1 || moveCol != -1);
}

int winnerCheck(char board [21][21], int n) {
    //Checks for the winner after each turn.
    int checkWinner;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board [i][j] == 'B') {
                checkWinner = findLongest(board, n, i, j, board [i][j]);
                if (checkWinner == 6) {
                    printf("\nBlack player wins. \n");
                    //This exits out of the loop.
                    i = n, j = n;
                    return 1;
                }
            } else if (board [i][j] == 'W') {
                checkWinner = findLongest(board, n, i, j, board [i][j]);
                if (checkWinner == 6) {
                    printf("\nWhite player wins. \n");
                    //This exits out of the loop.
                    i = n, j = n;
                    return 1;
                }
            }
        }
    }
    return 0;
}

int findLongest(char board [21][21], int n, int row, int col, char pieceColor) {
    //Finds  the longest horizontal, vertical or diagonal (positive or negative) 
    //sequence of black or white stones at a specific element.
    int horizontal = 1;
    int vertical = 1;
    int posDiagonal = 1;
    int negDiagonal = 1;

    //Increments the horizontal, vertical, and diagonal positions 
    //around square every time it identifies the same color of stone.
    for (int south = 1; south <= n; south++)
        if (board [row + south][col] != pieceColor)
            break;
        else
            vertical++;
    for (int north = 1; north <= n; north++)
        if (board [row - north][col] != pieceColor)
            break;
        else
            vertical++;
    for (int east = 1; east <= n; east++)
        if (board [row][col + east] != pieceColor)
            break;
        else
            horizontal++;
    for (int west = 1; west <= n; west++)
        if (board [row][col - west] != pieceColor)
            break;
        else
            horizontal++;
    for (int north = 1, east = 1; north <= n && east <= n; north++, east++)
        if (board [row - north][col + east] != pieceColor)
            break;
        else
            posDiagonal++;
    for (int south = 1, west = 1; south <= n && west <= n; south++, west++)
        if (board [row + south][col - west] != pieceColor)
            break;
        else
            posDiagonal++;
    for (int south = 1, east = 1; south <= n && east <= n; south++, east++)
        if (board [row + south][col + east] != pieceColor)
            break;
        else
            negDiagonal++;
    for (int north = 1, west = 1; north <= n && west <= n; north++, west++)
        if (board [row - north][col - west] != pieceColor)
            break;
        else
            negDiagonal++;

    //Returns the longest horizontal, vertical, or diagonal contiguous sequence.
    if (horizontal >= vertical && horizontal >= posDiagonal
            && horizontal >= negDiagonal)
        return horizontal;
    else if (vertical >= horizontal && vertical >= posDiagonal
            && vertical >= negDiagonal)
        return vertical;
    else if (posDiagonal >= horizontal && posDiagonal >= vertical
            && posDiagonal >= negDiagonal)
        return posDiagonal;
    else if (negDiagonal >= horizontal && negDiagonal >= vertical
            && negDiagonal >= posDiagonal)
        return negDiagonal;
    else
        return 0;
}

void score(char board [21][21], int n, int *maximumRow, int *maximumCol, char computerColor, char opponentColor) {
    //Checks for the score of each tile.
    int score, defensiveScore = 0, offensiveScore = 0;
    int maximumValue = 0;
    int longEnough1 = 0, longEnough2 = 0;
    char pieceColorBlack = 'B';
    char pieceColorWhite = 'W';
    char blankSpace = 'U';
    *maximumRow = 0;
    *maximumCol = 0;



    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (board[row][col] == 'U') {
                score = findLongest(board, n, row, col, pieceColorBlack) + findLongest(board, n, row, col, pieceColorWhite);
                defensiveScore = findLongest(board, n, row, col, opponentColor);
                offensiveScore = findLongest(board, n, row, col, computerColor);
                longEnough1 = findLongest(board, n, row, col, blankSpace);
                longEnough2 = findLongest(board, n, row, col, computerColor);
                if (longEnough1 + longEnough2 < 7) {
                    score = score - 5;
                }

                if (defensiveScore == 5) {
                    score = score + 5;
                } else if (defensiveScore == 6) {
                    score = score + 10;
                }
                if (offensiveScore == 5) {
                    score = score + 7;
                } else if (offensiveScore == 6) {
                    score = score + 15;
                }
                if (score > maximumValue) {
                    maximumValue = score;
                    *maximumRow = row;
                    *maximumCol = col;
                    //New part



                }
            }


        }
    }
}

int drawCheck(char board [21][21], int n) {
    //Checks for draws. Returns 1 if it is a draw and prints draw.
    int draw = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board [i][j] == 'U') {
                draw = 0;
                return 0;
            }
        }
    }
    if (draw == 1)
        printf("\nDraw!\n");
    return 1;
}
