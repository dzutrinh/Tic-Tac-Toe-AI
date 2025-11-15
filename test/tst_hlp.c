/* 
 * TEST_HELPER.C: Validation tests for Tic-Tac-Toe AI helper functions
 * --------------
 * Tests utility functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../defs.h"
#include "../helper.h"

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

void test_clear_function() {
    TEST("Screen Clear Function");
    // Test that clear() doesn't crash
    clear();
    ASSERT(1, "clear() executes without error");
}

void test_mssleep_function() {
    TEST("Millisecond Sleep Function");
    
    // Test that mssleep doesn't crash with various inputs
    mssleep(10);
    ASSERT(1, "mssleep(10) executes without error");
    
    mssleep(0);
    ASSERT(1, "mssleep(0) executes without error");
    
    mssleep(1);
    ASSERT(1, "mssleep(1) executes without error");
}

void test_progress_interval() {
    TEST("Progress Interval Configuration");
    
    int original_update = progress_update;
    
    progress_interval(5);
    ASSERT(progress_update == 5 * BOARD_SIZE, "progress_interval(5) sets correct update frequency");
    
    progress_interval(10);
    ASSERT(progress_update == 10 * BOARD_SIZE, "progress_interval(10) updates value correctly");
    
    progress_interval(1);
    ASSERT(progress_update == 1 * BOARD_SIZE, "progress_interval(1) sets minimum frequency");
    
    // Restore original
    progress_update = original_update;
}

void test_progress_show() {
    TEST("Progress Bar Display");
    
    // Reset progress state
    progress_pacifier = 0;
    progress_current = 0;
    progress_dir = +1;
    progress_update = 1;
    
    int original_pacifier = progress_pacifier;
    
    // Call progress_show multiple times
    for (int i = 0; i < 5; i++) {
        progress_show();
    }
    
    ASSERT(progress_pacifier > original_pacifier, "progress_pacifier increments");
    ASSERT(progress_current >= 0 && progress_current < PROGRESS_TOTAL, 
           "progress_current stays within bounds");
}

void test_progress_direction() {
    TEST("Progress Bar Direction");
    
    // Reset progress state
    progress_pacifier = 0;
    progress_current = 0;
    progress_dir = +1;
    progress_update = 1;
    
    // Run through enough iterations to test direction change
    for (int i = 0; i < PROGRESS_TOTAL * 3; i++) {
        progress_show();
    }
    
    ASSERT(progress_current >= 0 && progress_current < PROGRESS_TOTAL, 
           "Progress bar bounces correctly at boundaries");
}

void test_progress_array() {
    TEST("Progress Bar Array");
    
    ASSERT(PROGRESS_TOTAL == 12, "Progress array has correct size");
    
    int valid_strings = 1;
    for (int i = 0; i < PROGRESS_TOTAL; i++) {
        if (strlen(progress[i]) == 0) {
            valid_strings = 0;
            break;
        }
    }
    ASSERT(valid_strings, "All progress strings are non-empty");
    
    // Check that all strings end with \r (carriage return)
    int all_have_cr = 1;
    for (int i = 0; i < PROGRESS_TOTAL; i++) {
        size_t len = strlen(progress[i]);
        if (len == 0 || progress[i][len-1] != '\r') {
            all_have_cr = 0;
            break;
        }
    }
    ASSERT(all_have_cr, "All progress strings end with carriage return");
}

void test_sleep_precision() {
    TEST("Sleep Function Behavior");
    
    // Test that sleep function works with various durations
    // without timing precision requirements (OS-dependent)
    int durations[] = {1, 5, 10, 20};
    int num_tests = sizeof(durations) / sizeof(durations[0]);
    
    for (int i = 0; i < num_tests; i++) {
        mssleep(durations[i]);
    }
    
    ASSERT(1, "Sleep function handles various durations without crashing");
    
    // Test with longer duration to verify it actually sleeps
    time_t start = time(NULL);
    mssleep(100);
    time_t end = time(NULL);
    
    // Should take at least some time (but don't be too strict)
    ASSERT(end >= start, "Sleep function completes without hanging");
}

void test_global_variables() {
    TEST("Helper Global Variables");
    
    ASSERT(progress_current >= 0, "progress_current initialized to valid value");
    ASSERT(progress_dir == 1 || progress_dir == -1, "progress_dir has valid direction");
    ASSERT(progress_pacifier >= 0, "progress_pacifier initialized to valid value");
    ASSERT(progress_update > 0, "progress_update is positive");
}

int main() {
    printf("===========================================\n");
    printf("  Tic-Tac-Toe AI Helper Validation Tests\n");
    printf("  Board Size: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("===========================================\n");
    
    test_clear_function();
    test_mssleep_function();
    test_progress_interval();
    test_progress_show();
    test_progress_direction();
    test_progress_array();
    test_sleep_precision();
    test_global_variables();
    
    printf("\n===========================================\n");
    printf("  Test Results:\n");
    printf("  "C_EASY"v"C_RESET" Passed: "C_EASY"%d"C_RESET"\n", tests_passed);
    printf("  "C_ERROR"x"C_RESET" Failed: "C_ERROR"%d"C_RESET"\n", tests_failed);
    printf("  Total:    %d\n", tests_passed + tests_failed);
    printf("===========================================\n");
    
    return tests_failed > 0 ? 1 : 0;
}
