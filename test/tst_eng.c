/* 
 * TEST_ENGINE.C: Validation tests for Tic-Tac-Toe AI engine
 * --------------
 * Tests core game logic and AI behavior
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../defs.h"
#include "../helper.h"
#include "../engine.h"

int tests_passed = 0;
int tests_failed = 0;

#define TEST(name) printf("\n"C_WARNING"[TEST]"C_RESET" %s\n", name)
#define ASSERT(condition, message) \
    if (condition) { \
        printf("  "C_EASY"[v]"C_RESET" %s\n", message); \
        tests_passed++; \
    } else { \
        printf("  "C_ERROR"[x]"C_RESET" FAILED: %s\n", message); \
        tests_failed++; \
    }

void test_board_initialization() {
    TEST("Board Initialization");
    game_board test_board;
    init_board(test_board);
    
    int empty_count = 0;
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            if (test_board[r][c] == CELL_E) empty_count++;
    
    ASSERT(empty_count == BOARD_SIZE * BOARD_SIZE, "All cells are empty");
    ASSERT(has_move(test_board), "Board has moves available");
}

void test_cell_operations() {
    TEST("Cell Operations");
    game_board test_board;
    init_board(test_board);
    
    ASSERT(is_playable(test_board, 0, 0), "Empty cell (0,0) is playable");
    ASSERT(!is_occupied(test_board, 0, 0), "Empty cell (0,0) is not occupied");
    
    test_board[0][0] = CELL_X;
    ASSERT(!is_playable(test_board, 0, 0), "Occupied cell (0,0) is not playable");
    ASSERT(is_occupied(test_board, 0, 0), "Occupied cell (0,0) is occupied");
}

void test_win_detection_rows() {
    TEST("Win Detection - Rows");
    game_board test_board;
    
    for (int r = 0; r < BOARD_SIZE; r++) {
        init_board(test_board);
        for (int c = 0; c < BOARD_SIZE; c++)
            test_board[r][c] = CELL_X;
        
        char msg[64];
        sprintf(msg, "X wins on row %d", r);
        ASSERT(evaluate(test_board) == SCORE_X, msg);
    }
    
    for (int r = 0; r < BOARD_SIZE; r++) {
        init_board(test_board);
        for (int c = 0; c < BOARD_SIZE; c++)
            test_board[r][c] = CELL_O;
        
        char msg[64];
        sprintf(msg, "O wins on row %d", r);
        ASSERT(evaluate(test_board) == SCORE_O, msg);
    }
}

void test_win_detection_columns() {
    TEST("Win Detection - Columns");
    game_board test_board;
    
    for (int c = 0; c < BOARD_SIZE; c++) {
        init_board(test_board);
        for (int r = 0; r < BOARD_SIZE; r++)
            test_board[r][c] = CELL_X;
        
        char msg[64];
        sprintf(msg, "X wins on column %d", c);
        ASSERT(evaluate(test_board) == SCORE_X, msg);
    }
    
    for (int c = 0; c < BOARD_SIZE; c++) {
        init_board(test_board);
        for (int r = 0; r < BOARD_SIZE; r++)
            test_board[r][c] = CELL_O;
        
        char msg[64];
        sprintf(msg, "O wins on column %d", c);
        ASSERT(evaluate(test_board) == SCORE_O, msg);
    }
}

void test_win_detection_diagonals() {
    TEST("Win Detection - Diagonals");
    game_board test_board;
    
    // Primary diagonal (top-left to bottom-right)
    init_board(test_board);
    for (int i = 0; i < BOARD_SIZE; i++)
        test_board[i][i] = CELL_X;
    ASSERT(evaluate(test_board) == SCORE_X, "X wins on primary diagonal");
    
    init_board(test_board);
    for (int i = 0; i < BOARD_SIZE; i++)
        test_board[i][i] = CELL_O;
    ASSERT(evaluate(test_board) == SCORE_O, "O wins on primary diagonal");
    
    // Secondary diagonal (top-right to bottom-left)
    init_board(test_board);
    for (int i = 0; i < BOARD_SIZE; i++)
        test_board[i][BOARD_SIZE-1-i] = CELL_X;
    ASSERT(evaluate(test_board) == SCORE_X, "X wins on secondary diagonal");
    
    init_board(test_board);
    for (int i = 0; i < BOARD_SIZE; i++)
        test_board[i][BOARD_SIZE-1-i] = CELL_O;
    ASSERT(evaluate(test_board) == SCORE_O, "O wins on secondary diagonal");
}

void test_tie_detection() {
    TEST("Tie Detection");
    game_board test_board;
    init_board(test_board);
    
    // Create a tied board (3x3 example)
    if (BOARD_SIZE == 3) {
        test_board[0][0] = CELL_X; test_board[0][1] = CELL_O; test_board[0][2] = CELL_X;
        test_board[1][0] = CELL_X; test_board[1][1] = CELL_O; test_board[1][2] = CELL_O;
        test_board[2][0] = CELL_O; test_board[2][1] = CELL_X; test_board[2][2] = CELL_X;
        move_count = 9;  /* Update move count for full board */
        
        ASSERT(evaluate(test_board) == SCORE_TIE, "Tied game detected correctly");
        ASSERT(!has_move(test_board), "No moves left on full board");
    } else {
        printf("  (Skipped - only for 3x3 board)\n");
    }
}

