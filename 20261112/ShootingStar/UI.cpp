#include "UI.h"
#include "Player.h"
#include "TimeManager.h"
#include <stdio.h>
#include <cstring>

using namespace std;

static int Score = 0;

void ScoreAdd(int amount){	Score += amount; }

static void DrawString(Tile(*_BG)[DF_BG_SIZEX], int x, int y, const char* str, unsigned short color) {
	for (int i = 0; str[i] != '\0' && x + i < DF_BG_SIZEX; ++i) {
		_BG[y][x + i].Text = str[i];
		_BG[y][x + i].Color = color;
	}
}

static void DrawStringCenter(Tile(*_BG)[DF_BG_SIZEX], int y, const char* str, unsigned short color) { //화면 가운데에 문자열 출력
	int x = (DF_BG_SIZEX - (int)strlen(str)) / 2;
	DrawString(_BG, x, y, str, color);
}

void UIDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	int top = DF_BG_SIZEY; 

	//화면 구분선
	for (int x = 0; x < DF_BG_SIZEX; x++) {
		_BG[top][x].Text = '=';
		_BG[top][x].Color = 0x88;
	}


	//점수 출력
	char buf[64];
	sprintf_s(buf, "점수 : %d", Score);
	DrawString(_BG, 2, top + 2, buf, 0x0f);


	int hp = PlayerGetHP();
	for (int i = 0; i < PLAYER_MAXHP; ++i) {
		DrawString(_BG, 2 + i * 2, top + 4, (i < hp) ? "●" : "○", 0x0c);
	}

	// Lv.3  [########------------]  50/90
	char lv[64];
	sprintf_s(lv, "Lv.%d", GetPlayerLevel());
	DrawString(_BG, 2, top + 6, lv, 0x0B);   // 밝은 청록

	const int BAR_W = 20;
	int filled = GetPlayerExp() * BAR_W / NeedNextExp();

	char bar[BAR_W + 3];
	bar[0] = '[';
	for (int i = 0; i < BAR_W; ++i) bar[i + 1] = (i < filled) ? '#' : '-';
	bar[BAR_W + 1] = ']';
	bar[BAR_W + 2] = '\0';
	DrawString(_BG, 9, top + 6, bar, 0x0B);

	char expText[32];
	sprintf_s(expText, "%d/%d", GetPlayerExp(), NeedNextExp());
	DrawString(_BG, 9 + BAR_W + 3, top + 6, expText, 0x07);

	char fps[32];
	sprintf_s(fps, "FPS : %d", GetFps());
	DrawString(_BG, DF_BG_SIZEX - 12, top + 2, fps, 0x08);
}

void GameOverUI(Tile(*_BG)[DF_BG_SIZEX]) {
	const int boxW = 40;
	const int boxH = 7;

	//게임 영역의 한 가운데 찾기
	int left = (DF_BG_SIZEX - boxW) / 2;
	int top = (DF_BG_SIZEY - boxH) / 2;

	//게임 영역의 한 가운데에 있는 박스 영역을 비우고 테두리 그리기
	for (int y = 0; y < boxH; ++y) {
		for (int x = 0; x < boxW; ++x) {
			Tile& t = _BG[top + y][left + x];
			t.Text = (y == 0 || y == boxH - 1) ? '=' : ' ';
			t.Color = 0x0c;
		}
	}

	DrawStringCenter(_BG, top + 2, "G A M E   O V E R", 0x0C);

	char buf[64];
	sprintf_s(buf, "최종 점수 : %d", Score);
	DrawStringCenter(_BG, top + 3, buf, 0x0F);

	DrawStringCenter(_BG, top + 5, "ENTER : 타이틀로    ESC : 종료", 0x07);
}

void ScoreReset() { Score = 0; }


//메인 화면 UI
static const char* Logo[] = {
	R"( ____   _   _   ___    ___   _____  ___  _   _   ____     ____   _____     _     ____)",
	R"(/ ___| | | | | / _ \  / _ \ |_   _||_ _|| \ | | / ___|   / ___| |_   _|   / \   |  _ \)",
	R"(\___ \ | |_| || | | || | | |  | |   | | |  \| || |  _    \___ \   | |    / _ \  | |_) |)",
	R"( ___) ||  _  || |_| || |_| |  | |   | | | |\  || |_| |    ___) |  | |   / ___ \ |  _ <)",
	R"(|____/ |_| |_| \___/  \___/   |_|  |___||_| \_| \____|   |____/   |_|  /_/   \_\|_| \_\)",
};
static const int LOGO_LINES = sizeof(Logo) / sizeof(Logo[0]);   // 줄 수 자동 계산

// 줄마다 색을 다르게 → 그라데이션 효과
static const unsigned short LogoColors[] = { 0x0C, 0x0E, 0x0E, 0x0A, 0x0B };


void TitleDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	//줄마다 가운데 정렬하면 줄 길이가 달라서 그림이 어긋남
	//가장 긴 줄 기준으로 왼쪽 위치를 한 번만 정하고 모든 줄을 같은 x에서 시작
	int maxW = 0;
	for (int i = 0; i < LOGO_LINES; ++i)
	{
		int w = (int)strlen(Logo[i]);
		if (w > maxW) maxW = w;
	}
	int left = (DF_BG_SIZEX - maxW) / 2;
	int top = 10;

	for (int i = 0; i < LOGO_LINES; ++i)
		DrawString(_BG, left, top + i, Logo[i], LogoColors[i]);

	// 0.5초마다 깜빡이는 안내 문구
	if ((GetTickCount64() / 500) % 2 == 0)
		DrawStringCenter(_BG, top + LOGO_LINES + 6, "PRESS ENTER TO START", 0x0F);

	DrawStringCenter(_BG, top + LOGO_LINES + 10, "방향키 : 이동    SPACE : 발사    ESC : 종료", 0x08);
}