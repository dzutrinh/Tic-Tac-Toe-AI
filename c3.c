/* -------------------------------------------------
 * C3.C: Tic-Tac-Toe MiniMax
 * -------------------------------------------------
 * V0.2
 * by Trinh D.D. Nguyen
 * MIT licensed
 * -------------------------------------------------
 * Building:
 * - Linux/macOS      : Type 'make'
 * - Windows + MinGW  : Type 'mingw32-make'
 * - Windows + Dev-C++: Open 'c3.c' and hit F11
 * -------------------------------------------------
 * Changes:
 * - Removed DOS support
 * - Removed Windows 7 support
 * - Difficulty selection added
 * - Colorful interface is now supported
 * - Pacifier bar added
*/
#include "game.h"

int main() {  
    if (game_init()) game_close(game_play());
    return 0;
}
