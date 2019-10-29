#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 16
#define COLUMN 30
#define MINE_AMOUNT 99
#define DISCOVERED_MINE_SHAPE 'M' 
#define UNKNOWN_MINE_SHAPE '*'
#define CLEAR_SHAPE ' '// 사용자가 발견한 빈 칸
#define UNKNOWN '0' //사용자가 발견하지 못한 빈 칸

void print(char map[ROW][COLUMN]);
void show(char map[ROW][COLUMN]);
void create_map(char map[ROW][COLUMN]);
void mining(char map[ROW][COLUMN], int a, int b);
void numbering(char map[ROW][COLUMN]);
int minecheck(char map[ROW][COLUMN], int x, int y);
int select(char map[ROW][COLUMN], int x, int y, int life);
int chain(char map[ROW][COLUMN], int x, int y);
void discover(char map[ROW][COLUMN], int x, int y);
int kill_mine(char map[ROW][COLUMN], int x, int y, int life);
void boom(char map[ROW][COLUMN], int x, int y);
int scanner(char map[ROW][COLUMN], int life);
int Showmenu();

static int mode = 0;//0 : select, 1 : kill_mine

void print(char map[ROW][COLUMN]) {// 현재 지도를 표현해주는 함수(제작자용으로, 지도의 모든 상태가 나타난다)
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	return;
}

void show(char map[ROW][COLUMN]) {// 현재 지도를 보고, 플레이어의 상황에 맞게 출력해주는 함수, 지도의 값을 읽어 'ㅁ'을 한자로 바꾼 모양 중 적절한 모양을 프린트한다.
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			char temp = map[i][j];
			if (temp == UNKNOWN_MINE_SHAPE || temp == UNKNOWN || ('a' < temp && temp < 'j')) {
				printf("■"); //탐사 안한 부분
			}
			else if (temp == CLEAR_SHAPE) {
				printf("□"); //탐사를 했으나 멀쩡한 부분
			}
			else if (temp == 'Q') {
				printf("＠"); //지뢰 (kill모드에선 지뢰가 아닌 부분을 kill한 경우)
			}
			else if ('0' < temp && temp < '9') {
				printf(" %c", temp); //근처 지뢰개수
			}
			else if (temp == DISCOVERED_MINE_SHAPE) {
				printf("＊"); //지뢰 제거(발견) 완료
			}
		}
		printf("\n");
	}
}

void create_map(char map[ROW][COLUMN]) {// 지도의 초기화를 담당하는 함수
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			map[i][j] = UNKNOWN;
		}
	}
	return;
}


void mining(char map[ROW][COLUMN], int a, int b) {// 지도에 지뢰를 매설하는 과정, 첫 입력으로 a, b를 받아, 그 칸을 제외하고 지뢰 생성
	srand((unsigned)time(NULL));
	int x = 0, y = 0;
	for (int i = 0; i < MINE_AMOUNT; i++) {
		x = rand() % ROW;
		y = rand() % COLUMN;
		if (map[x][y] == UNKNOWN && !((x == a) && (y == b))) {
			map[x][y] = UNKNOWN_MINE_SHAPE;
		}
		else {
			i--;
			continue;
		}
	}
	return;
}

void numbering(char map[ROW][COLUMN]) {// 지도에 매설된 지뢰를 통해 숫자를 채워 넣는 과정
	int temp = 0;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == UNKNOWN_MINE_SHAPE) continue;
			temp = minecheck(map, i, j);
			if (temp == 0) continue;
			map[i][j] = (char)temp + 'a';//밝혀진 지뢰는 숫자, 밝혀지지 않은 지뢰는 b, c, d...와 같은 알파벳으로 표기된다.
		}
	}
}

