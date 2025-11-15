## Tic-Tac-Toe - AI
A simple Tic-Tac-Toe game between Human and Computer. Simply an illustration of the MiniMax algorithm. It's just for fun during the pandemic.

Updates: 
- The game interface got a major touch-up.
- AI difficulty levels added.
- Game board size can be changed via the symbol `BOARD_SIZE` in the file `defs.h`. The default value is `3`.
- Alpha-Beta pruning strategy added. The strategy can be disabled by undefine the `_USE_ALPHA_BETA_PRUNE_` symbol also in the header file `defs.h`.
- Several optimizations and code refactoring have been done to improve the game engine performance.

## Compiling
* GCC: type `make`
* MinGW: type `mingw32-make`
* DJGPP (DOS): type `makedos.bat`

## Tested
- Clang (macOS Tahoe)
- MinGW64 (Windows)

## Notes
- DOS support is now supported again.
- Linux is not tested, I hope it would work :)
- This is just a basic version of the game. Feels free to fork and modify anyway you need.
- Tests are included in the `test` folder. To build the tests, type `make test`.

## Screenshot
![Title screen](screens/screen01.png)
![Gameplay screen](screens/screen02.png)
