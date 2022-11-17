#include <stdio.h>						// printf();, puts();
#include <stdlib.h>						// srand(), rand();
#include <time.h>						// time();
#include <Windows.h>					// system("cls");
#include <conio.h>						// _getch();
#include <stdbool.h>					// true, false

#define UP		72
#define LEFT	75
#define RIGHT	77
#define DOWN	80						// 방향키 값 상수 선언

void SpawnPlayer();
void SpawnPolice();
void StructureLocation();
void UpdateScreen();
void ControlPlayer(int mode);
void ControlPolice();					// 함수 선언


typedef struct Entity {
	int x;
	int y;
	int vx;
	int vy;
} Entity; 
Entity player, police[2];				// 엔티티 구조체 선언

typedef struct Structure {
	int x;
	int y;
} Structure;
Structure pub, home;					// 건물 구조체 선언

int map[10][10] = {
   1,1,0,1,1,0,0,0,0,0,
   0,0,0,0,0,0,1,0,1,1,
   0,1,1,1,1,0,1,0,0,0,
   0,0,0,0,0,0,1,0,1,0,
   0,1,1,0,1,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,
   0,1,1,1,1,0,1,0,1,0,
   0,0,0,0,0,0,1,0,0,0,
   0,1,1,0,1,0,1,0,1,0,
   0,0,0,0,0,0,0,0,0,0
};										// 배열 선언 및 초기화

int main() {
	srand(time(NULL));

	SpawnPlayer();
	StructureLocation();
	SpawnPolice();

	while(true) { 
		system("cls");
		UpdateScreen();
		ControlPlayer(0);
		ControlPolice();
	}
	
}										// 메인 함수

void SpawnPlayer() {
	
	bool trigger = false;

	while (!trigger) {
		int randomX = rand() % 10;
		int randomY = rand() % 10;

		if (map[randomY][randomX] == 0) {
			player.x = randomX;
			player.y = randomY;
			trigger = true;
		}
	}
}										// 플레이어 스폰

void SpawnPolice() {
	int count = 0;
	while (count < 2) {					// 경찰 0, 1 스폰

		bool trigger = false;
		
		while (!trigger) {
			int randomX = rand() % 10;
			int randomY = rand() % 10;
			
			if (map[randomY][randomX] == 0) {
				police[count].x = randomX; 
				police[count].y = randomY; 
				trigger = true;
				count++;
			}
		}
	}
}										// 경찰 2명 스폰

void StructureLocation() {

	bool trigger = false;

	pub.x = player.x;					// 펍의 위치 저장
	pub.y = player.y;					// 플레이어 스폰 장소와 펍의 위치가 같음

	int x = -1, y = -1;					// 임시 위치 저장

										// 5 - pubQuad = homeQuad
	while (!trigger) {
		if (pub.x <= 4 && pub.y <= 4) {
			x = rand() % 5 + 5;			//5~9 출력
			y = rand() % 5 + 5;			//5~9 출력
		}								// home.x >= 5, home.y >= 5		(5 - 1)Quad = 4Quad
		else if (pub.x <= 9 && pub.y <= 4) {
			x = rand() % 5;				//0~4 출력
			y = rand() % 5 + 5;			//5~9 출력
		}								// home.x <= 4, home.y >= 5		(5 - 2)Quad = 3Quad
		else if (pub.x <= 4 && pub.y <= 9) {
			x = rand() % 5 + 5;			//5~9 출력
			y = rand() % 5;				//0~4 출력
		}								// home.x >= 5, home.y <= 4		(5 - 3)Quad = 2Quad
		else if (pub.x <= 9 && pub.y <= 9) {
			x = rand() % 5;				//0~4 출력
			y = rand() % 5;				//0~4 출력
		}								// home.x <= 4, home.y <= 4		(5 - 4)Quad = 1Quad

		if (map[y][x] == 0) {
			home.x = x;
			home.y = y;
			trigger = true;				// 홈의 위치 지정
		}
	}
}										// 펍의 위치를 이용한 집의 위치 정하기

