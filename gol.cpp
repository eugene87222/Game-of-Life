#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

vector<vector<int>> board;
vector<vector<int>> new_board;

// int toInt(char *str) {
//     int result = 0;
//     int len = strlen(str);
//     for(int i = 0; i < len; i++) {
//         result = result * 10 + (str[i] - '0');
//     }
//     return result;
// }

void printHelp() {
    cout <<
        "-h, --height <h>:          board height\n"
        "-w, --width <w>:           board width\n"
        "-m, --millisecond <m>:     refresh interval\n";
    exit(1);
}

void sleepClear(int millisecond) {
    this_thread::sleep_for(chrono::milliseconds(millisecond));
    system("clear");
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

void draw(int height, int width) {
    for(int i = 0; i <= height+1; i++) {
        for(int j = 0; j <= width+1; j++) {
            if(i == 0 && j == 0) cout << "╔";
            else if(i == 0 && j == width+1) cout << "╗";
            else if(i == height+1 && j == 0) cout << "╚";
            else if(i == height+1 && j == width+1) cout << "╝";
            else if(i == 0 || i == height+1) cout << "══";
            else if(j == 0 || j == width+1) cout << "║";
            else if(board[i][j] == 0) cout << "  ";
            else cout << "██";
        }
        cout << endl;
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

int main(int argc, char *argv[]) {
    int height, width, millisecond = 150;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    height = w.ws_row - 3;
    width = (w.ws_col - 2) / 2;

    const char* const short_opts = "h:w:m:";
    const option long_opts[] = {
            {"height", required_argument, nullptr, 'h'},
            {"width", required_argument, nullptr, 'w'},
            {"millisecond", required_argument, nullptr, 'm'},
            {nullptr, no_argument, nullptr, 0}
    };
    while(true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if(opt == -1) break;

        switch(opt) {
        case 'h':
            height = stoi(optarg);
            break;
        case 'w':
            width = stoi(optarg);
            break;
        case 'm':
            millisecond = stoi(optarg);
            break;
        case '?':
        default:
            printHelp();
            break;
        }
    }

    generateBoard(height, width);
    sleepClear(0);
    while(1) {
        draw(height, width);
        update(height, width);
        sleepClear(millisecond);
    }
    return 0;
}