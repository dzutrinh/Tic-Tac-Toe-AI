/* 
 * ENGINE.H: Tic-Tac-Toe AI MiniMax AI Core
 * ---------
 * Coded by Trinh D.D. Nguyen
 * Last updates: Nov, 2025
 */
#ifndef _TICTACTOE_MINIMAX_ENGINE_H_
#define _TICTACTOE_MINIMAX_ENGINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defs.h"

#define MIN_INF (-1000)
#define MAX_INF (+1000)

/* Move ordering: prioritize center, corners, then edges */
static const int move_priority[9] = {4, 0, 2, 6, 8, 1, 3, 5, 7};

/* =============== PROTOTYPES ==================== */

unsigned int hash_board(game_board g);

void clear_trans_table();

int lookup_trans_table(unsigned int hash, int depth);

bool is_playable(game_board g, int c, int r);

bool is_occupied(game_board g, int c, int r);

bool has_move(game_board g);

void init_board(game_board g);

void show_board(game_board g, bool final);

int evaluate(game_board g);

#ifdef _USE_ALPHA_BETA_PRUNE_
    int minimax(game_board g, int depth, bool ismax, int alpha, int beta);
#else
    int minimax(game_board g, int depth, bool ismax);
#endif

bool human_move(game_board g, int c, int r);

void computer_move(game_board g);

/* =============================================== */

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
    /* Fast path: use move counter if within valid range */
    if (move_count >= 0 && move_count < BOARD_SIZE * BOARD_SIZE) {
        return move_count < BOARD_SIZE * BOARD_SIZE;
    }
    /* Fallback: count empty cells (for tests that manually fill boards) */
    int count = 0;
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            if (g[r][c] == CELL_E) count++;
    return count > 0;
}

/* initialize game board */
void init_board(game_board g) {
    for (int r = 0; r < BOARD_SIZE; r++)
    for (int c = 0; c < BOARD_SIZE; c++)
        g[r][c] = CELL_E;
    move_count = 0;
    clear_trans_table();
}

/* display game board */
void show_board(game_board g, bool final) {

#define HBAR    for (int c = 0; c < BOARD_SIZE; c++) \
                    printf(C_DARK"+-----"C_RESET);          \
                printf(C_RESET"+\n");

    HBAR;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (is_occupied(g, c, r)) {
                if (g[r][c] == CELL_X)
                    printf(C_DARK"| "C_X"%2c  "C_DARK, g[r][c]);
                else
                    printf(C_DARK"| "C_O"%2c  "C_DARK, g[r][c]);
            }
            else {
                if (!final)
                    printf(C_DARK"| "C_EMPTY"%2d  "C_DARK, r * BOARD_SIZE + c);
                else
                    printf(C_DARK"|     ");
            }
        }
        printf(C_DARK"|\n");
        HBAR;
    }
}

/* ---------------------- */
/* Transposition table functions */
unsigned int hash_board(game_board g) {
    unsigned int hash = 0;
    unsigned int mult = 1;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int val = (g[r][c] == CELL_X) ? 1 : (g[r][c] == CELL_O) ? 2 : 0;
            hash += val * mult;
            mult *= 3;
        }
    }
    return hash;
}

void clear_trans_table() {
    for (int i = 0; i < TRANS_TABLE_SIZE; i++) {
        trans_table[i].hash = 0;
        trans_table[i].score = 0;
        trans_table[i].depth = -1;
    }
}

int lookup_trans_table(unsigned int hash, int depth) {
    int idx = hash % TRANS_TABLE_SIZE;
    if (trans_table[idx].hash == hash && trans_table[idx].depth >= depth) {
        return trans_table[idx].score;
    }
    return MIN_INF - 1;  /* not found */
}

void store_trans_table(unsigned int hash, int score, int depth) {
    int idx = hash % TRANS_TABLE_SIZE;
    if (trans_table[idx].depth <= depth) {
        trans_table[idx].hash = hash;
        trans_table[idx].score = score;
        trans_table[idx].depth = depth;
    }
}
/* ---------------------- */

