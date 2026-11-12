#include <stdio.h>

#define SIZE 4   // board is always 4x4, so it's a fixed constant

/* ---------- "random" numbers without stdlib ----------
   rand() is not in our syllabus, so I made a tiny formula of my own.
   Every call changes the seed, so the next number looks different.
   The seed is passed as a pointer so the change sticks outside the function. */
int nextRandom(int *seed)
{
    *seed = (*seed * 1103 + 12345) % 32749;
    return *seed;
}

void printBoard(int board[SIZE][SIZE], int score)
{
    int i, j;

    printf("\n\n  2048   |   SCORE: %d\n\n", score);
    for (i = 0; i < SIZE; i++)
    {
        printf("  +------+------+------+------+\n  |");
        for (j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
                printf("      |");          // empty cell stays blank
            else
                printf(" %4d |", board[i][j]);
        }
        printf("\n");
    }
    printf("  +------+------+------+------+\n\n");
}

// puts a new tile (mostly 2, sometimes 4) on a random empty cell
void addTile(int board[SIZE][SIZE], int *seed)
{
    int i, j;
    int empty = 0, pick, value;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            if (board[i][j] == 0)
                empty++;

    if (empty == 0)
        return;                             // board is full, nothing to add

    pick = nextRandom(seed) % empty;        // which empty cell we use
    if (nextRandom(seed) % 10 == 0)
        value = 4;                          // roughly 1 in 10 times
    else
        value = 2;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                if (pick == 0)
                {
                    board[i][j] = value;
                    return;
                }
                pick--;
            }
        }
    }
}

/* ---------- the movement logic ----------
   Trick: I don't write 4 different move codes. For any direction, I pull out
   one row/column as a "line" so that it always slides towards index 0.
   Then one function handles the sliding + merging for everything. */

// finds the real board position of the j-th cell of line number i
void getPosition(char dir, int i, int j, int *r, int *c)
{
    if (dir == 'a')        // left: read the row normally
    {
        *r = i;
        *c = j;
    }
    else if (dir == 'd')   // right: read the row backwards
    {
        *r = i;
        *c = SIZE - 1 - j;
    }
    else if (dir == 'w')   // up: read the column top to bottom
    {
        *r = j;
        *c = i;
    }
    else                   // down: read the column bottom to top
    {
        *r = SIZE - 1 - j;
        *c = i;
    }
}

void readLine(int board[SIZE][SIZE], char dir, int i, int line[SIZE])
{
    int j, r, c;
    for (j = 0; j < SIZE; j++)
    {
        getPosition(dir, i, j, &r, &c);
        line[j] = board[r][c];
    }
}

void writeLine(int board[SIZE][SIZE], char dir, int i, int line[SIZE])
{
    int j, r, c;
    for (j = 0; j < SIZE; j++)
    {
        getPosition(dir, i, j, &r, &c);
        board[r][c] = line[j];
    }
}

// slide everything to the front, merge equal neighbours once, slide again
void processLine(int line[SIZE], int *score)
{
    int temp[SIZE] = {0, 0, 0, 0};
    int j, k = 0;

    // step 1: remove the gaps
    for (j = 0; j < SIZE; j++)
    {
        if (line[j] != 0)
        {
            temp[k] = line[j];
            k++;
        }
    }

    // step 2: merge pairs (a merged tile can't merge again in the same move)
    for (j = 0; j < SIZE - 1; j++)
    {
        if (temp[j] != 0 && temp[j] == temp[j + 1])
        {
            temp[j] = temp[j] * 2;
            *score += temp[j];
            temp[j + 1] = 0;
        }
    }

    // step 3: remove the gaps again, because merging created new ones
    for (j = 0; j < SIZE; j++)
        line[j] = 0;
    k = 0;
    for (j = 0; j < SIZE; j++)
    {
        if (temp[j] != 0)
        {
            line[k] = temp[j];
            k++;
        }
    }
}

// returns 1 if something actually moved, otherwise 0
int moveBoard(int board[SIZE][SIZE], char dir, int *score)
{
    int i, j, moved = 0;
    int line[SIZE], old[SIZE];

    for (i = 0; i < SIZE; i++)
    {
        readLine(board, dir, i, line);
        for (j = 0; j < SIZE; j++)
            old[j] = line[j];               // keep a copy to compare later

        processLine(line, score);

        for (j = 0; j < SIZE; j++)
            if (old[j] != line[j])
                moved = 1;

        writeLine(board, dir, i, line);
    }
    return moved;
}

/* ---------- game state checks ---------- */

int hasWon(int board[SIZE][SIZE])
{
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            if (board[i][j] == 2048)
                return 1;
    return 0;
}

// game continues if there is an empty cell OR two equal tiles are touching
int canMove(int board[SIZE][SIZE])
{
    int i, j;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
                return 1;
            if (j < SIZE - 1 && board[i][j] == board[i][j + 1])
                return 1;
            if (i < SIZE - 1 && board[i][j] == board[i + 1][j])
                return 1;
        }
    }
    return 0;
}

int main()
{
    int board[SIZE][SIZE];
    int score = 0, seed, lucky;
    int i, j, moved, playing = 1;
    char move;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            board[i][j] = 0;

    printf("===== 2048 in C =====\n");
    printf("Moves: w = up, a = left, s = down, d = right, q = quit\n");
    printf("Merge equal tiles and reach 2048!\n\n");

    // the player's lucky number is our starting seed, so every game feels different
    printf("Enter your lucky number: ");
    scanf("%d", &lucky);
    if (lucky < 0)
        lucky = -lucky;
    seed = lucky % 32749;

    addTile(board, &seed);
    addTile(board, &seed);

    while (playing)
    {
        printBoard(board, score);
        printf("Your move: ");
        scanf(" %c", &move);                // space before %c skips the Enter key

        if (move >= 'A' && move <= 'Z')
            move = move + 32;               // capital to small letter

        if (move == 'q')
        {
            playing = 0;
        }
        else if (move == 'w' || move == 'a' || move == 's' || move == 'd')
        {
            moved = moveBoard(board, move, &score);

            if (moved)
                addTile(board, &seed);      // new tile only if the board changed
            else
                printf("\nNothing moved there, try another direction.\n");

            if (hasWon(board))
            {
                printBoard(board, score);
                printf("YOU MADE 2048! Absolute legend.\n");
                playing = 0;
            }
            else if (!canMove(board))
            {
                printBoard(board, score);
                printf("No moves left. Game over!\n");
                playing = 0;
            }
        }
        else
        {
            printf("\nInvalid key. Use w, a, s, d or q.\n");
        }
    }

    printf("\nFinal score: %d\n", score);
    return 0;
}
