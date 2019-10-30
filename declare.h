// 선언된 표준 라이브러리
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 선언된 상수값들
#include "constants.h"

// 선언된 함수 프로토타입

void print(char map[ROW][COLUMN]);
void show(char map[ROW][COLUMN]);
void create_map(char map[ROW][COLUMN]);
void mining(char map[ROW][COLUMN], int a, int b);
void numbering(char map[ROW][COLUMN]);
int minecheck(char map[ROW][COLUMN], int x, int y);
int select_(char map[ROW][COLUMN], int x, int y, int life);
int chain(char map[ROW][COLUMN], int x, int y);
void discover(char map[ROW][COLUMN], int x, int y);
int kill_mine(char map[ROW][COLUMN], int x, int y, int life);
void boom(char map[ROW][COLUMN], int x, int y);
void reveal(char map[ROW][COLUMN], int x, int y);
int scanner(char map[ROW][COLUMN], int life);
int scan_int(int rowcol);
int win(char map[ROW][COLUMN]);
int showmenu();
