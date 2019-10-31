#include "./declare.h"

// 지도의 초기화를 담당하는 함수.
// 지도의 모든 값을 알려지지 않은 빈 칸으로 설정한다.
void create_map(char map[ROW][COLUMN]) {
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      map[i][j] = UNKNOWN;
    }
  }
  return;
}

// 지도에 지뢰를 매설하는 과정.
// 첫 입력으로 a, b를 받아, 그 칸을 제외하고 랜덤하게 지뢰 생성.
void mining(char map[ROW][COLUMN], int a, int b) {
  srand((unsigned)time(NULL));
  int x = 0, y = 0;
  for (int i = 0; i < MINE_AMOUNT; i++) {
    x = rand_r(0) % ROW;
    y = rand_r(0) % COLUMN;
    if (map[x][y] == UNKNOWN && !((x == a) && (y == b))) {
      map[x][y] = UNKNOWN_MINE_SHAPE;
    } else {
      i--;
      continue;
    }
  }
  return;
}

// 지정된 칸 주위 8칸을 탐색하여 지뢰의 수를 리턴하는 함수 (minecheck와 협업)
int minecheck(char map[ROW][COLUMN], int x, int y) {
  int hor[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int ver[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int result = 0;
  int xi = 0, yi = 0;
  for (int i = 0; i < 8; i++) {
    xi = x + hor[i], yi = y + ver[i];
    if (xi < 0 || xi >= ROW || yi < 0 || yi >= COLUMN)
      continue;
    if (map[xi][yi] == UNKNOWN_MINE_SHAPE)
      result++;
  }
  return result;
}

// 지도에 매설된 지뢰를 통해 숫자를 채워 넣는 과정.
// minecheck를 통해 주변의 지뢰의 수를 세고,
// 해당하는 칸에 주변의 지뢰의 수를 저장한다.
void numbering(char map[ROW][COLUMN]) {
  int temp = 0;
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      if (map[i][j] == UNKNOWN_MINE_SHAPE)
        continue;
      temp = minecheck(map, i, j);
      if (temp == 0)
        continue;

      // 밝혀진 지뢰는 숫자, 밝혀지지 않은 지뢰는
      // b, c, d...와 같은 알파벳으로 표기된다.
      map[i][j] = (char)temp + 'a';
    }
  }
}
