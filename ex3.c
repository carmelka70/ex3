#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

    char getPlayerToken(int playerNumber);

    int isColumnFull(char board[][COLS], int column);

    int isBoardFull(char board[][COLS], int cols);

    int isInBounds(int x, int lowerBound, int upperBound);

/* Return index of row where token will land, or -1 if column full */
    int getFreeRow(char board[][COLS], int rows, int cols, int column);

/* Place token in column (0-based). Return row index or -1 if illegal */
    int makeMove(char board[][COLS], int rows, int cols, int column, char token);

/* Check if playerNumber won. Return 1 if yes, 0 if not */
    int checkVictory(char board[][COLS], int rows, int cols, int playerNumber, int connectNumber);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
    int humanChoose(char board[][COLS], int rows, int cols);

/* Computer*/
int computerChoose(char board[][COLS], int rows, int cols, int playerNumber);

void runConnectFour(char board[][COLS], int rows, int cols, int p1Type, int p2Type);

    void initBoard(char board[][COLS], int rows, int cols);

    void printBoard(char board[][COLS], int rows, int cols);

    int getPlayerType(int playerNumber);


int main() {
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}







char getPlayerToken(int playerNumber)
{
    return (playerNumber == 1) ? TOKEN_P1 : TOKEN_P2;
}


int isInBounds(int x, int lowerBound, int upperBound)
{
    return (x >= lowerBound) && (x <= upperBound);
}


int isBoardFull(char board[][COLS], int cols)
{
    /* 
    * We can check only the upper row, since in connect four if a slot is full 
    * every slot below it is full too
    */

    for (int c = 0; c < cols; c++)
        if (board[0][c] == EMPTY)
            return 0;

    return 1;

}

int isColumnFull(char board[][COLS], int column)
{
    /* 
    * We can check only the upper row, since in connect four if a slot is full 
    * every slot below it is full too
    */

    return (board[0][column] != EMPTY);
}

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char board[][COLS], int rows, int cols, int column)
{
    if (isColumnFull(board, column)) return -1;

    /*
     * Loop over the column from above, and check for the first non-empty slot.
     * Return the one above it (the minimal free row).
     */

    for (int r = 0; r < rows; r++)
        if (board[r][column] != EMPTY)
            return r - 1;

    // Otherwise, every row is empty; return the lowest
    return rows;
}

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char board[][COLS], int rows, int cols, int column, char token)
{
    int freeRow = getFreeRow(board, rows, cols, column);

    if (freeRow == -1) // No free rows at this column
        return -1;

    board[freeRow][column] = token;
    return freeRow;
}

void initBoard(char board[][COLS], int rows, int cols)
{
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            board[r][c] = EMPTY;
}

int isPlayerSlot(char board[][COLS], int rows, int cols, int row, int column, int playerNumber)
{
    // Checked slot is out of bounds.
    if (!isInBounds(row, 0, rows-1) || !isInBounds(column, 0 ,cols-1)) return 0;

    return (board[row][column] == getPlayerToken(playerNumber));
}

