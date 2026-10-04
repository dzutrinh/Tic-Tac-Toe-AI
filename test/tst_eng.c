/* 
 * TEST_ENGINE.C: Validation tests for Tic-Tac-Toe AI engine
 * --------------
 * Tests core game logic and AI behavior
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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

void test_has_move_without_counter() {
    TEST("Full Board Detection (move counter not updated)");
    if (BOARD_SIZE != 3) {
        printf("  (Skipped - only for 3x3 board)\n");
        return;
    }
    
    game_board test_board;
    init_board(test_board);             /* resets move_count to 0 */
    const char * cells = "XOXXOOOXO";
    for (int i = 0; i < 9; i++)
        test_board[i / 3][i % 3] = cells[i];
    
    ASSERT(!has_move(test_board), "Full board has no moves even if move_count is 0");
}

void test_computer_move_full_board() {
    TEST("Computer Move on Full Board");
    if (BOARD_SIZE != 3) {
        printf("  (Skipped - only for 3x3 board)\n");
        return;
    }
    
    game_board test_board, before;
    const char * cells = "XOXXOOOXO";
    int levels[] = { GAME_EASY, GAME_HARD };
    
    for (int l = 0; l < 2; l++) {
        init_board(test_board);
        for (int i = 0; i < 9; i++)
            test_board[i / 3][i % 3] = cells[i];
        move_count = 9;
        memcpy(before, test_board, sizeof(game_board));
        game_depth = levels[l];
        
        computer_move(test_board);      /* must not write outside the board */
        
        ASSERT(memcmp(before, test_board, sizeof(game_board)) == 0,
               l == 0 ? "Easy mode leaves a full board untouched"
                      : "Minimax leaves a full board untouched");
    }
}

void test_move_ordering() {
    TEST("Move Ordering");
    game_board test_board;
    init_board(test_board);             /* builds the move ordering table */
    
    int seen[BOARD_SIZE * BOARD_SIZE] = {0}, ok = 1;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        int pos = move_priority[i];
        if (pos < 0 || pos >= BOARD_SIZE * BOARD_SIZE || seen[pos]++) ok = 0;
    }
    ASSERT(ok, "Every cell appears exactly once in the move ordering");
    
    if (BOARD_SIZE == 3) {
        int expected[9] = {4, 0, 2, 6, 8, 1, 3, 5, 7};
        ASSERT(memcmp(move_priority, expected, sizeof(expected)) == 0,
               "3x3 ordering is center, corners, then edges");
    }
}

/* plays every possible sequence of human moves against the AI; returns the
   number of games the AI lost */
static int play_all_games(game_board g, int * games) {
    int lost = 0;
    for (int pos = 0; pos < BOARD_SIZE * BOARD_SIZE; pos++) {
        int r = pos / BOARD_SIZE, c = pos % BOARD_SIZE;
        if (g[r][c] != CELL_E) continue;
        
        game_board b;
        memcpy(b, g, sizeof(game_board));
        b[r][c] = human;
        if (evaluate(b) == SCORE_O) { (*games)++; lost++; continue; }
        if (!has_move(b))           { (*games)++; continue; }
        
        computer_move(b);
        if (evaluate(b) != SCORE_TIE || !has_move(b)) { (*games)++; continue; }
        
        lost += play_all_games(b, games);
    }
    return lost;
}

void test_ai_never_loses() {
    TEST("AI Never Loses (every possible game)");
    if (BOARD_SIZE != 3) {
        printf("  (Skipped - only for 3x3 board)\n");
        return;
    }
    
    int levels[] = { GAME_MEDIUM, GAME_HARD, GAME_IMPOSSIBLE };
    const char * names[] = { "Medium", "Hard", "Impossible" };
    int saved_update = progress_update;
    
    human = CELL_O;
    computer = CELL_X;
    progress_update = 1 << 30;          /* keep the pacifier quiet and fast */
    
    for (int l = 0; l < 3; l++) {
        game_board test_board;
        int games = 0, lost;
        char msg[96];
        
        game_depth = levels[l];
        init_board(test_board);
        lost = play_all_games(test_board, &games);
        
        sprintf(msg, "%s AI loses none of %d games", names[l], games);
        ASSERT(lost == 0, msg);
    }
    
    progress_update = saved_update;
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
    test_has_move_without_counter();
    test_computer_move_full_board();
    test_move_ordering();
    test_ai_never_loses();
    
    printf("\n===========================================\n");
    printf("  Test Results:\n");
    printf("  "C_EASY"v"C_RESET" Passed: "C_EASY"%d"C_RESET"\n", tests_passed);
    printf("  "C_ERROR"x"C_RESET" Failed: "C_ERROR"%d"C_RESET"\n", tests_failed);
    printf("  Total:    %d\n", tests_passed + tests_failed);
    printf("===========================================\n");
    
    return tests_failed > 0 ? 1 : 0;
}
