#include "UI.h"
#include "Player.h"
#include "TimeManager.h"
#include <stdio.h>
#include <cstring>

using namespace std;

static int Score = 0;

int GetScore() { return Score; }
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

static int DisplayScore = 0; //화면에 보이는 점수. 실제 Score를 천천히 따라감

//테두리 박스 + 제목. 안쪽은 건드리지 않음
static void DrawBox(Tile(*_BG)[DF_BG_SIZEX], int x, int y, int w, int h, const char* title, unsigned short color)
{
	for (int dy = 0; dy < h; ++dy) {
		for (int dx = 0; dx < w; ++dx) {
			bool edgeY = (dy == 0 || dy == h - 1);
			bool edgeX = (dx == 0 || dx == w - 1);
			if (!edgeY && !edgeX) continue;

			Tile& t = _BG[y + dy][x + dx];
			t.Text = (edgeY && edgeX) ? '+' : (edgeY ? '-' : '|');
			t.Color = color;
		}
	}
	char buf[32];
	sprintf_s(buf, "[ %s ]", title);
	DrawString(_BG, x + 2, y, buf, 0x0F);
}

//──── HP ────
static void DrawHP(Tile(*_BG)[DF_BG_SIZEX], int x, int y)
{
	int hp = PlayerGetHP();
	unsigned short color = (hp == PLAYER_MAXHP) ? 0x0A : (hp == 1) ? 0x0C : 0x0E; //초록 / 빨강 / 노랑
	bool blinkOn = (GetTickCount64() / 250) % 2 == 0; //0.25초 주기

	if (hp == 1 && !blinkOn) color = 0x04; //위험하면 밝은 빨강 <-> 어두운 빨강 깜빡임

	DrawString(_BG, x, y, "HP", 0x07);
	for (int i = 0; i < PLAYER_MAXHP; ++i) {
		if (i < hp) DrawString(_BG, x + 4 + i * 3, y, "●", color);
		else        DrawString(_BG, x + 4 + i * 3, y, "○", 0x08);
	}

	if (hp == 1)
		DrawString(_BG, x + 4 + PLAYER_MAXHP * 3 + 1, y, "DANGER!", color);
}

//──── Lv + EXP 막대 ────
static void DrawExp(Tile(*_BG)[DF_BG_SIZEX], int x, int y)
{
	char lv[16];
	sprintf_s(lv, "Lv.%-2d", GetPlayerLevel()); //%-2d : 두 자리가 되어도 막대 위치가 밀리지 않게
	DrawString(_BG, x, y, lv, 0x0B);

	const int BAR_W = 20;
	int filled = GetPlayerExp() * BAR_W / NeedNextExp();

	for (int i = 0; i < BAR_W; ++i) {
		Tile& t = _BG[y][x + 7 + i];
		t.Text = ' ';
		t.Color = (i < filled) ? 0xB0 : 0x80; //앞자리 = 배경색. 배경 하늘색 / 배경 회색
	}

	char pct[8];
	sprintf_s(pct, "%3d%%", GetPlayerExp() * 100 / NeedNextExp()); //%% 는 % 문자 출력
	DrawString(_BG, x + 7 + BAR_W + 1, y, pct, 0x07);
}

//──── 큰 숫자 점수 ────
static const char* BigDigit[10][3] = {
	{ " _ ", "| |", "|_|" },  // 0
	{ "   ", "  |", "  |" },  // 1
	{ " _ ", " _|", "|_ " },  // 2
	{ " _ ", " _|", " _|" },  // 3
	{ "   ", "|_|", "  |" },  // 4
	{ " _ ", "|_ ", " _|" },  // 5
	{ " _ ", "|_ ", "|_|" },  // 6
	{ " _ ", "  |", "  |" },  // 7
	{ " _ ", "|_|", "|_|" },  // 8
	{ " _ ", "|_|", " _|" },  // 9
};

static void DrawScore(Tile(*_BG)[DF_BG_SIZEX], int centerX, int y)
{
	//롤링 : 남은 차이의 1/8씩 따라감 -> 처음엔 빠르고 끝에선 천천히
	if (DisplayScore < Score) {
		int step = (Score - DisplayScore) / 8;
		DisplayScore += (step > 1) ? step : 1;
	}
	bool rolling = (DisplayScore < Score);

	const int DIGITS = 7;
	const int DIGIT_W = 4; //숫자 3칸 + 간격 1칸
	char num[16];
	sprintf_s(num, "%07d", DisplayScore); //앞을 0으로 채운 7자리

	int x = centerX - (DIGITS * DIGIT_W) / 2;
	bool leadingZero = true;

	for (int i = 0; i < DIGITS; ++i) {
		int d = num[i] - '0'; //문자 '7' -> 숫자 7
		if (d != 0 || i == DIGITS - 1) leadingZero = false;

		unsigned short color = leadingZero ? 0x08 : (rolling ? 0x0E : 0x0F);
		for (int row = 0; row < 3; ++row)
			DrawString(_BG, x + i * DIGIT_W, y + row, BigDigit[d][row], color);
	}
}

//──── SHOT / FPS ────
static void DrawSystem(Tile(*_BG)[DF_BG_SIZEX], int x, int y)
{
	int lines = PlayerGetShotLines();
	DrawString(_BG, x, y, "SHOT", 0x07);
	for (int i = 0; i < MAX_SHOTLINE; ++i)
		DrawString(_BG, x + 6 + i * 2, y, (i < lines) ? ">" : "-", (i < lines) ? 0x0A : 0x08);

	char fps[16];
	sprintf_s(fps, "FPS  %d", GetFps());
	DrawString(_BG, x, y + 2, fps, 0x08);
}

void UIDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	int top = DF_BG_SIZEY;

	//화면 구분선
	for (int x = 0; x < DF_BG_SIZEX; x++) {
		_BG[top][x].Text = '=';
		_BG[top][x].Color = 0x08; //0x88은 글자색 = 배경색이라 '='가 안 보였음
	}

	const int PANEL_Y = top + 1; //41번 줄부터
	const int PANEL_H = 7;       //41~47번 줄

	DrawBox(_BG,  1, PANEL_Y, 36, PANEL_H, "PILOT",  0x03);
	DrawBox(_BG, 38, PANEL_Y, 44, PANEL_H, "SCORE",  0x06);
	DrawBox(_BG, 83, PANEL_Y, 36, PANEL_H, "SYSTEM", 0x08);

	DrawHP    (_BG, 4, PANEL_Y + 2);
	DrawExp   (_BG, 4, PANEL_Y + 4);
	DrawScore (_BG, 38 + 44 / 2, PANEL_Y + 2); //SCORE 패널의 가운데
	DrawSystem(_BG, 86, PANEL_Y + 2);
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

void ScoreReset() { Score = 0; DisplayScore = 0; }


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