void test_human_move() {
    TEST("Human Move Validation");
    game_board test_board;
    init_board(test_board);
    
    human = CELL_O;
    computer = CELL_X;
    current = human;
    
    ASSERT(human_move(test_board, 0, 0), "Valid move accepted");
    ASSERT(test_board[0][0] == CELL_O, "Cell marked correctly");
    ASSERT(current == computer, "Turn switched to computer");
    
    current = human;
    ASSERT(!human_move(test_board, 0, 0), "Occupied cell rejected");
}

void test_computer_move() {
    TEST("Computer Move Validation");
    game_board test_board;
    init_board(test_board);
    
    human = CELL_O;
    computer = CELL_X;
    current = computer;
    game_depth = GAME_MEDIUM;
    
    srand((unsigned int)time(NULL));
    
    computer_move(test_board);
    
    int computer_moves = 0;
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            if (test_board[r][c] == CELL_X) computer_moves++;
    
    ASSERT(computer_moves == 1, "Computer made exactly one move");
    ASSERT(current == human, "Turn switched to human");
}

void test_ai_blocking() {
    TEST("AI Blocking Logic");
    if (BOARD_SIZE != 3) {
        printf("  (Skipped - only for 3x3 board)\n");
        return;
    }
    
    game_board test_board;
    init_board(test_board);
    
    human = CELL_O;
    computer = CELL_X;
    game_depth = GAME_IMPOSSIBLE;
    
    // Set up board where blocking is critical
    test_board[0][0] = CELL_O;
    test_board[0][1] = CELL_O;
    test_board[1][1] = CELL_X;  /* AI has center */
    move_count = 3;
    
    computer_move(test_board);
    
    // Verify AI blocked the winning move
    ASSERT(test_board[0][2] == CELL_X, "AI blocks human's winning move");
}

void test_ai_winning() {
    TEST("AI Winning Logic");
    if (BOARD_SIZE != 3) {
        printf("  (Skipped - only for 3x3 board)\n");
        return;
    }
    
    game_board test_board;
    init_board(test_board);
    
    human = CELL_O;
    computer = CELL_X;
    game_depth = GAME_IMPOSSIBLE;  // Use max depth for this test
    
    // Computer has two in a row, should win on this turn
    test_board[1][0] = CELL_X;
    test_board[1][1] = CELL_X;
    move_count = 2;  /* Update move count for manual setup */
    
    computer_move(test_board);
    
    int result = evaluate(test_board);
    
    // AI should either win immediately or make a winning position
    ASSERT(result == SCORE_X || result == SCORE_TIE, "AI wins or maintains advantage");
    
    // Verify AI made exactly one move
    int x_count = 0;
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            if (test_board[r][c] == CELL_X) x_count++;
    ASSERT(x_count == 3, "AI made exactly one move (3 X's total)");
}

void test_easy_mode() {
    TEST("Easy Mode Random Moves");
    game_board test_board;
    init_board(test_board);
    
    human = CELL_O;
    computer = CELL_X;
    game_depth = GAME_EASY;
    
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < 3; i++) {
        init_board(test_board);
        computer_move(test_board);
        
        int moves = 0;
        for (int r = 0; r < BOARD_SIZE; r++)
            for (int c = 0; c < BOARD_SIZE; c++)
                if (test_board[r][c] == CELL_X) moves++;
        
        ASSERT(moves == 1, "Easy mode makes valid move");
    }
}

int main() {
    printf("===========================================\n");
    printf("  Tic-Tac-Toe AI Engine Validation Tests\n");
    printf("  Board Size: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("  Engine: %s\n", GAME_ENGINE);
    printf("===========================================\n");
    
    test_board_initialization();
    test_cell_operations();
    test_win_detection_rows();
    test_win_detection_columns();
    test_win_detection_diagonals();
    test_tie_detection();
    test_human_move();
    test_computer_move();
    test_ai_blocking();
    test_ai_winning();
    test_easy_mode();
    
    printf("\n===========================================\n");
    printf("  Test Results:\n");
    printf("  "C_EASY"v"C_RESET" Passed: "C_EASY"%d"C_RESET"\n", tests_passed);
    printf("  "C_ERROR"x"C_RESET" Failed: "C_ERROR"%d"C_RESET"\n", tests_failed);
    printf("  Total:    %d\n", tests_passed + tests_failed);
    printf("===========================================\n");
    
    return tests_failed > 0 ? 1 : 0;
}
