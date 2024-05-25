/* -------------------------------------------------
 * Tic-Tac-Toe MiniMax
 * -------------------------------------------------
 * V0.2
 * by Trinh D.D. Nguyen
 * MIT licensed
 * -------------------------------------------------
 * Building:
 * - Linux/macOS      : Type 'make'
 * - Windows + MinGW  : Type 'mingw32-make'
 * - Windows + Dev-C++: Open C3.C and hit F11
 * -------------------------------------------------
 * Changes:
 * - Remove DOS support
 * - Difficulty selection added
 * - Colorful display is now supported
*/
#include "game.h"

int main() {  
    if (game_init())
        game_close(game_play());
    return 0;
}
