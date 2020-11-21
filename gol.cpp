#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <locale.h>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

vector<vector<int>> board;
vector<vector<int>> new_board;


void printHelp() {
    cout <<
        "\nUse left/right arrow to decrease/increase the time interval.\n"
        "Press \"p\" or spacebar to pause\n"
        "Press \"r\" to re-generate the board\n"
        "Press \"q\" to stop the program\n";
    exit(0);
}


void generateBoard(int height, int width) {
    // 0 for dead
    // 1 for alive
    srand(time(NULL));
    board.push_back(vector<int>(width+2, 0));
    for(int i = 1; i < height+1; i++) {
        board.push_back(vector<int>(width+2, 0));
        for(int j = 1; j < width+1; j++) {
            int score = rand()%100 + 1;
            if(score >= 50) board[i][j] = 0;
            else board[i][j] = 1;
        }
    }
    board.push_back(vector<int>(width+2, 0));
}


void update(int height, int width) {
    new_board = board;
    for(int i = 1; i < height+1; i++) {
        for(int j = 1; j < width+1; j++) {
            int alive = board[i-1][j-1] + board[i-1][j] + board[i-1][j+1] + 
                        board[i][j-1] + board[i][j+1] + 
                        board[i+1][j-1] + board[i+1][j] + board[i+1][j+1];
            if(board[i][j] == 1) {
                if(alive != 2 && alive != 3) {
                    new_board[i][j] = 0;
                }
            }
            else {
                if(alive == 3){
                    new_board[i][j] = 1;
                }
            }
        }
    }
    board = new_board;
}


void status(char *status_background, int gen, int millisecond) {
	mvprintw(0, 2, status_background);
	mvprintw(0, 3, "Iter:%4d,", gen);
	mvprintw(0, 14, "Interval:%4dms", millisecond);
} 


void draw(int height, int width, char *status_background, int gen, int millisecond) {
	int not_first;
    for(int i=0; i<=height+1; i++) {
		for(int j=0; j<=width+1; j++) {
            not_first = (int)(j>0);
            if(i==0 && j==0) mvaddstr(i, j, "╔");
            else if(i==height+1 && j==0) mvaddstr(i, j, "╚");
            else if(i==0 && j==width+1) mvaddstr(i, j*2-not_first, "╗");
            else if(i==height+1 && j==width+1) mvaddstr(i, j*2-not_first, "╝");
            else if(i==0 || i==height+1) mvaddstr(i, j*2-not_first, "══");
            else if(j==0 || j==width+1) mvaddstr(i, j*2-not_first, "║");
            else if(board[i][j] == 0) mvaddstr(i, j*2-not_first, "  ");
            else mvaddstr(i, j*2-not_first, "██");
		}
    }
	status(status_background, gen, millisecond);
	refresh();
}


void goodBye() {
    clear();
	endwin();
    exit(0);
}


int main(int argc, char *argv[]) {
    const char* const short_opts = "h";
    const option long_opts[] = {
            {"help", no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
    };
    while(true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if(opt == -1) break;

        switch(opt) {
        case 'h':
        case '?':
        default:
            printHelp();
            break;
        }
    }

    setlocale(LC_ALL,"");
    initscr();
	clear();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);

    int rows, cols, height, width, millisecond;
	getmaxyx(stdscr, rows, cols);
    height = rows - 2;
    width = (cols-2) / 2;
    millisecond = 100;

    char status_background[29];
	memset(status_background, ' ', 29);
	status_background[28] = '\0';

    generateBoard(height, width);

    int keyin, gen = 0;
    bool pause = false;
	while(true) {
		keyin = getch();
		switch(keyin) {
		case 'q':
		case 'Q':
            goodBye();
            break;					
		case 'r':
		case 'R':
            generateBoard(height, width);
			break;
        case 'p':
        case 'P':
        case KEY_SREPLACE:
            pause = !pause;
            if(pause) mvprintw(height+1, 3, "/ PAUSED /");
			else mvprintw(height+1, 3, "══════════");
            break;
		case KEY_LEFT:
            if(millisecond > 9) millisecond -= 10;
			break;
		case KEY_RIGHT:
            millisecond += 10;
			break;
		default:
            break;
		}

        if(!pause) {
            draw(height, width, status_background, gen, millisecond);
            update(height, width);
            gen++;
        }
        usleep(millisecond*1000);
	}

    return 0;
}