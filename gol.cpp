#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#endif

#include <ncurses.h>
using namespace std;

vector<vector<int>> board;
vector<vector<int>> new_board;

int toInt(char *str) {
    int result = 0;
    int len = strlen(str);
    for(int i = 0; i < len; i++) {
        result = result * 10 + (str[i] - '0');
    }
    return result;
}

void sleepClear(int millisecond) {
    this_thread::sleep_for(chrono::milliseconds(millisecond));
    system("clear");
}

void generateBoard(int height, int width) {
    srand(time(NULL));
    board.push_back(vector<int>(width+2, 0));
    for(int i = 1; i < height+1; i++) {
        board.push_back(vector<int>(width+2, 0));
        for(int j = 1; j < width+1; j++) {
            int score = rand()%100 + 1;
            if(score >= 60) board[i][j] = 0;
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
    int height = toInt(argv[1]);
    int width = toInt(argv[2]);
    generateBoard(height, width);
    sleepClear(0);
    while(1) {
        draw(height, width);
        update(height, width);
        sleepClear(20);
    }
    return 0;
}