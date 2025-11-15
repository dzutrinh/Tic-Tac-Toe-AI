/* -------------------------------------------------
 * C3.C: Tic-Tac-Toe MiniMax
 * -------------------------------------------------
 * V0.4
 * by Trinh D.D. Nguyen
 * MIT licensed
 * -------------------------------------------------
 * Building:
 * - Linux/macOS      : Type 'make'
 * - Windows + MinGW  : Type 'mingw32-make'
 * - Windows + Dev-C++: Open 'c3.c' and hit F11
 * - DOS + DJGPP      : Type 'make'
 * -------------------------------------------------
 * Changes:
 * - Removed DOS support
 * - Removed Windows 7 support
 * - Difficulty selection added
 * - Colorful interface is now supported
 * - Pacifier bar added
 * - Alpha-Beta pruning added
 * - Board size is now customizable.
 * - Code refactoring and optimization
 * - Tested utilities added
 * - Game engine optimized for faster runtime
 * - Improved AI move ordering
*/
#include "game.h"

int main() {
    bool keep_playing = true;
    
    while (keep_playing) {
        if (game_init()) {
            game_close(game_play());
            keep_playing = game_ask_continue();
        } else {
            keep_playing = false;
        }
    }
    
    return 0;
}
