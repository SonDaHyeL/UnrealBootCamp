
#include <iostream>
#include "Struct.h"
#include "DoubleBuffering.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "UI.h"
#include "TimeManager.h"
#include "Background.h"
#include "Effect.h"

static Tile BG[DF_SCREEN_SIZEY][DF_BG_SIZEX] = {};

//게임 상태 관리 변수 추가
enum GameState { STATE_TITLE, STATE_PLAY, STATE_GAMEOVER };
static GameState State = STATE_TITLE;

static bool IsKeyPressed(int vk)
{
	static bool prev[256] = {};
	bool now = (GetAsyncKeyState(vk) & 0x8000) != 0;
	bool pressed = now && !prev[vk];
	prev[vk] = now;
	return pressed;
}


//초기화
void Start()
{
	srand((unsigned)time(NULL)); //새 시드 
	//10만큼 DF_BG_SIZEY보다 크게 잡아서 UI자리 넣음
	system("mode con:cols=120 lines=50");
	ScreenInit();
	BackgroundInit();
}

void GameReset() {
	PlayerStart({ 10,20 });
	BulletReset();
	EnemyReset();
	ScoreReset();
	EffectReset();
}

void Update()
{
	BackgroundUpdate();
	PlayerUpdate();
	BulletUpdate();
	EnemyUpdate();
	EBulletUpdate();
	EffectUpdate();
}
void Draw()
{
	BackgroundDraw(BG);
	PlayerDraw(BG);
	BulletDraw(BG);
	EnemyDraw(BG);
	EBulletDraw(BG);
	EffectDraw(BG);
	UIDraw(BG);
}

void ClearScreen()
{
	unsigned short BaseColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
	for (int y = 0; y < DF_SCREEN_SIZEY; ++y)
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

	bool running = true;
	while (running) //메인 게임 루프
	{
		FPSUpdate();
		ClearScreen();

		switch (State)
		{
		case STATE_TITLE:
			BackgroundUpdate();
			BackgroundDraw(BG);
			TitleDraw(BG);
			if (IsKeyPressed(VK_RETURN)) { 
				GameReset();
				State = STATE_PLAY; 
			}
			else if (IsKeyPressed(VK_ESCAPE)) { running = false; }
			break;
		case STATE_PLAY:
			Update(); //계속 들어옴.
			Draw();
			if (PlayerGetHP() <= 0) { State = STATE_GAMEOVER; }
			break;
		case STATE_GAMEOVER:
			Draw();
			GameOverUI(BG);
			if (IsKeyPressed(VK_RETURN)) { State = STATE_TITLE; }
			if (IsKeyPressed(VK_ESCAPE)) { running = false; }
			break;
		default:
			break;
		}
		Render();
		Sleep(16);
	}
	ScreenRelease(); //종료시 해야될처리
}