/* Check if playerNumber won. Return 1 if yes, 0 if not */
int checkSequence(char board[][COLS], int rows, int cols, int playerNumber, int connectNumber)
{
    // Check for horizontal
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols - CONNECT_N; c++)
        {
            // if all slots are captured, return 1. Else, continue.
            int totalCaptured = 0;

            for (int i = 0; i < connectNumber; i++)
                totalCaptured += isPlayerSlot(board, rows, cols, r, c+i ,playerNumber);

            if (totalCaptured == connectNumber) return 1;
        }

    // Check for vertical
    for (int r = 0; r < rows - CONNECT_N; r++)
        for (int c = 0; c < cols; c++)
        {
            // if all slots are captured, return 1. Else, continue.
            int totalCaptured = 0;

            for (int i = 0; i < connectNumber; i++)
                totalCaptured += isPlayerSlot(board, rows, cols, r+i, c ,playerNumber);

            if (totalCaptured == connectNumber) return 1;
        }


    // Check for up diagonal
    for (int r = CONNECT_N - 1; r < rows; r++)
        for (int c = 0; c < cols - CONNECT_N; c++)
        {
            // if all slots are captured, return 1. Else, continue.
            int totalCaptured = 0;

            for (int i = 0; i < connectNumber; i++)
                totalCaptured += isPlayerSlot(board, rows, cols, r-i, c+i ,playerNumber);

            if (totalCaptured == connectNumber) return 1;
        }

    // Check for down diagonal
    for (int r = 0; r < rows - CONNECT_N; r++)
        for (int c = 0; c < cols - CONNECT_N; c++)
        {
            // if all slots are captured, return 1. Else, continue.
            int totalCaptured = 0;

            for (int i = 0; i < connectNumber; i++)
                totalCaptured += isPlayerSlot(board, rows, cols, r+i, c+i ,playerNumber);

            if (totalCaptured == connectNumber) return 1;
        }

    // No connections found
    return 0;
}

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char board[][COLS], int rows, int cols)
{
    int chosenColumn = -1;
    while (1)
    {
        printf("Enter column (1-%d):", cols);
        if (scanf("%d", &chosenColumn) == 1)
        {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        
        if(!isInBounds(chosenColumn, 1, cols))
        {
            printf("Invalid column. Choose between 1 and %d.\n", cols);
            continue;
        }

        if (isColumnFull(board, chosenColumn-1))
        {
            printf("Column %d is full. Choose another column.\n", chosenColumn);
        }

        //The column is valid:
        return chosenColumn;
    }
}


int checkCreateSequence(char board[][COLS], int rows, int cols, int playerNumber, int sequenceLength)
{
    for (int c = 0; c < cols; c++)
    {

        /*
         * For each column, we will try to add a token, and check using our checkSequence function
         * After, we will clear that slot back, and if we got a good enough sequence, we will chose that column.
         */
        int freeRow = makeMove(board, rows, cols, c, getPlayerToken(playerNumber));

        // The column is full
        if (freeRow == -1) continue;

        if (checkVictory(board, rows, cols, playerNumber, CONNECT_N)) // A winning move
        {
            board[freeRow][c] = EMPTY;
            return c;
        }
        else 
            board[freeRow][c] = EMPTY;
    }
}

int computerChoose(char board[][COLS], int rows, int cols, int playerNumber)
{
    /*
     * Priority order
     * 1. Winning move - if it is possible to win on the next move - choose the column that produces the win.
     * 2. Blocking the opponent - if the opponent can win on their next move choose the column that prevents this.
     * 3. Creating a sequence of three - if it is possible to create a sequence of three tokens do so.
     * 4. Blocking the opponent’s sequence of three.
     * 5. Choosing according to the arbitrary ordering rule.
     */


    // 1. Winning move - if it is possible to win on the next move - choose the column that produces the win.
    for (int c = 0; c < cols; c++)
    {

        /*
         * For each column, we will try to add a token, and check using our checkVictory function
         * if after that move we won. we will clear that slot back, and if we won, we will chose that column.
         */
        int freeRow = makeMove(board, rows, cols, c, getPlayerToken(playerNumber));

        // The column is full
        if (freeRow == -1) continue;

        if (checkVictory(board, rows, cols, playerNumber, CONNECT_N)) // A winning move
        {
            board[freeRow][c] = EMPTY;
            return c;
        }
        else 
            board[freeRow][c] = EMPTY;
    }

    // 2. Blocking the opponent - if the opponent can win on their next move choose the column that prevents this.

    int opponentNumber = (playerNumber == 1) ? 2 : 1;
    for (int c = 0; c < cols; c++)
    {

        /*
         * Same as (1). If the opponent can win next turn by chosing column c, we will fill it instead.
         */
        int freeRow = makeMove(board, rows, cols, c, getPlayerToken(opponentNumber));

        // The column is full
        if (freeRow == -1) continue;

        if (checkVictory(board, rows, cols, opponentNumber, CONNECT_N)) // A winning move
        {
            board[freeRow][c] = EMPTY;
            return c;
        }
        else 
            board[freeRow][c] = EMPTY;
    }
    
    // 3. Creating a sequence of three - if it is possible to create a sequence of three tokens do so.
    for (int c = 0; c < cols; c++)
    {

        /*
         * Now, we only need to check for CONNECT_N-1 (3).
         */
        int freeRow = makeMove(board, rows, cols, c, getPlayerToken(playerNumber));

        // The column is full
        if (freeRow == -1) continue;

        if (checkVictory(board, rows, cols, playerNumber, CONNECT_N-1)) // A winning move
        {
            board[freeRow][c] = EMPTY;
            return c;
        }
        else 
            board[freeRow][c] = EMPTY;
    }
 
}
