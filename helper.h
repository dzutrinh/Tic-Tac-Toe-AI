#ifndef _TICTACTOE_MINIMAX_HELPERS_H_
#define _TICTACTOE_MINIMAX_HELPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* =============== PROTOTYPES ==================== */

void clear();

void show_progress();

/* =============================================== */

char progress[10][64] = {
    C_MEDIUM"["C_THINKING"##         "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING" ##        "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING"  ##       "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING"   ##      "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING"    ##     "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING"     ##    "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING"      ##   "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING"       ##  "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING"        ## "C_MEDIUM"]\r",
    C_MEDIUM"["C_THINKING"         ##"C_MEDIUM"]\r",
};
int progress_total = 10;
int progress_current = 0;
int progress_dir = +1;

void clear() {
    puts("\x1b[2J\x1b[H");
}

void show_progress() {
    fputs(C_BRIGHT"Computer"C_NORMAL" is "C_ERROR"thinking"C_NORMAL": ", stdout);
    fputs(progress[progress_current], stdout);
    fflush(stdout);
    progress_current+=progress_dir;
    if (progress_current <= 0 || progress_current >= progress_total-1)
        progress_dir *= -1;
}

#endif