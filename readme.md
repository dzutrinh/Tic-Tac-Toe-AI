## Tic-Tac-Toe - AI
A simple Tic-Tac-Toe game between Human and Computer. Simply an illustration of the MiniMax algorithm. It's just for fun during the pandemic.

Updates: 
- The game interface got a major touch-up.
- AI difficulty levels added.
- Game board size can be changed via the symbol `BOARD_SIZE` in the file `defs.h`. The default value is `3`.
- Alpha-Beta pruning strategy added. The strategy can be disabled by undefine the `_USE_ALPHA_BETA_PRUNE_` symbol also in the header file `defs.h`.

## Compiling
* GCC: type `make`
* MinGW: type `mingw32-make`

## Tested
- Clang (macOS Monterey)
- MinGW64 (Windows)

## Notes
- DOS support is now temporarily removed.
- Linux is not tested, I hope it would work :)

## Screenshot
![Title screen](screens/screen01.png)
![Gameplay screen](screens/screen02.png)
