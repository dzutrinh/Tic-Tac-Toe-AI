#ifndef _TICTACTOE_MINIMAX_HELPERS_H_
#define _TICTACTOE_MINIMAX_HELPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

static void clear() {
    puts("\x1b[2J\x1b[H");
}

static int mini(int a, int b) {
    return a < b ? a : b;
}

static int maxi(int a, int b) {
    return a > b ? a : b;
}

static void show_progress() {
    fputs(C_BRIGHT"Computer"C_NORMAL" is "C_ERROR"thinking"C_NORMAL": ", stdout);
    fputs(progress[progress_current], stdout);
    fflush(stdout);
    progress_current+=progress_dir;
    if (progress_current <= 0 || progress_current >= progress_total-1)
        progress_dir *= -1;
}

#endif