/* board evaluate function: X wins = +1, O wins = -1, tie = 0 */
/* Optimized with inlined logic for better performance */
int evaluate(game_board g) {
    int i, match;
    
    /* row checks - inlined */
	for (int r = 0; r < BOARD_SIZE; r++) {
        match = 1;
        for (int c = 0; c < BOARD_SIZE && match; c++)
            match = match && (g[r][c] == CELL_X);
        if (match) return SCORE_X;
        
        match = 1;
        for (int c = 0; c < BOARD_SIZE && match; c++)
            match = match && (g[r][c] == CELL_O);
        if (match) return SCORE_O;
    }

    /* column checks - inlined */
	for (int c = 0; c < BOARD_SIZE; c++) {
        match = 1;
        for (int r = 0; r < BOARD_SIZE && match; r++)
            match = match && (g[r][c] == CELL_X);
        if (match) return SCORE_X;
        
        match = 1;
        for (int r = 0; r < BOARD_SIZE && match; r++)
            match = match && (g[r][c] == CELL_O);
        if (match) return SCORE_O;
    }

    /* primary diagonal check - inlined */
    match = 1;
    for (i = 0; i < BOARD_SIZE && match; i++)
        match = match && (g[i][i] == CELL_X);
    if (match) return SCORE_X;
    
    match = 1;
    for (i = 0; i < BOARD_SIZE && match; i++)
        match = match && (g[i][i] == CELL_O);
    if (match) return SCORE_O;

    /* secondary diagonal check - inlined */
    match = 1;
    for (i = 0; i < BOARD_SIZE && match; i++)
        match = match && (g[i][BOARD_SIZE-1-i] == CELL_X);
    if (match) return SCORE_X;
    
    match = 1;
    for (i = 0; i < BOARD_SIZE && match; i++)
        match = match && (g[i][BOARD_SIZE-1-i] == CELL_O);
    if (match) return SCORE_O;

    return SCORE_TIE;
}

int mini(int a, int b) {
    return a < b ? a : b;
}

int maxi(int a, int b) {
    return a > b ? a : b;
}

#ifdef _USE_ALPHA_BETA_PRUNE_
/* the minimax algorithm: assuming player is on the minimizer side */
int minimax(game_board g, int depth, bool ismax, int alpha, int beta) {
    int r, c, best, score;
    
    /* Check transposition table */
    unsigned int hash = hash_board(g);
    int cached = lookup_trans_table(hash, game_depth - depth);
    if (cached != MIN_INF - 1) return cached;
    
    score = evaluate(g);                    /* evaluating the board */
    if (score != SCORE_TIE) return score;   /* return score if a player won */
    if (!has_move(g)) return SCORE_TIE;     /* no more move? it is a tie */

    if (depth >= game_depth) return score;

    progress_show();                        /* show progress bar */

    states++;                               /* explored a search state */
    
    if (ismax) {                            /* evaluating the maximizer player */
        best = MIN_INF;                     /* for finding max */
        
        /* Move ordering: try priority moves first */
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            int pos = move_priority[i];
            r = pos / BOARD_SIZE;
            c = pos % BOARD_SIZE;
            
            if (g[r][c] == CELL_E) {        /* found an empty cell */
                g[r][c] = computer;         /* assuming computer move on that cell */
                move_count++;
                /* recursively explore down the state space */
                score = minimax(g, depth+1, false, alpha, beta);
                g[r][c] = CELL_E;           /* undo that move */
                move_count--;
                best = maxi(score, best);   /* obtain the maximum score */
                
                /* alpha-beta pruning */
                alpha = maxi(alpha, best);
                if (beta <= alpha) break;   /* cutoff */
            }
        }
        
        /* Store in transposition table */
        store_trans_table(hash, best, game_depth - depth);
        return best;
    }
    else {                                  /* the minimizer's turn */
        best = MAX_INF;                     /* for finding min */
        
        /* Move ordering: try priority moves first */
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            int pos = move_priority[i];
            r = pos / BOARD_SIZE;
            c = pos % BOARD_SIZE;
            
            if (g[r][c] == CELL_E) {        /* found an empty cell */
                g[r][c] = human;            /* assuming human move on that cell */
                move_count++;
                /* recursively explore down the state space */
		        score = minimax(g, depth+1, true, alpha, beta);
                g[r][c] = CELL_E;           /* undo that move */
                move_count--;
                best = mini(score, best);   /* obtain the minimum score */
                
                /* alpha-beta pruning */
                beta = mini(beta, best);
                if (beta <= alpha) break;   /* cutoff */
            }
        }
        
        /* Store in transposition table */
        store_trans_table(hash, best, game_depth - depth);
        return best;
    }
}
#else
/* the minimax algorithm: assuming player is on the minimizer side */
int minimax(game_board g, int depth, bool ismax) {
    int r, c, best, score;
    
    /* Check transposition table */
    unsigned int hash = hash_board(g);
    int cached = lookup_trans_table(hash, game_depth - depth);
    if (cached != MIN_INF - 1) return cached;
    
    score = evaluate(g);                    /* evaluating the board */
    if (score != SCORE_TIE) return score;   /* return score if a player won */
    if (!has_move(g)) return SCORE_TIE;     /* no more move? it is a tie */

    if (depth >= game_depth) return score;

    progress_show();                        /* show progress bar */

    states++;                               /* explored a search state */
    
    if (ismax) {                            /* evaluating the maximizer player */
        best = MIN_INF;                     /* for finding max */
        
        /* Move ordering: try priority moves first */
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            int pos = move_priority[i];
            r = pos / BOARD_SIZE;
            c = pos % BOARD_SIZE;
            
            if (g[r][c] == CELL_E) {        /* found an empty cell */
                g[r][c] = computer;         /* assuming computer move on that cell */
                move_count++;
                /* recursively explore down the state space */
                score = minimax(g, depth+1, false);
                g[r][c] = CELL_E;           /* undo that move */
                move_count--;
                best = maxi(score, best);   /* obtain the maximum score */
            }
        }
        
        /* Store in transposition table */
        store_trans_table(hash, best, game_depth - depth);
        return best;
    }
    else {                                  /* the minimizer's turn */
        best = MAX_INF;                     /* for finding min */
        
        /* Move ordering: try priority moves first */
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            int pos = move_priority[i];
            r = pos / BOARD_SIZE;
            c = pos % BOARD_SIZE;
            
            if (g[r][c] == CELL_E) {        /* found an empty cell */
                g[r][c] = human;            /* assuming human move on that cell */
                move_count++;
                /* recursively explore down the state space */
		        score = minimax(g, depth+1, true);
                g[r][c] = CELL_E;           /* undo that move */
                move_count--;
                best = mini(score, best);   /* obtain the minimum score */
            }
        }
        
        /* Store in transposition table */
        store_trans_table(hash, best, game_depth - depth);
        return best;
    }
}
#endif

