## TUI-INFINITE-BlackJack ♠️♥️

BlackJack or 21 cards game with text user interface to be played in the terminal, made completly in c++ with thee `ncurses` library.
---------------------------------------------------------------------------------------
<img src="img.png">
---------------------------------------------------------------------------------------

## Dependences
 - `lncurses` c/c++ library
 -  compiler suited for c++ (I reccomend g++)
 -  to be openned in terminal

## How to play
**On linux/macOS**  
 - `git clone` the repo or install the zip
 - compile the `.cpp` file ( `g++ blackjack.cpp -o blackjack -lncurses ` )
 - `./blackjack`
 -  h - hit
 -  s - stay
 -  q - quit
 -  y/n for another hand 
 
-------------------------------------------

 **On Android** 
 - install Termux from the play store and inside paste:
 - `pkg update && pkg upgrade && pkg install clang`
 -  ` clang++ main.cpp -o program && ./program`

----------------------------------------------------

I am still working on the game so there is no *double down* or *split* and the UI may bug rarely


