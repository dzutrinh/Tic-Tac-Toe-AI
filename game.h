#ifndef _TICTACTOE_MINIMAX_GAME_H_
#define _TICTACTOE_MINIMAX_GAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include "defs.h"
#include "helper.h"
#include "engine.h"

/* =============== PROTOTYPES ==================== */
bool game_init();

int game_play();

void game_close(int result);
/* =============================================== */

#ifdef _WIN32
	static 	DWORD mode = 0;						
	static	HANDLE hConsole = NULL;
#endif

void  game_logo() {
    clear();
    puts(logo);
    printf( C_RESET"%42s"C_WARNING"%d"C_RESET"."C_WARNING"%d"C_RESET"\n", 
            "V", GAME_VERSION & 0x00FF, (GAME_VERSION >> 8));
}

bool game_init() {

#ifdef _WIN32	
    /* enable Virtual Terminal on Windows platforms to use ANSI sequences */
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hConsole, &mode);
	SetConsoleMode(hConsole, mode | 0x0004);
#endif

    char choice;
    game_logo();
    printf( "Select "C_BRIGHT"difficulty"C_RESET" level:\n"
            "  "C_EASY"E"C_RESET"asy\n"
            "  "C_MEDIUM"M"C_RESET"edium\n"
            "  "C_HARD"H"C_RESET"ard\n"
            "  "C_IMPOSSIBLE"I"C_RESET"mpossible\n"
            C_DARK"  -------------"C_RESET"\n"
            "  Nah, I "C_O"q"C_RESET"uit\n"
            C_DARK"  -------------"C_RESET"\n"
            "Your choice: ");
    scanf("%c", &choice);
    choice = toupper(choice);
    switch (choice) {
    case 'E': game_depth = GAME_EASY; break;
    case 'M': game_depth = GAME_MEDIUM; break;
    case 'H': game_depth = GAME_HARD; break;
    case 'I': game_depth = GAME_IMPOSSIBLE; break;
    case 'Q': return false;
    }
    return true;
}

int game_play() {
    bool quit = false;                  /* quit flag */
    int input, c, r, eval;
    current = human;                    /* human moves first */

    init_board(board);
    do {
        game_logo();
        show_board(board, false);       /* draw game board */
        if (has_move(board)) {          /* if the board is playable */
            do {                        /* get user input as index */
                printf("Searched states: "C_THINKING"%-6d"C_RESET"\n", states);
                printf(C_BRIGHT"Human "C_RESET"="C_O" %c"C_RESET" - "
                    C_BRIGHT"Computer "C_RESET"="C_X" %c\n", human, computer);
                printf(C_BRIGHT"Your move "C_DARK"("C_ERROR"-1"C_BRIGHT" = "C_WARNING"quit"C_DARK"): ");
                scanf("%d", &input);
                if (input == -1) {
                    quit = true;
                    break;
                }
                r = input / BOARD_SIZE; /* convert to row and col */
                c = input % BOARD_SIZE; /* and make the move if possible */
            } while(!human_move(board, c, r));
        }
        else quit = true;               /* no more cell to play */

        if (!quit) {                    /* if human placed a move */
            computer_move(board);       /* now to the computer's turn */
            eval = evaluate(board);     /* evaluate the board */
            switch (eval) {
            case SCORE_X: quit = true; break;
            case SCORE_O: quit = true; break;
            }
        }
    } while(!quit);
    return eval;
}

void game_close(int result) {
    game_logo();
    show_board(board, true);
    switch (result) {
    case SCORE_X: printf(C_X"X"C_WARNING" WINS!"C_RESET"\n"); break;
    case SCORE_O: printf(C_O"O"C_WARNING" WINS!"C_RESET"\n"); break;
    case SCORE_TIE: printf(C_WARNING"GAME TIES!"C_RESET"\n"); break;
    }
    puts(C_THINKING"Thanks for playing"C_RESET"!");

#ifdef _WIN32
	SetConsoleMode(hConsole, mode);	    /* restore previous CMD mode */
#endif
}

#endif