/* human make his move */
bool human_move(game_board g, int c, int r) {
    if (is_playable(g, c, r)) {        /* check if the cell is empty */
        g[r][c] = human;            /* set the piece */
        move_count++;               /* increment move counter */
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
    
    /* Easy mode: make random moves */
    if (game_depth == GAME_EASY) {
        int empty_cells[BOARD_SIZE * BOARD_SIZE][2];
        int count = 0;
        
        /* find all empty cells */
        for (r = 0; r < BOARD_SIZE; r++)
        for (c = 0; c < BOARD_SIZE; c++)
            if (g[r][c] == CELL_E) {
                empty_cells[count][0] = r;
                empty_cells[count][1] = c;
                count++;
            }
        
        /* pick a random empty cell */
        if (count > 0) {
            int random_index = rand() % count;
            mv.r = empty_cells[random_index][0];
            mv.c = empty_cells[random_index][1];
        }
    }
    else {
        /* Normal mode: use minimax algorithm with move ordering */
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            int pos = move_priority[i];
            r = pos / BOARD_SIZE;
            c = pos % BOARD_SIZE;
            
            if (g[r][c] == CELL_E) {        /* found an empty cell */
                g[r][c] = computer;         /* assuming the move */
                move_count++;
                /* search the search space */
#ifdef _USE_ALPHA_BETA_PRUNE_
                score = minimax(g, 0, false, MIN_INF, MAX_INF);
#else
                score = minimax(g, 0, false);
#endif
                g[r][c] = CELL_E;           /* and undo it */
                move_count--;
                
                if (score > best) {         /* find the best score */             
                    best = score;           /* and save it */
                    mv.r = r;               /* also the coordinates */
                    mv.c = c;               /* of that move */
                    
                    /* Early termination: if winning move found, take it */
                    if (best == SCORE_X) break;
                }
            }
        }
    }
    
    g[mv.r][mv.c] = computer;           /* computer make a move */
    move_count++;                       /* increment move counter */
    current = human;                    /* turn is now back to human */
}

#endif