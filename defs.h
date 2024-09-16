/* 
 * DEFS.H: Tic-Tac-Toe AI global definitions 
 * -------
 * Coded by Trinh D.D. Nguyen
 * Last updates: May, 2024
 */
#ifndef _TICTACTOE_MINIMAX_DEFS_H_
#define _TICTACTOE_MINIMAX_DEFS_H_

/* Alpha-Beta pruning strategy:
 * enable to compact the search space for faster runtime
 */
#define _USE_ALPHA_BETA_PRUNE_

#define BOARD_SIZE      3                  /* board size, default at 3 */
#define GAME_EASY       2
#define GAME_MEDIUM     3
#define GAME_HARD       5
#define GAME_IMPOSSIBLE 6
#define GAME_VERSION    0x0300             /* game version */
#ifdef  _USE_ALPHA_BETA_PRUNE_
    #define GAME_ENGINE     "ABPRUNE"
#else
    #define GAME_ENGINE     "MINIMAX"
#endif

#define CELL_X          ('X')              /* cross piece */
#define CELL_O          ('O')              /* nought piece */
#define CELL_E          (0)                /* empty cell */
#define SCORE_X         (+1)               /* evaluation score for X */
#define SCORE_O         (-1)               /* evaluation score for O */
#define SCORE_TIE       (0)                /* no winner found, tie */

#ifndef __DJGPP__
	#define C_X             "\x1b[38;5;20m"
	#define C_O             "\x1b[38;5;207m"
	#define C_EMPTY         "\x1b[38;5;34m"
	#define C_RESET         "\x1b[0m"
	#define C_BRIGHT        "\x1b[38;5;231m"
	#define C_NORMAL        "\x1b[38;5;243m"
	#define C_DARK          "\x1b[38;5;247m"
	#define C_ERROR         "\x1b[38;5;196m"
	#define C_WARNING       "\x1b[38;5;214m"
	#define C_THINKING      "\x1b[38;5;190m"
	#define C_MISC          "\x1b[38;5;52m"
	#define C_EASY          "\x1b[38;5;40m"
	#define C_MEDIUM        "\x1b[38;5;220m"
	#define C_HARD          "\x1b[38;5;202m"
	#define C_IMPOSSIBLE    "\x1b[38;5;9m"
#else
	#define C_X             ""
	#define C_O		""
	#define C_EMPTY 	""
	#define C_RESET 	""
	#define C_BRIGHT	""
	#define C_NORMAL	""
	#define C_DARK		""
	#define C_ERROR 	""
	#define C_WARNING	""
	#define C_THINKING	""
	#define C_MISC		""
	#define C_EASY		""
	#define C_MEDIUM	""
	#define C_HARD          ""
	#define C_IMPOSSIBLE	""
#endif

typedef enum {false, true} bool;    /* simulation of boolean type */ 

typedef struct mv {                 /* store the best move found */
    int r, c;                       /* only used by the AI */
} move;

typedef char game_board[BOARD_SIZE][BOARD_SIZE];

char human = CELL_O;                /* human player symbol */
char computer = CELL_X;             /* computer symbol */
char current;                       /* current turn */
int game_depth = GAME_MEDIUM;       /* AI level */
game_board board;                   /* empty game board */
int states = 0;                     /* searched state counter */

#ifndef __DJGPP__
char * logo = 
    "\x1b[38;5;231m _____ _       \x1b[38;5;225m_____           \x1b[38;5;230m_____        \n"
    "\x1b[38;5;87m|_   _(_)__ \x1b[38;5;8m__\x1b[38;5;213m|_   _|_ _ __ \x1b[38;5;8m__\x1b[38;5;220m|_   _|__  ___ \n"
    "\x1b[38;5;27m  | | | / _|\x1b[38;5;8m___|\x1b[38;5;165m| |/ _` / _|\x1b[38;5;8m___|\x1b[38;5;172m| |/ _ \\/ -_)\n"
    "\x1b[38;5;19m  |_| |_\\__|    \x1b[38;5;90m|_|\\__,_\\__|    \x1b[38;5;94m|_|\\___/\\___|"C_RESET;
#else
char * logo = 
    " _____ _       _____           _____        \n"
    "|_   _(_)__ __|_   _|_ _ __ __|_   _|__  ___ \n"
    "  | | | / _|___|| |/ _` / _|___|| |/ _ \\/ -_)\n"
    "  |_| |_\\__|    |_|\\__,_\\__|    |_|\\___/\\___|";
#endif

#endif
