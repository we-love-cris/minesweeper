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

void show(char map[ROW][COLUMN]) {// 현재 지도를 보고, 플레이어의 상황에 맞게 출력해주는 함수, 지도의 값을 읽어 'ㅁ'을 한자로 바꾼 모양 중 적절한 모양을 프린트한다.
	system("cls");
	printf("  ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	for (int i = 0; i < COLUMN; i++) {
		printf("%2d", i + 1);
	}
	printf("\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	for (int i = 0; i < ROW; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("%2d", i + 1);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		for (int j = 0; j < COLUMN; j++) {
			char temp = map[i][j];
			if (temp == UNKNOWN_MINE_SHAPE || temp == UNKNOWN || ('a' < temp && temp < 'j')) {
				printf("■");
			}
			else if (temp == CLEAR_SHAPE) {
				printf("□");
			}
			else if (temp == 'Q') {
				printf("＠");
			}
			else if ('0' < temp && temp < '9') {
				printf(" %c", temp);
			}
			else if (temp == DISCOVERED_MINE_SHAPE) {
				printf("＊");
			}
		}
		printf("\n");
	}
}

int scanner(char map[ROW][COLUMN], int life) {
	int inputx = -1, inputy = -1;
	if (mode) { printf("\n현재 모드 : kill_mine (모드 변경은 0을 입력하세요) \n"); }
	else { printf("\n현재 모드 : select (모드변경은 0을 입력하세요) \n"); }
	while (inputx == -1) {
		printf("선택할 행 (최대 : %d) : ", ROW);
		inputx = scan_int(ROW);
	}
	if (inputx == 0) {
		if (mode) mode = 0;
		else mode = 1;
		return life;
	}
	while (inputy == -1) {
		printf("선택할 열 (최대 : %d) : ", COLUMN);
		inputy = scan_int(COLUMN);
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
		//int sum = 0;
		int i = 0;
		int difficulty = -1;
		//char c;
		//int k = 0;

		//char str[MINE_AMOUNT] = "";

		while (difficulty == -1) {
			printf("난이도를 선택하세요(easy(1), normal(2), crazy(3), 설명서(0)): ");
			difficulty = scan_int(3);
		}

		/*
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
				break;
			}
		}
		*/

		//difficulty = sum;

		switch (difficulty) {
		case 1:
			return 10;
		case 2:
			return 5;
		case 3:
			return 1;
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

int scan_int(int maximum) {//숫자를 입력받는 함수, maximum을 초과하거나 정수가 아닐 경우 -1을 리턴한다
	char input[100] = { "0" };
	char temp = '\0';
	int result = 0;
	int overflowflag = 0;

	for (int i = 0; i < 99; i++) {
		scanf("%c", &temp);
		input[i] = temp;
		if (temp == '\n')
			break;
		if (i == 98 && temp != '\n') {
			overflowflag = 01;
			i = 0;
		}
	}
	if (overflowflag) {
		printf("올바른 값을 입력해 주세요\n");
		return -1;
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
			return -1;
		}
	}
	if (result <= maximum)
		return result;
	else {
		printf("올바른 값을 입력해 주세요\n");
		return -1;
	}

}