#include "declare.h"

static int mode = 0; //0 : select, 1 : kill_mine

void print(char map[ROW][COLUMN])
{ // 현재 지도를 표현해주는 함수(제작자용으로, 지도의 모든 상태가 나타난다)
  for (int i = 0; i < ROW; i++)
  {
    for (int j = 0; j < COLUMN; j++)
    {
      printf("%c", map[i][j]);
    }
    printf("\n");
  }
  return;
}

void show(char map[ROW][COLUMN])
{ // 현재 지도를 보고, 플레이어의 상황에 맞게 출력해주는 함수, 지도의 값을 읽어 'ㅁ'을 한자로 바꾼 모양 중 적절한 모양을 프린트한다.
  for (int i = 0; i < ROW; i++)
  {
    for (int j = 0; j < COLUMN; j++)
    {
      char temp = map[i][j];
      if (temp == UNKNOWN_MINE_SHAPE || temp == UNKNOWN || ('a' < temp && temp < 'j'))
      {
        printf("■"); //탐사 안한 부분
      }
      else if (temp == CLEAR_SHAPE)
      {
        printf("□"); //탐사를 했으나 멀쩡한 부분
      }
      else if (temp == 'Q')
      {
        printf("＠"); //지뢰 (kill모드에선 지뢰가 아닌 부분을 kill한 경우)
      }
      else if ('0' < temp && temp < '9')
      {
        printf(" %c", temp); //근처 지뢰개수
      }
      else if (temp == DISCOVERED_MINE_SHAPE)
      {
        printf("＊"); //지뢰 제거(발견) 완료
      }
    }
    printf("\n");
  }
}

int scanner(char map[ROW][COLUMN], int life)
{ //모드 선택 및 행,열 결정.
  int inputx = 0, inputy = 0;
  int result;
  char input[5];
  if (mode)
  {
    printf("현재 모드 : kill_mine (모드 변경은 0을 입력하세요) \n");
  }
  else
  {
    printf("현재 모드 : select (모드변경은 0을 입력하세요) \n");
  }
  printf("선택할 행 (최대 : %d) : ", ROW);
  scanf("%d", &inputx);
  if (inputx == 0)
  {
    if (mode)
      mode = 0;
    else
      mode = 1;
    return life;
  }
  printf("선택할 열 (최대 : %d) : ", COLUMN);
  scanf("%d", &inputy);
  if (inputy == 0)
  {
    if (mode)
      mode = 0;
    else
      mode = 1;
    return life;
  }
  inputx -= 1;
  inputy -= 1;
  if (mode)
  {
    result = kill_mine(map, inputx, inputy, life);
  }
  else
  {
    result = select(map, inputx, inputy, life);
  }
  return result;
}

int Showmenu()
{ //난이도 설정 메뉴
  int difficulty = 0;

  while (1)
  {
    printf("난이도를 선택하세요(easy(1), normal(2), crazy(3), 설명서(0)): ");
    scanf("%d", &difficulty);

    switch (difficulty)
    {
    case 1:
      return 10;
      break;
    case 2:
      return 5;
      break;
    case 3:
      return 1;
      break;
    case 0:
      printf("select mode: 탐색(지뢰 선택 시 LIFE 감소)\n");
      printf("kill mode: 지뢰제거(지뢰 외 제거 시 LIFE 감소)\n");
      break;
    default:
      printf("난이도를 올바르게 입력하세요\n");
      break;
    }
  }
}
