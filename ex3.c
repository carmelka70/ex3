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
    int getFreeRow(char board[][COLS], int rows, int column);

/* Place token in column (0-based). Return row index or -1 if illegal */
    int makeMove(char board[][COLS], int rows, int column, int playerNumber);

/* Check if playerNumber has a sequenceLength long streak that passes through (r,c) */
    int checkSequence(char board[][COLS], int rows, int cols, int playerNumber, int sequenceLength,
                      int r, int c);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
    int humanChoose(char board[][COLS], int cols);

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
int getFreeRow(char board[][COLS], int rows, int column)
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
    return rows - 1;
}

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char board[][COLS], int rows, int column, int playerNumber)
{
    int freeRow = getFreeRow(board, rows, column);

    if (freeRow == -1) // No free rows at this column
        return -1;

    board[freeRow][column] = getPlayerToken(playerNumber);

    return freeRow;
}

void initBoard(char board[][COLS], int rows, int cols)
{
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            board[r][c] = EMPTY;
}

/* Check if playerNumber has a sequenceLength long streak that passes through (r,c) */
int checkSequence(char board[][COLS], int rows, int cols, int playerNumber, int sequenceLength,
                  int row, int col)
{
    // Check for horizontal
    int count = 0;
    for (int r = row, c = col - (sequenceLength - 1); c <= col + (sequenceLength - 1); c++)
    {
        if (!isInBounds(c, 0, cols - 1)) continue;

        if (board[r][c] != getPlayerToken(playerNumber))
            count = 0;
        else
            count++;

        if (count >= sequenceLength)
            return 1;
    }



    // Check for vertical
    count = 0;
    for (int c = col, r = row - (sequenceLength - 1); r <= row + (sequenceLength - 1); r++)
    {
        if (!isInBounds(r, 0, rows - 1)) continue;

        if (board[r][c] != getPlayerToken(playerNumber))
            count = 0;
        else
            count++;

        if (count >= sequenceLength)
            return 1;
    }



    // Check for up diagonal
    count = 0;
    for (int c = col - (sequenceLength - 1), r = row + (sequenceLength - 1); c <= col + (sequenceLength - 1); r--, c++)
    {
        if (!isInBounds(r, 0, rows - 1)) continue;
        if (!isInBounds(c, 0, cols - 1)) continue;

        if (board[r][c] != getPlayerToken(playerNumber))
            count = 0;
        else
            count++;

        if (count >= sequenceLength)
            return 1;
    }


    // Check for down diagnoal
    count = 0;
    for (int c = col - (sequenceLength - 1), r = row - (sequenceLength - 1); r <= row + (sequenceLength - 1); r++, c++)
    {
        if (!isInBounds(r, 0, rows - 1)) continue;
        if (!isInBounds(c, 0, cols - 1)) continue;

        if (board[r][c] != getPlayerToken(playerNumber))
            count = 0;
        else
            count++;

        if (count >= sequenceLength)
            return 1;
    }


    return 0;
}

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char board[][COLS], int cols)
{
    int chosenColumn = -1;
    while (1)
    {
        printf("Enter column (1-%d):", cols);
        if (scanf("%d", &chosenColumn) != 1)
        {
            printf("Invalid input. Enter a number\n");
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

        //The column is valid: return it in base 0.
        return chosenColumn - 1;
    }
}

int orderColumns(int i, int cols)
{
    // The center column. If there are 2 we pick the right one. (+1 adds bias to the bigger value)
    int center = (cols+1) / 2;

    // The distance from the center increases by 1 in pairs of 2 as i increases.
    int distance = (i + 1)/2;

    /* If we have even amount of columns, we start by adding and then subtracting.
     * Else, the columns are odd, and we begin at the "right center" so we first subtract to
     * the left center and then add again. by using i%2 we count on which iteration we are on
     */

    return (i % 2 == cols % 2) ? center - distance : center + distance;
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

    for (int i = 0; i < cols; i++)
    {
        int c = orderColumns(i, cols) - 1; // in base 0

        /*
         * For each column, we will try to add a token, and check using our checkSequence function
         * if after that move we won. we will clear that slot back, and if we won, we will chose that column.
         */
        int freeRow = makeMove(board, rows, c, playerNumber);

        // The column is full
        if (freeRow == -1) continue;

        if (checkSequence(board, rows, cols, playerNumber, CONNECT_N, freeRow, c)) // A winning move
        {
            board[freeRow][c] = EMPTY;
            return c;
        }
        else 
            board[freeRow][c] = EMPTY;
    }

    // 2. Blocking the opponent - if the opponent can win on their next move choose the column that prevents this.

    int opponentNumber = (playerNumber == 1) ? 2 : 1;
    for (int i = 0; i < cols; i++)
    {
        int c = orderColumns(i, cols) - 1; // in base 0

        /*
         * Same as (1). If the opponent can win next turn by chosing column c, we will fill it instead.
         */
        int freeRow = makeMove(board, rows, c, opponentNumber);

        // The column is full
        if (freeRow == -1) continue;

        if (checkSequence(board, rows, cols, opponentNumber, CONNECT_N, freeRow, c)) // A winning move
        {
            board[freeRow][c] = EMPTY;
            return c;
        }
        else 
            board[freeRow][c] = EMPTY;
    }
    
    // 3. Creating a sequence of three - if it is possible to create a sequence of three tokens do so.

    for (int i = 0; i < cols; i++)
    {
        int c = orderColumns(i, cols) - 1; // in base 0

        /*
         * Now, we only need to check for CONNECT_N-1 (3).
         */
        int freeRow = makeMove(board, rows, c, playerNumber);

        // The column is full
        if (freeRow == -1) continue;

        if (checkSequence(board, rows, cols, playerNumber, CONNECT_N-1, freeRow, c)) // A 3 token sequence
        {
            board[freeRow][c] = EMPTY;
            return c;
        }
        else 
            board[freeRow][c] = EMPTY;
    }
 
    // 4. Blocking the opponent’s sequence of three.

    for (int i = 0; i < cols; i++)
    {
        int c = orderColumns(i, cols) - 1; // in base 0

        /*
         * Lastly, check for oponnent possible sequence of 3.
         */
        int freeRow = makeMove(board, rows, c, opponentNumber);

        // The column is full
        if (freeRow == -1) continue;

        if (checkSequence(board, rows, cols, opponentNumber, CONNECT_N - 1, freeRow, c)) // A 3 token sequence
        {
            board[freeRow][c] = EMPTY;
            return c;
        }
        else 
            board[freeRow][c] = EMPTY;
    }

    // 5. Choosing according to the arbitrary ordering rule.

    for (int i = 0; i < cols; i++)
    {
        int c = orderColumns(i, cols) - 1; // in base 0

        /*
         * If all else fails, we follow the ordering rule and choose the first not full column.
         */

        if (!isColumnFull(board, c))
            return c;
    }

    return -1;
}

void runConnectFour(char board[][COLS], int rows, int cols, int p1Type, int p2Type)
{
    int currentPlayer = 1;

    while (1)
    {
        printf("Player %d (%c) turn.\n", currentPlayer, getPlayerToken(currentPlayer));

        int playerType = (currentPlayer == 1) ? p1Type : p2Type;

        int chosenColumn, chosenRow;
        if (playerType == HUMAN)
        {
            chosenColumn = humanChoose(board, cols);
            chosenRow = makeMove(board, rows, chosenColumn, currentPlayer);
        }
        else // playerType == COMPUTER
        {
            chosenColumn = computerChoose(board, rows, cols, currentPlayer);
            printf("Computer chose column %d\n", chosenColumn+1);
            chosenRow = makeMove(board, rows, chosenColumn, currentPlayer);
        }

        printBoard(board, rows, cols);
        
        if (isBoardFull(board, cols))
        {
            printf("Board full and no winner. It's a tie!\n");
            return;
        }

        if (checkSequence(board, rows, cols, currentPlayer, CONNECT_N, chosenRow, chosenColumn)) // current player won
        {
            printf("Player %d (%c) wins!\n", currentPlayer, getPlayerToken(currentPlayer));
            return;
        }

        // Switch players.
        currentPlayer = (currentPlayer == 1) ? 2 : 1; 
    }
}
