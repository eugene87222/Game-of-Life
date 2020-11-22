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
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

vector<vector<int>> board;
vector<vector<int>> new_board;
vector<thread> thds;
int thd_num = 4;


void printHelp() {
    cout <<
        "\nUse left/right arrow to decrease/increase the time interval.\n"
        "Press \"p\" or spacebar to pause\n"
        "Press \"r\" to re-generate the board\n"
        "Press \"q\" to stop the program\n";
    exit(0);
}


void emptyBoard(int height, int width) {
    board = vector<vector<int>>(height+2, vector<int>(width+2, 0));
}


void randomBoard(int height, int width) {
    emptyBoard(height, width);
    srand(time(NULL));
    for(int i=1; i<height+1; i++) {
        for(int j=1; j<width+1; j++) {
            int score = rand()%100 + 1;
            // 0 for dead
            // 1 for alive
            if(score >= 50) board[i][j] = 0;
            else board[i][j] = 1;
        }
    }
}


void Gosperglidergun(int height, int width) {
    emptyBoard(height, width);

    board[4][2] = 1;
    board[5][2] = 1;
    board[4][3] = 1;
    board[5][3] = 1;

    board[2][13] = 1;
    board[3][13] = 1;
    board[7][13] = 1;
    board[8][13] = 1;
    board[3][15] = 1;
    board[7][15] = 1;
    board[4][16] = 1;
    board[5][16] = 1;
    board[6][16] = 1;
    board[4][17] = 1;
    board[5][17] = 1;
    board[6][17] = 1;

    board[7][20] = 1;
    board[6][21] = 1;
    board[7][21] = 1;
    board[8][21] = 1;
    board[5][22] = 1;
    board[9][22] = 1;
    board[7][23] = 1;
    board[4][24] = 1;
    board[10][24] = 1;
    board[4][25] = 1;
    board[10][25] = 1;
    board[5][26] = 1;
    board[9][26] = 1;
    board[6][27] = 1;
    board[7][27] = 1;
    board[8][27] = 1;

    board[6][36] = 1;
    board[7][36] = 1;
    board[6][37] = 1;
    board[7][37] = 1;
}


void Simkinglidergun(int height, int width) {
    emptyBoard(height, width);

    board[21][2] = 1;
    board[22][2] = 1;
    board[21][3] = 1;
    board[22][3] = 1;

    board[18][6] = 1;
    board[19][6] = 1;
    board[18][7] = 1;
    board[19][7] = 1;

    board[21][9] = 1;
    board[22][9] = 1;
    board[21][10] = 1;
    board[22][10] = 1;

    board[4][22] = 1;
    board[5][22] = 1;
    board[3][23] = 1;
    board[5][23] = 1;
    board[3][24] = 1;
    board[2][25] = 1;
    board[3][25] = 1;

    board[10][23] = 1;
    board[11][23] = 1;
    board[12][23] = 1;
    board[10][24] = 1;
    board[13][24] = 1;
    board[10][25] = 1;
    board[13][25] = 1;
    board[13][27] = 1;
    board[9][28] = 1;
    board[13][28] = 1;
    board[10][29] = 1;
    board[12][29] = 1;
    board[11][30] = 1;

    board[10][33] = 1;
    board[11][33] = 1;
    board[10][34] = 1;
    board[11][34] = 1;
}


void update_thd(int h_begin, int h_end, int w_begin, int w_end) {
    for(int i = h_begin; i <= h_end; i++) {
        for(int j = w_begin; j <= w_end; j++) {
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


void status(char *status_background, int gen, int ms) {
	mvprintw(0, 2, status_background);
	mvprintw(0, 3, "Iter:%4d,", gen);
	mvprintw(0, 14, "Interval:%4dms", ms);
} 


void draw(int height, int width, char *status_background, int gen, int ms) {
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
	status(status_background, gen, ms);
	refresh();
}


void goodBye() {
    clear();
	endwin();
    exit(0);
}


int main(int argc, char *argv[]) {
    bool gosper=false, simkin=false;
    const char* const short_opts = "h";
    const option long_opts[] = {
            {"gosper", no_argument, nullptr, 'g'},
            {"simkin", no_argument, nullptr, 's'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
    };
    while(true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if(opt == -1) break;

        switch(opt) {
        case 'g':
            gosper = true;
            break;
        case 's':
            simkin = true;
            break;
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

    int rows, cols, height, width, ms, p_size;
	getmaxyx(stdscr, rows, cols);
    height = rows - 2;
    width = (cols-2) / 2;
    ms = 100;
    p_size = height / thd_num;
    thds = vector<thread>(thd_num);

    char status_background[29];
	memset(status_background, ' ', 29);
	status_background[28] = '\0';

    if(gosper && height>=11 && width>=38) Gosperglidergun(height, width);
    else if(simkin && height>=24 && width>=35) Simkinglidergun(height, width);
    else randomBoard(height, width);

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
            randomBoard(height, width);
			break;
        case 'p':
        case 'P':
        case KEY_SREPLACE:
            pause = !pause;
            if(pause) mvprintw(height+1, 3, "/ PAUSED /");
			else mvprintw(height+1, 3, "══════════");
            break;
		case KEY_LEFT:
            if(ms > 9) ms -= 10;
			break;
		case KEY_RIGHT:
            ms += 10;
			break;
		default:
            break;
		}

        if(!pause) {
            draw(height, width, status_background, gen, ms);
            // update(height, width);
            new_board = board;
            for(int i=0; i<thd_num; i++) {
                thds[i] = thread(update_thd, p_size*i+1, i==thd_num-1?height:p_size*(i+1), 1, width);
            }
            for(int i=0; i<thd_num; i++) {
                thds[i].join();
            }
            board = new_board;
            gen++;
        }
        usleep(ms*1000);
	}

    return 0;
}