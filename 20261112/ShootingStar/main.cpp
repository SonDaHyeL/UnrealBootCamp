
#include <iostream>
#include "Struct.h"
#include "DoubleBuffering.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"


static Tile BG[DF_BG_SIZEY][DF_BG_SIZEX] = {};

//초기화
void Start()
{
	srand((unsigned)time(NULL)); //새 시드 
	//10만큼 DF_BG_SIZEY보다 크게 잡아서 UI자리
	system("mode con:cols=200 lines=50");

	ScreenInit();

	COORD SpawnPoint = { 10,10 };
	PlayerStart(SpawnPoint);
}


void Update()
{
	PlayerUpdate();
	BulletUpdate();
	EnemyUpdate();

}
void Draw()
{
	PlayerDraw(BG);
	BulletDraw(BG);
	EnemyDraw(BG);
}

void ClearScreen()
{
	unsigned short BaseColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
	for (int y = 0; y < DF_BG_SIZEY; ++y)
	{
		for (int x = 0; x < DF_BG_SIZEX; ++x)
		{
			BG[y][x].Text = ' ';
			BG[y][x].Color = BaseColor;
		}
	}
}

void Render()
{
	//마지막에 널문자
	//BG[DF_BG_SIZEY - 1][DF_BG_SIZEX - 1].Text = '\0';
	ScreenClear();
	ScreenPrint(BG);
	ScreenFlipping();
}

int main()
{
	Start();//한번 호출 초기화 용도

	while (true) //메인 게임 루프
	{
		ClearScreen();

		Update(); //계속 들어옴.
		Draw();
		Render();
		Sleep(30);
	}
	ScreenRelease(); //종료시 해야될처리
}




