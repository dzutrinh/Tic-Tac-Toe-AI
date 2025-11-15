/* 
 * GAME.H: Tic-Tac-Toe AI Game Engine
 * -------
 * Coded by Trinh D.D. Nguyen
 * Last updates: Nov, 2025
 */
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

bool game_ask_continue();
/* =============================================== */

#ifdef _WIN32
	static 	DWORD mode = 0;						
	static	HANDLE hConsole = NULL;
#endif

void  game_logo() {
    clear();
    puts(logo);
    printf( "%32s"C_ERROR"%d"C_RESET"."C_ERROR"%d"C_RESET" ["C_MISC"%s"C_RESET"]\n",            
            "V", GAME_VERSION & 0x00FF, (GAME_VERSION >> 8), GAME_ENGINE);
}

bool game_init() {

#ifdef _WIN32	
    /* enable Virtual Terminal on Windows platforms to use ANSI sequences */
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hConsole, &mode);
	SetConsoleMode(hConsole, mode | 0x0004);
#endif

    /* seed random number generator for easy mode */
    srand((unsigned int)time(NULL));

    char choice;
    int valid = 0;
    int c;
    
    do {
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
        
        if (scanf(" %c", &choice) != 1) {
            printf(C_ERROR"Invalid input! Please try again.\n"C_RESET);
            mssleep(1000);
            /* clear bad input */
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        /* consume newline */
        while ((c = getchar()) != '\n' && c != EOF);
        
        choice = toupper(choice);
        switch (choice) {
        case 'E': 
            game_depth = GAME_EASY; 
            progress_interval(2); 
            valid = 1;
            break;
        case 'M': 
            game_depth = GAME_MEDIUM; 
            progress_interval(5); 
            valid = 1;
            break;
        case 'H': 
            game_depth = GAME_HARD; 
            progress_interval(50); 
            valid = 1;
            break;
        case 'I': 
            game_depth = GAME_IMPOSSIBLE; 
            progress_interval(100); 
            valid = 1;
            break;
        case 'Q': 
            return false;
        default:
            printf(C_ERROR"Invalid choice! Please select E, M, H, I, or Q.\n"C_RESET);
            mssleep(1000);
            valid = 0;
        }
    } while (!valid);
    
    return true;
}

int game_play() {
    bool quit = false;                  /* quit flag */
    bool valid;
    int input, c, r, eval, range;
    int scan_result, ch;
    
    current = human;                    /* human moves first */
    range = (BOARD_SIZE * BOARD_SIZE)-1;
    init_board(board);
    do {
        game_logo();
        show_board(board, false);       /* draw game board */
        if (has_move(board)) {          /* if the board is playable */
            do {                        /* get user input as index */
                printf("Moves explored: ["C_THINKING"%-6d"C_RESET"]\n", states);
                printf(C_BRIGHT"Human "C_RESET"["C_O"%c"C_RESET"] - "
                       C_BRIGHT"Computer "C_RESET"["C_X"%c"C_RESET"]\n", human, computer);
                valid = false;
                
                while (!valid) {
                    printf(C_BRIGHT"Your move "C_DARK"["C_WARNING"%d"C_DARK"-"C_WARNING"%d"C_DARK"] ("C_ERROR"-1"C_BRIGHT" = "C_WARNING"quit"C_DARK"): ", 0, range);
                    
                    scan_result = scanf("%d", &input);
                    
                    /* handle invalid input (non-numeric) */
                    if (scan_result != 1) {
                        printf(C_ERROR"Invalid! Enter a number: "C_RESET);
                        /* clear bad input */
                        while ((ch = getchar()) != '\n' && ch != EOF);
                        continue;
                    }
                    
                    /* consume newline */
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    
                    /* validate range */
                    if (input < -1 || input > range) {
                        printf(C_ERROR"Out of range [%d-%d]: "C_RESET, 0, range);
                        continue;
                    }
                    
                    valid = true;
                }
                
                if (input == -1) {
                    quit = true;
                    break;
                }
                
                r = input / BOARD_SIZE; /* convert to row and col */
                c = input % BOARD_SIZE; /* and make the move if possible */
                
                /* check if cell is already occupied */
                if (is_occupied(board, c, r)) {
                    printf(C_ERROR"Cell occupied! Try another.\n"C_RESET);
                    mssleep(800);
                } else {
                    human_move(board, c, r);
                    break;
                }
            } while(true);
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
}

bool game_ask_continue() {
    char choice;
    int c;
    
    printf("\n");
    printf(C_BRIGHT"Play again? "C_RESET"("C_EASY"Y"C_RESET"/"C_ERROR"N"C_RESET"): ");
    
    if (scanf(" %c", &choice) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        return false;
    }
    
    /* consume newline */
    while ((c = getchar()) != '\n' && c != EOF);
    
    choice = toupper(choice);
    
    if (choice == 'N') {
        puts(C_THINKING"Thanks for playing"C_RESET"!");
        
#ifdef _WIN32
        SetConsoleMode(hConsole, mode);     /* restore previous CMD mode */
#endif
        return false;
    }
    
    return true;
}

#endif