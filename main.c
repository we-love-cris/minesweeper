#include "declare.h"

int main(void)
{
  //이 지뢰 찾기에는 두 가지 모드가 있습니다
  //먼저 kill_mine 모드입니다
  //이 모드는 원작 지뢰찾기에 오른쪽 마우스 버튼과 어느 정도 대응되며, 지뢰라고 생각되는 좌표를 선택할 수 있습니다
  //만약 지뢰라면, 지뢰임이 드러나고, 지뢰가 아니라면, 게임에서 패배합니다.
  //그 다음은 select 모드입니다
  //이 모드는 원작 지뢰찾기의 왼쪽 마우스 버튼과 어느 정도 대응되며, 좌표를 하나 선택할 수 있습니다.
  //만약 이 좌표가 지뢰라면 게임에서 패배합니다
  //지뢰가 아니라면, 그 칸의 숫자를 드러내거나, 숫자가 없는 빈 칸이라면, 연쇄적으로 칸들을 탐색하여 숫자들을 드러냅니다.
  //mode => 0 : select, 1 : kill_mine

  char map[ROW][COLUMN];
  int startx = 0, starty = 0;
  int life = Showmenu();
  char initialinput[5];

  printf("지뢰찾기를 시작합니다.\n");
  printf("현재 LIFE: %d\n", life);
  printf("시작할 행을 입력하세요 (최대 %d) : ", ROW);
  scanf("%d", &startx);
  printf("시작할 열을 입력하세요 (최대 %d) : ", COLUMN);
  scanf("%d", &starty);
  create_map(map);
  mining(map, startx - 1, starty - 1);
  numbering(map);
  select(map, startx - 1, starty - 1, life);
  show(map);
  while (life)
  {
    printf("현재 LIFE: %d\n", life);
    life = scanner(map, life);
    show(map);
  }

  return 0;
}