int minecheck(char map[ROW][COLUMN], int x, int y) {//지정된 칸 주위 8칸을 탐색하여 지뢰의 수를 리턴하는 함수, minecheck와 협업
	int hor[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int ver[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int result = 0;
	int xi = 0, yi = 0;
	for (int i = 0; i < 8; i++) {
		xi = x + hor[i], yi = y + ver[i];
		if (xi < 0 || xi >= ROW || yi < 0 || yi >= COLUMN) continue;
		if (map[xi][yi] == UNKNOWN_MINE_SHAPE) result++;
	}
	return result;
}

int select(char map[ROW][COLUMN], int x, int y, int life) {//만약 밝혀지지 않은 지뢰라면 0을 리턴, 나머지는 1을 리턴한다
	char temp = map[x][y];
	if (temp == UNKNOWN_MINE_SHAPE) {
		boom(map, x, y);
		life--;
	}
	if (temp == UNKNOWN) {
		chain(map, x, y);
	}
	else if ('a' < temp && temp < 'j') {
		map[x][y] = map[x][y] - 'a' + '0';
	}
	return life;
}

int chain(char map[ROW][COLUMN], int x, int y) {//찾기 모드에서 버튼이 눌렸을 때 알려지지 않은 빈 칸이었을 경우, 연쇄적으로 빈 칸과 숫자들을 탐색
	int hor[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int ver[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int xi = 0, yi = 0;
	char temp = map[x][y];
	if (temp == CLEAR_SHAPE || temp == DISCOVERED_MINE_SHAPE || temp == UNKNOWN_MINE_SHAPE || ('0' < temp && temp < '9')) return 1; // 이미 조사된 칸이나 지뢰면 return
	discover(map, x, y);// 이번 칸을 조사
	for (int i = 0; i < 8; i++) {//주변 8칸을 연쇄적으로 조사
		xi = x + hor[i], yi = y + ver[i];
		if (xi < 0 || xi >= ROW || yi < 0 || yi >= COLUMN) continue;
		if (temp == UNKNOWN)
			chain(map, xi, yi);
	}
	return 1;
}

void discover(char map[ROW][COLUMN], int x, int y) {// 알려지지 않은 빈칸이거나, 알려지지 않은 숫자일 경우 그를 사용자에게 드러내는 함수, chain과 협업
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
	if (temp == UNKNOWN_MINE_SHAPE)
		map[x][y] = DISCOVERED_MINE_SHAPE;
	else {
		boom(map, x, y);
		life--;
	}
	return life;
}

void boom(char map[ROW][COLUMN], int x, int y) {
	map[x][y] = 'Q';
	return;
}

int scanner(char map[ROW][COLUMN], int life) { //모드 선택 및 행,열 결정.
	int inputx = 0, inputy = 0;
	int result;
	char input[5];
	if (mode) { printf("현재 모드 : kill_mine (모드 변경은 0을 입력하세요) \n"); }
	else { printf("현재 모드 : select (모드변경은 0을 입력하세요) \n"); }
	printf("선택할 행 (최대 : %d) : ", ROW);
	scanf("%d", &inputx);
	if (inputx == 0) {
		if (mode) mode = 0;
		else mode = 1;
		return life;
	}
	printf("선택할 열 (최대 : %d) : ", COLUMN);
	scanf("%d", &inputy);
	if (inputy == 0) {
		if (mode) mode = 0;
		else mode = 1;
		return life;
	}
	inputx -= 1;
	inputy -= 1;
	if (mode) {
		result = kill_mine(map, inputx, inputy, life);
	}
	else {
		result = select(map, inputx, inputy, life);
	}
	return result;
}

int Showmenu() { //난이도 설정 메뉴
	int difficulty = 0;

	while (1) {
		printf("난이도를 선택하세요(easy(1), normal(2), crazy(3), 설명서(0)): ");
		scanf("%d", &difficulty);

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

int main(void) {
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
	while (life) {
		printf("현재 LIFE: %d\n", life);
		life = scanner(map, life);
		show(map);
	}

	return 0;
}