#include "declare.h"

int select(char map[ROW][COLUMN], int x, int y, int life)
{ //만약 밝혀지지 않은 지뢰라면 0을 리턴, 나머지는 1을 리턴한다
  char temp = map[x][y];
  if (temp == UNKNOWN_MINE_SHAPE)
  {
    boom(map, x, y);
    life--;
  }
  if (temp == UNKNOWN)
  {
    chain(map, x, y);
  }
  else if ('a' < temp && temp < 'j')
  {
    map[x][y] = map[x][y] - 'a' + '0';
  }
  return life;
}

int chain(char map[ROW][COLUMN], int x, int y)
{ //찾기 모드에서 버튼이 눌렸을 때 알려지지 않은 빈 칸이었을 경우, 연쇄적으로 빈 칸과 숫자들을 탐색
  int hor[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int ver[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int xi = 0, yi = 0;
  char temp = map[x][y];
  if (temp == CLEAR_SHAPE || temp == DISCOVERED_MINE_SHAPE || temp == UNKNOWN_MINE_SHAPE || ('0' < temp && temp < '9'))
    return 1;          // 이미 조사된 칸이나 지뢰면 return
  discover(map, x, y); // 이번 칸을 조사
  for (int i = 0; i < 8; i++)
  { //주변 8칸을 연쇄적으로 조사
    xi = x + hor[i], yi = y + ver[i];
    if (xi < 0 || xi >= ROW || yi < 0 || yi >= COLUMN)
      continue;
    if (temp == UNKNOWN)
      chain(map, xi, yi);
  }
  return 1;
}

void discover(char map[ROW][COLUMN], int x, int y)
{ // 알려지지 않은 빈칸이거나, 알려지지 않은 숫자일 경우 그를 사용자에게 드러내는 함수, chain과 협업
  char temp = map[x][y];
  if (temp == UNKNOWN)
  {
    map[x][y] = CLEAR_SHAPE;
    return;
  }
  if ('a' < temp && temp < 'j')
  {
    map[x][y] = map[x][y] - 'a' + '0';
    return;
  }
}

int kill_mine(char map[ROW][COLUMN], int x, int y, int life)
{
  char temp = map[x][y];
  if (temp == UNKNOWN_MINE_SHAPE)
    map[x][y] = DISCOVERED_MINE_SHAPE;
  else
  {
    boom(map, x, y);
    life--;
  }
  return life;
}

void boom(char map[ROW][COLUMN], int x, int y)
{
  map[x][y] = 'Q';
  return;
}