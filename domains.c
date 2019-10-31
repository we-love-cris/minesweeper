#include "./declare.h"

// 게임 모드 중 select 모드에 해당한다.
// 만약 밝혀지지 않은 지뢰라면 life를 1 깎아 리턴한다.
int select_(char map[ROW][COLUMN], int x, int y, int life) {
  char temp = map[x][y];
  if (temp == UNKNOWN_MINE_SHAPE) {
    if (life == 1)
      boom(map, x, y);
    else
      reveal(map, x, y);
    life--;
  }
  if (temp == UNKNOWN) {
    chain(map, x, y);
  } else if ('a' < temp && temp < 'j') {
    map[x][y] = map[x][y] - 'a' + '0';
  }
  return life;
}

// select에서 버튼이 눌렸을 때 알려지지 않은 빈 칸이었을 경우,
// 연쇄적으로 빈 칸과 숫자들을 탐색
int chain(char map[ROW][COLUMN], int x, int y) {
  int hor[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int ver[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int xi = 0, yi = 0;
  char temp = map[x][y];
  if (temp == CLEAR_SHAPE
      || temp == DISCOVERED_MINE_SHAPE
      || temp == UNKNOWN_MINE_SHAPE
      || ('0' < temp && temp < '9'))
    return 1;  // 이미 조사된 칸이나 지뢰면 return
  discover(map, x, y);  // 이번 칸을 조사

  // 주변 8칸을 연쇄적으로 조사
  for (int i = 0; i < 8; i++) {
    xi = x + hor[i], yi = y + ver[i];
    if (xi < 0 || xi >= ROW || yi < 0 || yi >= COLUMN)
      continue;
    if (temp == UNKNOWN)
      chain(map, xi, yi);
  }
  return 1;
}

// 알려지지 않은 빈칸이거나, 알려지지 않은 숫자일 경우
// 그를 사용자에게 드러내는 함수, chain과 협업
void discover(char map[ROW][COLUMN], int x, int y) {
  char temp = map[x][y];
  if (temp == UNKNOWN) {
    map[x][y] = CLEAR_SHAPE;
    return;
  }
  if ('a' < temp && temp < 'j') {
    map[x][y] = map[x][y] - 'a' + '0';
    return;
  }
}

int kill_mine(char map[ROW][COLUMN], int x, int y, int life) {
  char temp = map[x][y];
  if (temp == UNKNOWN_MINE_SHAPE) {
    map[x][y] = DISCOVERED_MINE_SHAPE;
  } else {
    if (life == 1)
      boom(map, x, y);
    else
      reveal(map, x, y);
    life--;
  }
  return life;
}

void boom(char map[ROW][COLUMN], int x, int y) {
  map[x][y] = 'Q';
  return;
}

void reveal(char map[ROW][COLUMN], int x, int y) {
  char temp = map[x][y];
  if (temp == UNKNOWN_MINE_SHAPE) {
    temp = DISCOVERED_MINE_SHAPE;
  } else if ('a' < temp && temp < 'j') {
    temp = temp - 'a' + '0';
  } else if (UNKNOWN) {
    chain(map, x, y);
    temp = CLEAR_SHAPE;
  }
  map[x][y] = temp;
  return;
}

void win(char map[ROW][COLUMN]) {
  show(map);
  printf("축하합니다! 승리하셨습니다!\n");
  return;
}
