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

int scanner(char map[ROW][COLUMN], int life) {
	int inputx = 0, inputy = 0;
	if (mode) { printf("\n현재 모드 : kill_mine (모드 변경은 0을 입력하세요) \n"); }
	else { printf("\n현재 모드 : select (모드변경은 0을 입력하세요) \n"); }
	while (!inputx) {
		printf("선택할 행 (최대 : %d) : ", ROW);
		inputx = scan_int(1);
	}
	if (inputx == 0) {
		if (mode) mode = 0;
		else mode = 1;
		return life;
	}
	while (!inputy) {
		printf("선택할 열 (최대 : %d) : ", COLUMN);
		inputy = scan_int(0);
	}
	if (inputy == 0) {
		if (mode) mode = 0;
		else mode = 1;
		return life;
	}
	inputx -= 1;
	inputy -= 1;
	if (mode) {
		life = kill_mine(map, inputx, inputy, life);
	}
	else {
		life = select_(map, inputx, inputy, life);
	}
	return life;
}


int calc(int m) {
	if (m == 0) return 1;
	return 10 * calc(m - 1);
}

int 거듭제곱(char str[], int k) {
	int sum = 0;
	for (int i = 0; i <= k; i++) {
		if (i == k) {
			sum += str[k] - 48;
		}
		else sum += (str[i] - 48) * calc(k - i);
	}
	return sum;
}

int showmenu() { //난이도 설정 메뉴
	while (1) {
		int sum = 0; int i = 0;
		int difficulty = 100;
		char c;
		int k = 0;

		char str[MINE_AMOUNT] = "";
		printf("난이도를 선택하세요(easy(1), normal(2), crazy(3), 설명서(0)): ");
		while (1) {
			if (i > MINE_AMOUNT - 1) break;
			scanf("%c", &c);
			if (c == '\n') break;
			else str[i] = c;
			i++;
		}

		for (k = 0; k < i; k++) {
			if (str[k] >= '0' && str[k] <= '9') {
				sum = 거듭제곱(str, k);
			}
			if (str[k] < '0' || str[k] > '9') {
				printf("잘못된 입력입니다. \n");
				break;
			}
			if (str[k] < '0' || str[k] > '9') {
				break;
			}
		}

		difficulty = sum;

		switch (difficulty) {
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

int scan_int(int rowcol) {//row선택인지 column선택인지에 따라 숫자를 받는 함수 row의 경우 1, column인 경우 0이 입력된다
	char input[100] = { "0" };
	char temp = '\0';
	int result = 0;

	for (int i = 0; i < 99; i++) {
		scanf("%c", &temp);
		input[i] = temp;
		if (temp == '\n')
			break;
	}
	for (int i = 0; i < 99; i++) {
		temp = input[i];
		if (temp == '\n') break;
		if ('0' <= temp && temp <= '9') {
			result *= 10;
			result += temp - '0';
		}
		else {
			printf("올바른 값을 입력해 주세요\n");
			return 0;
		}
	}
	if (rowcol) {// row에 해당하는 입력을 받을 경우
		if (result <= ROW)
			return result;
		else {
			printf("올바른 값을 입력해 주세요\n");
			return 0;
		}
	}
	else { // Column에 대한 입력을 받을 경우
		if (result <= COLUMN)
			return result;
		else {
			printf("올바른 값을 입력해 주세요\n");
			return 0;
		}
	}
}