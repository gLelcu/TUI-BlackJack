
/// a se compila cu:  `g++ blackjack.cpp -lncurses -lm -o blackjack`

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
    mvprintw(7, 63, "q - quit");

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

void first2cardsdealer(WINDOW *win){ //primele 2 carti si calculam scorul
    int rank = rand() % 13;
    dealcard(win, 1, 1, rank, rand() % 4);
    if(rank == 0)
            if(score.dealer + 11 > 21)
                score.dealer += (rank >= 9) ? 10 : rank + 1;
            else   score.dealer += 11; //daca scorul cu 11 depaseste 21 asul e 1 si viceversa
    else
        score.dealer += (rank >= 9) ? 10 : rank + 1;  // adunam la scor valoarea cartii si daca e mai mare ca 9 adunam 10
        
    refresh();
    wrefresh(win);
    rank = rand() % 13;
    dealcard(win, 12, 1, rank, rand() % 4);
    if(rank == 0)
            if(score.dealer + 11 > 21)
                score.dealer += (rank >= 9) ? 10 : rank + 1;
            else score.dealer += 11;
    else
        score.dealer += (rank >= 9) ? 10 : rank + 1;
    refresh();
    wrefresh(win);
}

void first2cardsplayer(WINDOW *win){ // facem acelasi lucru
    int rank = rand() % 13;
    dealcard(win, 1, 11, rank, rand() % 4);
    if(rank == 0)
            if(score.player + 11 > 21)
                score.player += (rank >= 9) ? 10 : rank + 1;
            else   score.player += 11;
    else
        score.player += (rank >= 9) ? 10 : rank + 1;
    refresh();
    wrefresh(win);
    rank = rand() % 13;
    dealcard(win, 12, 11, rank, rand() % 4);
    if(rank == 0)
            if(score.player + 11 > 21)
                score.player += (rank >= 9) ? 10 : rank + 1;
            else   score.player += 11;
    else
        score.player += (rank >= 9) ? 10 : rank + 1;
    refresh();
    wrefresh(win);
}
void dealdealer(WINDOW *win, int &x){

    if(score.dealer == 21){
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, 0, 20, "  BLACKJACK    ");
        wattroff(win, COLOR_PAIR(1));
        refresh();
        wrefresh(win);
    }
    else if (score.dealer > 21) {
        mvwprintw(win, 0, 20, "  DEALER BUST    ");
        refresh();
        wrefresh(win);
    }
    else if(score.dealer < 17) {
        x += 11;
        int rank = rand() % 13;
        dealcard(win, x, 1, rank, rand() % 4);
        if(rank == 0)
            if(score.dealer + 11 > 21)
                score.dealer += (rank >= 9) ? 10 : rank + 1;
            else   score.dealer += 11;
        else
            score.dealer += (rank >= 9) ? 10 : rank + 1;
        refresh();
        wrefresh(win);
        dealdealer(win, x);
    }
}
void dealplayer(WINDOW *win, int &y){
    int rank = rand() % 13;
     y+=11;
    dealcard(win, y, 11, rank, rand() % 4);
    if(rank == 0)
            if(score.player + 11 > 21)
                score.player += (rank >= 9) ? 10 : rank + 1;
            else   score.player += 11;
    else
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
            first2cardsdealer(win);
            first2cardsplayer(win);
            mvprintw(8, 63, "ANOTHER ONE!     "); 
            refresh();
            wrefresh(win);
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

bool checkscore(WINDOW *win, bool finalCompare){
    if(score.player > 21){
        mvwprintw(win, 0, 20, "  PLAYER BUST, DEALER WINS    ");
        refresh();
        wrefresh(win);
        return true;
    }
    if(score.dealer > 21){
        mvwprintw(win, 0, 20, "  PLAYER WINS, DEALER BUST    ");
        refresh();
        wrefresh(win);
        return true;
    }

    if(!finalCompare){
        if(score.player == 21){
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, 0, 20, "  BLACKJACK    ");
            wattroff(win, COLOR_PAIR(1));
            refresh();
            wrefresh(win);
            return true;
        }
        return false;
    }

    if(score.player == score.dealer){
        mvwprintw(win, 0, 20, "  PUSH    ");
    }
    else if(score.dealer > score.player){
        mvwprintw(win, 0, 20, "  DEALER WINS    ");
    }
    else {
        mvwprintw(win, 0, 20, "  PLAYER WINS    ");
    }

    refresh();
    wrefresh(win);
    return true;
}

int main(){

    initscr();//mandatoriu 
    noecho();//nu afiseaza ce scriem
    curs_set(0);//ascunde cursorul
    srand(time(0));//seed pt random
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1);

    WINDOW *win = newwin(20, 60, 1, 1); // noua fereastra
    buildWindow(win);    
    
    int x = 12, y = 12;
    first2cardsdealer(win);
    first2cardsplayer(win);

    while (true){
               
        char ply = wgetch(win);//player choice
        if(ply == 'h') {
            dealplayer(win, y);
            if(checkscore(win, false) && !endgame(win, x, y)) {
                break;
            }
        }
        else if (ply == 's'){
            dealdealer(win, x);
            if(checkscore(win, true) && !endgame(win, x, y)) {
                break;
            }
        }
        else if(ply == 'q'){
            break;
        }
        else {
            mvwprintw(win, 8, 63, "  INVALID INPUT    ");
            refresh();
            wrefresh(win);
        }

    }
    getch(); //asteapta o tasta    
    endwin(); //mandatoriu

        return 0;

}
