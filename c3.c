/* ---------------------------------------- */
/* CONNECT-3: human vs computer tic-tac-toe */
/* Illustration of the MiniMax algorithm    */
/* Coded by Trinh D.D. Nguyen               */
/* ---------------------------------------- */
/* just type 'make' to compile              */
/* ---------------------------------------- */

#include <stdio.h>

#define N           3                  /* board size */
#define X           ('x')              /* cross piece */
#define O           ('o')              /* nought piece */
#define E           (' ')              /* empty cell */
#define XSCORE      (+1)               /* evaluation score for X */
#define OSCORE      (-1)               /* evaluation score for O */
#define TIE         (0)                /* no winner found, tie */
#define max(a, b)   ((a)>(b))?(a):(b)  /* to use with minimax() */
#define min(a, b)   ((a)<(b))?(a):(b)
#define false       (0)                /* boolean values */
#define true        (1)

typedef int bool;                       /* simulation of boolean type */ 

struct move {                           /* store the best move found */
    int r, c;                           /* only used by the AI */
};

char human = O;                         /* player symbol */
char computer = X;                      /* computer symbol */

static char current;                    /* current play */
static char board[3][3] =               /* empty game board  */
    {E, E, E,                           /*  1st row indices: 0  1  2 */
     E, E, E,                           /*  2nd row indices: 3  4  5 */
     E, E, E};                          /*  3rd row indices: 6  7  8 */  
static int states = 0;                  /* searched state counter */

/* check if a cell is empty */
bool playable(char board[3][3], int c, int r)
{
    return board[r][c] == E;
}

/* check if the board still playable */
bool has_move(char board[3][3])
{
    int r, c;
    for (r = 0; r < N; r++)
    for (c = 0; c < N; c++)
        if (board[r][c] == E) return true;
    return false;
}

/* display game board */
void show_board(char board[3][3])
{
    int v = 0, r, c;
    for (r = 0; r < N; r++)
    {
	for (c = 0; c < N; c++)
        {
            if (!playable(board, c, r))
                printf("[ %c ]", board[r][c]);
	        else
                printf("[ %d ]", v);
            v++;
        }
        printf("\n");
    }
    printf("\n");
}

/* board evaluate function: X wins = +1, O wins = -1, tie = 0 */
int evaluate(char board[3][3])
{
    int r, c;

    /* row checks */
	for (r = 0; r < N; r++)
    {
        if (board[r][0] == board[r][1] && board[r][1] == board[r][2])
        {
            if (board[r][0] == X) return XSCORE; else
            if (board[r][0] == O) return OSCORE;
        }
    }

    /* column check */
	for (c = 0; c < N; c++)
    {
        if (board[0][c] == board[1][c] && board[1][c] == board[2][c])
        {
            if (board[0][c] == X) return XSCORE; else
            if (board[0][c] == O) return OSCORE;
        }
    }

    /* diagonal checks */
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        if (board[0][0] == X) return XSCORE; else
        if (board[0][0] == O) return OSCORE;
    }

    if (board[2][0] == board[1][1] && board[1][1] == board[0][2])
    {
        if (board[2][0] == X) return XSCORE; else
        if (board[2][0] == O) return OSCORE;
    }
    return TIE;
}

/* the minimax algorithm: assuming player is on the minimizer side */
int minimax(char board[3][3], int depth, bool ismax)
{
    int r, c, best;
    int score = evaluate(board);        /* evaluating the board */
    if (score != TIE) return score;     /* return score if a player won */
    if (!has_move(board)) return TIE;   /* no more move? it is a tie */

    states++;                           /* explored a search state */
    if (ismax)                          /* evaluating the maximizer player */
    {
        int best = -1000;               /* for finding max */
		for (r = 0; r < N; r++)         /* scan the game board */
		for (c = 0; c < N; c++)
            if (board[r][c] == E)       /* found an empty cell */
            {
                board[r][c] = computer; /* assuming computer move on that cell */
                /* recursively explore down the state space */
                score = minimax(board, depth+1, false);
                board[r][c] = E;        /* undo that move */
                best = max(score, best);/* obtain the maximum score */
            }
        return best;                    /* and return it */
    }
    else                                /* the minimizer's turn */
    {
        int best = 1000;                /* for finding min */
	    for (r = 0; r < N; r++)         /* scan the game board */
	    for (c = 0; c < N; c++)
            if (board[r][c] == E)       /* found an empty cell */
            {
                board[r][c] = human;    /* assuming human move on that cell */
                /* recursively explore down the state space */
		        score = minimax(board, depth+1, true);
                board[r][c] = E;        /* undo that move */
                best = min(score, best);/* obtain the minimum score */
            }
        return best;                    /* also return it */
    }
}

/* human make his move */
bool human_move(char board[3][3], int c, int r)
{
    if (playable(board, c, r))          /* check if the cell is empty */
    {
        board[r][c] = human;            /* set the piece */
        current = computer;             /* and switch turn to computer */
        return true;                    /* human made a move */
    }
    return false;                       /* human could not make a move */
}

/* AI select its best move */
void computer_move(char board[3][3])
{
    int best = -1000;			/* for finding the best move */
    int score, r, c;
    struct move mv = {-1, -1};

    states = 0;                         /* reset state counter */
    for (r = 0; r < N; r++)             /* board scan */
    for (c = 0; c < N; c++)
        if (board[r][c] == E)           /* found an empty cell */
        {
            board[r][c] = computer;     /* assuming the move */
            /* search the search space */
	        score = minimax(board, 0, false);
            board[r][c] = E;            /* and undo it */
            if (score > best)           /* find the minimum score */             
            {
                best = score;           /* and save it */
                mv.r = r;               /* also the coordinates */
                mv.c = c;               /* of that move */
            }
        }
    board[mv.r][mv.c] = computer;       /* computer make a move */
    current = human;                    /* turn is now back to human */
}

int main()
{  
    bool quit = false;                  /* quit flag */
    int input, c, r, eval;

    current = human;                    /* human moves first */

    printf("human = %c - computer = %c\n", human, computer);
    do {
        show_board(board);              /* draw game board */
        if (has_move(board))            /* if the board is playable */
        {
            do {                        /* get user input as index */
                printf("human move (-1 to quit): ");
                scanf("%d", &input);
                if (input == -1)  
                {
                    quit = true;
                    break;
                }
                r = input / 3;          /* convert to row and col */
                c = input % 3;          /* and make the move if possible */
            } while(!human_move(board, c, r));
        }
        else quit = true;               /* no more cell to play */

        if (!quit)                      /* if human placed a move */
        {
            computer_move(board);       /* now to the computer's turn */

            /* optional: print out the number of searched states */
            printf("searched %d states\n", states);

            eval = evaluate(board);     /* evaluate the board */
            switch (eval) {
            case XSCORE: printf("computer wins!\n"); quit = true; break;
            case OSCORE: printf("human wins!\n"); quit = true; break;
            }
        }
    } while(!quit);

    printf("finalized:\n");
    show_board(board);
    if (!has_move(board) && eval == TIE) printf("ties!\n"); 

    return 0;
}