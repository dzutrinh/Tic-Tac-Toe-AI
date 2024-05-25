#ifndef _TICTACTOE_MINIMAX_ENGINE_H_
#define _TICTACTOE_MINIMAX_ENGINE_H_

#include <stdio.h>

/* check if a cell is empty */
bool is_playable(game_board g, int c, int r) {
    return g[r][c] == CELL_E;
}

/* check if a cell is occupied */
bool is_occupied(game_board g, int c, int r) {
    return g[r][c] != CELL_E;
}

/* check if the board is still playable */
bool has_move(game_board g) {
    for (int r = 0; r < BOARD_SIZE; r++)
    for (int c = 0; c < BOARD_SIZE; c++)
        if (g[r][c] == CELL_E) return true;
    return false;
}

/* initialize game board */
void init_board(game_board g) {
    for (int r = 0; r < BOARD_SIZE; r++)
    for (int c = 0; c < BOARD_SIZE; c++)
        g[r][c] = CELL_E;
}

/* display game board */
void show_board(game_board g, bool final) {

#define HBAR    for (int c = 0; c < BOARD_SIZE; c++) \
                    printf(C_DARK"+----");           \
                printf("+\n");

    HBAR;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (is_occupied(g, c, r)) {
                if (g[r][c] == CELL_X)
                    printf(C_DARK"| "C_X"%2c ", g[r][c]);
                else
                    printf(C_DARK"| "C_O"%2c ", g[r][c]);
            }
            else {
                if (!final)
                    printf(C_DARK"| "C_EMPTY"%2d "C_DARK, r * BOARD_SIZE + c);
                else
                    printf(C_DARK"|    ");
            }
        }
        printf(C_DARK"|\n");
        HBAR;
    }
}

bool match_row(game_board g, int r, char sym) {
    bool match = true;
    for (int c = 0; c < BOARD_SIZE && match; c++)
        match = match && g[r][c] == sym; 
    return match;
}

bool match_column(game_board g, int c, char sym) {
    bool match = true;
    for (int r = 0; r < BOARD_SIZE && match; r++)
        match = match && (g[r][c] == sym); 
    return match;
}

bool match_primary_diagonal(game_board g, char sym) {
    bool match = true;
    for (int i = 0; i < BOARD_SIZE && match; i++)
        match = match && (g[i][i] == sym); 
    return match;
}

bool match_secondary_diagonal(game_board g, char sym) {
    bool match = true;
    for (int i = 0; i < BOARD_SIZE && match; i++)
        match = match && (g[i][BOARD_SIZE-1-i] == sym); 
    return match;
}

/* board evaluate function: X wins = +1, O wins = -1, tie = 0 */
int evaluate(game_board g) {
    /* row checks */
	for (int r = 0; r < BOARD_SIZE; r++) {
        if (match_row(g, r, CELL_X)) return SCORE_X;
        if (match_row(g, r, CELL_O)) return SCORE_O;
    }

    /* column checks */
	for (int c = 0; c < BOARD_SIZE; c++) {
        if (match_column(g, c, CELL_X)) return SCORE_X;
        if (match_column(g, c, CELL_O)) return SCORE_O;
    }

    /* primary diagonal check */
    if (match_primary_diagonal(g, CELL_X)) return SCORE_X;
    if (match_primary_diagonal(g, CELL_O)) return SCORE_O;

    /* secondary diagonal check */
    if (match_secondary_diagonal(g, CELL_X)) return SCORE_X;
    if (match_secondary_diagonal(g, CELL_O)) return SCORE_O;

    return SCORE_TIE;
}

/* the minimax algorithm: assuming player is on the minimizer side */
int minimax(game_board g, int depth, bool ismax) {
    int r, c, best;
    int score = evaluate(g);        /* evaluating the board */
    if (score != SCORE_TIE) return score;     /* return score if a player won */
    if (!has_move(g)) return SCORE_TIE;   /* no more move? it is a tie */

    if (depth >= game_depth) return score;

    show_progress();
    states++;                           /* explored a search state */
    if (ismax) {                        /* evaluating the maximizer player */
        int best = -1000;               /* for finding max */
		for (r = 0; r < BOARD_SIZE; r++)         /* scan the game board */
		for (c = 0; c < BOARD_SIZE; c++)
            if (g[r][c] == CELL_E) {     /* found an empty cell */
                g[r][c] = computer; /* assuming computer move on that cell */
                /* recursively explore down the state space */
                score = minimax(g, depth+1, false);
                g[r][c] = CELL_E;        /* undo that move */
                best = maxi(score, best);/* obtain the maximum score */
            }
        return best;                    /* and return it */
    }
    else {                              /* the minimizer's turn */
        int best = 1000;                /* for finding min */
	    for (r = 0; r < BOARD_SIZE; r++)         /* scan the game board */
	    for (c = 0; c < BOARD_SIZE; c++)
            if (g[r][c] == CELL_E) {     /* found an empty cell */
                g[r][c] = human;    /* assuming human move on that cell */
                /* recursively explore down the state space */
		        score = minimax(g, depth+1, true);
                g[r][c] = CELL_E;        /* undo that move */
                best = mini(score, best);/* obtain the minimum score */
            }
        return best;                    /* also return it */
    }
}

/* human make his move */
bool human_move(game_board g, int c, int r) {
    if (is_playable(g, c, r)) {        /* check if the cell is empty */
        g[r][c] = human;            /* set the piece */
        current = computer;             /* and switch turn to computer */
        return true;                    /* human made a move */
    }
    return false;                       /* human could not make a move */
}

/* AI select its best move */
void computer_move(game_board g) {
    int best = -1000;			        /* for finding the best move */
    int score, r, c;
    move mv = {-1, -1};

    states = 0;                         /* reset state counter */
    for (r = 0; r < BOARD_SIZE; r++)    /* board scan */
    for (c = 0; c < BOARD_SIZE; c++)
        if (g[r][c] == CELL_E) {        /* found an empty cell */
            g[r][c] = computer;         /* assuming the move */
            /* search the search space */
	        score = minimax(g, 0, false);
            g[r][c] = CELL_E;           /* and undo it */
            if (score > best) {         /* find the minimum score */             
                best = score;           /* and save it */
                mv.r = r;               /* also the coordinates */
                mv.c = c;               /* of that move */
            }
        }
    g[mv.r][mv.c] = computer;           /* computer make a move */
    current = human;                    /* turn is now back to human */
}

#endif