void UpdateScreen() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if		(map[i][j] == 1)							printf("■");
			else if (i == player.y		&& j == player.x)		printf("ⓓ");
			else if (i == pub.y			&& j == pub.x)			printf("Ｐ");
			else if (i == police[0].y	&& j == police[0].x)	printf("P0");
			else if (i == police[1].y	&& j == police[1].x)	printf("P1");
			else if (i == home.y && j == home.x)				printf("Ｈ");
			else if (map[i][j] == 0)							printf("□");
		}
		puts("");
	}
}

void ControlPlayer(int mode) {
	
	if (mode == 0) {
		int input = _getch();			// _getch()로 입력
		
		if (input == 224) {				// ex)방향키의 확장 아스키 코드는 224, xx이므로 먼저 224 확인
			input = _getch();
			
			if		(input == UP && player.y > 0) {
				player.vx = 0;
				player.vy = -1;
			}							// ↑ 속력 지정
			else if (input == LEFT && player.x > 0) {
				player.vx = -1;
				player.vy = 0;
			}							// ← 속력 지정
			else if (input == RIGHT && player.x < 9) {
				player.vx = +1;
				player.vy = 0; 
			}							// → 속력 지정
			else if (input == DOWN && player.y < 9) {
				player.vx = 0;
				player.vy = +1;
			}							// ↓ 속력 지정

			if (map[player.y + player.vy][player.x + player.vx] == 0) {
				player.x += player.vx;	// 이동
				player.y += player.vy;	// 이동
				player.vx = 0;			// 속력 초기화
				player.vy = 0;			// 속력 초기화
			}
		}
	}

	else if (mode == 1) {
		Sleep(150);

		bool trigger = false;

		while (!trigger) {
			int RandomDirection = rand() % 4; // 0 = ↑, 1 = ←, 2 = →, 3 = ↓

			if		(RandomDirection == 0 && player.y > 0) {
				player.vx = 0;
				player.vy = -1;
			}							// ↑ 방향 지정
			else if (RandomDirection == 1 && player.x > 0) {
				player.vx = -1;
				player.vy = 0;
			}							// ← 방향 지정
			else if (RandomDirection == 2 && player.x < 9) {
				player.vx = +1;
				player.vy = 0;
			}							// → 방향 지정
			else if (RandomDirection == 3 && player.y < 9) {
				player.vx = 0;
				player.vy = +1;
			}							// ↓ 방향 지정

			if (map[player.y + player.vy][player.x + player.vx] == 0) {
				player.x += player.vx;	// 이동
				player.y += player.vy;	// 이동
				player.vx = 0;			// 속력 초기화
				player.vy = 0;			// 속력 초기화
				trigger = true;
			}						
		}
	}
}

void ControlPolice() {
	Sleep(150);

	int count = 0;

	while (count < 2) {					// 경찰 2명 이동
	bool trigger = false;

	while (!trigger) {
		int RandomDirection = rand() % 4; // 0 = ↑, 1 = ←, 2 = →, 3 = ↓

		if		(RandomDirection == 0 && police[count].y > 0) {
			police[count].vx = 0;
			police[count].vy = -1;
		}								// ↑ 방향 지정
		else if (RandomDirection == 1 && police[count].x > 0) {
			police[count].vx = -1;
			police[count].vy = 0;
		}								// ← 방향 지정
		else if (RandomDirection == 2 && police[count].x < 9) {
			police[count].vx = +1;
			police[count].vy = 0;
		}								// → 방향 지정
		else if (RandomDirection == 3 && police[count].y < 9) {
			police[count].vx = 0;
			police[count].vy = +1;
		}								// ↓ 방향 지정

			if (map[police[count].y + police[count].vy][police[count].x + police[count].vx] == 0) {
				police[count].x += police[count].vx;	// 이동
				police[count].y += police[count].vy;	// 이동
				police[count].vx = 0;		 			// 속력 초기화
				police[count].vy = 0;					// 속력 초기화
				trigger = true;
				count++;
			}
		}
	}
}