/// in caz ca uit sa compilez
/// g++ blackjack.cpp -lncurses -lm -o blackjack

#include <iostream>
#include <cmath>
#include <ncurses.h>
#include <string.h>

using namespace std;

struct {
    char suit[4] = {'H', 'D', 'C', 'S'};
    char rank[14] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K',};
}card;

struct {
    int dealer = 0;
    int player = 0;
}score;

void buildWindow(WINDOW *win)
{
    refresh(); // refresh sa vedem ecranul

    box(win, 0, 0); // facem o cutie in jurul ferestrei
    
    wprintw(win, "TUI BlackJack "); // scriem pe cutie

    //scriem inafara cutii
    mvprintw(1, 63, "Dealer stays on 17 and hits on 16");
    mvprintw(2, 63, "How to play:");
    mvprintw(3, 63, "h - hit");
    mvprintw(4, 63, "s - stay");
    mvprintw(5, 63, "d - double");
    mvprintw(6, 63, "p - split");

    refresh();
    wrefresh(win); // refresh sa vedem cutia  
}

void dealcard(WINDOW *win, int x, int y, int r, int s) {
    mvwprintw(win, y, x, "+--------+");
    mvwprintw(win, y + 1, x, "| %c      |", card.rank[r]);
    mvwprintw(win, y + 2, x, "|        |");
    mvwprintw(win, y + 3, x, "|    %c   |", card.suit[s]);
    mvwprintw(win, y + 4, x, "|        |");
    mvwprintw(win, y + 5, x, "|     %c  |", card.rank[r]);
    mvwprintw(win, y + 6, x, "+--------+");
}

void first2cardsdealer(WINDOW *win){
    int rank = rand() % 13;
    dealcard(win, 1, 1, rank, rand() % 4);
    refresh();
    score.dealer += (rank >= 9) ? 10 : rank + 1; // adunam la score valoare si daca e mai mare ca 9 adunam 10
    wrefresh(win);
    rank = rand() % 13;
    dealcard(win, 12, 1, rank, rand() % 4);
    score.dealer += (rank >= 9) ? 10 : rank + 1;
    refresh();
    wrefresh(win);
}

void first2cardsplayer(WINDOW *win){
    int rank = rand() % 13;
    dealcard(win, 1, 11, rank, rand() % 4);
    score.player += (rank >= 9) ? 10 : rank + 1; // la fel si pt player
    refresh();
    wrefresh(win);
    rank = rand() % 13;
    dealcard(win, 12, 11, rank, rand() % 4);
    score.player += (rank >= 9) ? 10 : rank + 1;
    refresh();
    wrefresh(win);
}
void dealdealer(WINDOW *win, int &x){

    if(score.dealer == 21){
        mvwprintw(win, 0, 20, "  BLACKJACK    ");
        refresh();
        wrefresh(win);
    }
    if (score.dealer > 21) {
        mvwprintw(win, 0, 20, "  DEALER BUST    ");
        refresh();
        wrefresh(win);
    }
    if(score.dealer < 17) {
        x += 11;
        int rank = rand() % 13;
        dealcard(win, x, 1, rank, rand() % 4);
        score.dealer += (rank >= 9) ? 10 : rank + 1;
        refresh();
        wrefresh(win);
        dealdealer(win, x);
    }
}
void dealplayer(WINDOW *win, int &y){
    int rank = rand() % 13;
    dealcard(win, y+11, 11, rank, rand() % 4);
    score.player += (rank >= 9) ? 10 : rank + 1;
    refresh();
    wrefresh(win);
}

int endgame(WINDOW *win, int &x, int &y){
    mvprintw(8, 63, "DEAL AGAIN? [y/n]");
        refresh();
        wrefresh(win);
         char again = wgetch(win);
         if(again == 'y')
         {
            score.dealer = 0;
            score.player = 0;
            werase(win);
            buildWindow(win);
            x = 12;
            y = 12;
            return 1;
         }
         else if (again == 'n') {
            mvprintw(8, 63, "press any key to exit :(("); 
            refresh();
            wrefresh(win);
            return 0;
         }
         return 0;
}

int main(){

    initscr();//mandatoriu 
    noecho();//nu afiseaza ce scriem
    curs_set(0);//ascunde cursorul
    srand(time(0));//seed pt random

    WINDOW *win = newwin(20, 60, 1, 1); // noua fereastra
    buildWindow(win);    
    
    int x = 12, y = 12;
    while (true){
        
        first2cardsdealer(win);
        first2cardsplayer(win);
        char ply = wgetch(win);//player choice
        if(ply == 'h') {
            dealplayer(win, y);
            if(score.player > 21) {
                mvwprintw(win, 0, 20, "  PLAYER BUST    ");
                refresh();
                wrefresh(win);
                if (!endgame(win, x, y)) {
                    break;
                }
            }
            else if (score.player == 21) {
                mvwprintw(win, 0, 20, "  BLACKJACK    ");
                refresh();
                wrefresh(win);
                if (!endgame(win, x, y)) {
                    break;
                }
            }
        }
        else if (ply == 's'){
            dealdealer(win, x);
            if(score.dealer == score.player) {
                mvwprintw(win, 0, 20, "  PUSH    ");
                refresh();
                wrefresh(win);
                if (!endgame(win, x, y)) {
                    break;
                }
            }
            else if(score.dealer > score.player && score.dealer <= 21) {
                mvwprintw(win, 0, 20, "  DEALER WINS    ");
                refresh();
                wrefresh(win);
                if (!endgame(win, x, y)) {
                    break;
                }
            }
            else {
                mvwprintw(win, 0, 20, "  PLAYER WINS    ");
                refresh();
                wrefresh(win);
                if (!endgame(win, x, y)) {
                    break;
                }
            }
        }
        else {
            mvwprintw(win, 0, 20, "  INVALID INPUT    ");
            refresh();
            wrefresh(win);
            if(!endgame(win, x, y)) {
                break;
        }
        }
    }
    getch(); //asteapta o tasta    
    endwin(); //mandatoriu

        return 0;

}