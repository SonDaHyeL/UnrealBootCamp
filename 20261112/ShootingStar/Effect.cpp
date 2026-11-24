#include "Effect.h"


//***타격 이펙트
#define MAX_EFFECT 30
static Effect effects[MAX_EFFECT] = {};
static const int STAGE_COUNT = 3; //이펙트 변화 단계
static const int FRAMES_PER_STAGE = 3; //각 단계별 프레임 수

//[단계][줄] 형식. 각 줄마다 3글자
static const char* ExplosionShape[STAGE_COUNT][3] = {
	{ "\\|/", "-*-", "/|\\" },   // \ 는 문자열 안에서 \\ 로 써야 함!
	{ ".+.",  "+ +",  ".+." },
	{ ". .",  " . ",  ". ." },
};

static const unsigned short ExplosionColor[STAGE_COUNT] = { 0x0E, 0x0C, 0x08 }; //폭발 단계 별 색상 변경

//***레벨 업 이펙트
static const int POPUP_FRAME = 40; //표시 시간 (약 1초)
static const int POPUP_RISE_INTERVAL = 10; 
static const int POPUP_FADE_FRAME = 15; 
static const unsigned short PopColors[] = { 0x0E,0x0C,0x0D,0x0B,0x0A };
static const int POPUP_COLOR_COUNT = sizeof(PopColors) / sizeof(PopColors[0]);

static COORD PopupPos = {};
static int PopupTimer = 0;
static bool PopupShotUp = false;

static void DrawPopupText(Tile(*_BG)[DF_BG_SIZEX], int centerX, int y, const char* text, unsigned short color) {
	if (y < 0 || y >= DF_BG_SIZEY) return;

	int len = (int)strlen(text);
	int x = centerX - len / 2;
	if (x < 0) x = 0;
	if (x + len > DF_BG_SIZEX) x = DF_BG_SIZEX - 1;

	for (int i = 0; i < len; ++i) {
		_BG[y][x + i].Text = text[i];
		_BG[y][x + i].Color = color;
	}
}



void EffectSpawn(COORD center)
{
	for (int i = 0; i < MAX_EFFECT;++i) {
		if (!effects[i].Active) {
			effects[i].Active = true;
			effects[i].Center = center;
			effects[i].Frame = 0;
			return;
		}
	}
	PopupTimer = 0;
}

void EffectUpdate()
{
	for (int i = 0; i < MAX_EFFECT;++i) {
		if (!effects[i].Active) continue;

		if (++effects[i].Frame >= STAGE_COUNT * FRAMES_PER_STAGE) {
			effects[i].Active = false; //현재 총 9프레임 후 종료
		}
	}
	if (PopupTimer > 0) --PopupTimer;
	
}

void EffectDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	for (int i = 0; i < MAX_EFFECT; ++i) {
		if (!effects[i].Active) continue;

		int stage = effects[i].Frame / FRAMES_PER_STAGE;
		for (int dy = -1; dy <= 1; ++dy) {
			for (int dx = -1; dx <= 1; ++dx) {
				int x = effects[i].Center.X + dx;
				int y = effects[i].Center.Y + dy;

				if (x < 0 || x >= DF_BG_SIZEX || y < 0 || y >= DF_BG_SIZEY) { continue; }

				char c = ExplosionShape[stage][dy + 1][dx + 1];
				if (c == ' ') continue; //빈 칸은 투명하게 둠

				_BG[y][x].Text = c;
				_BG[y][x].Color= ExplosionColor[stage];
			}
		}
	}

	if (PopupTimer > 0) {
		int elapsed = POPUP_FRAME - PopupTimer;
		int y = PopupPos.Y - 1 - elapsed / POPUP_RISE_INTERVAL;
		int titleY = PopupShotUp ? y - 1 : y;
		if (titleY < 0) { y -= titleY; titleY = 0; }

		unsigned short color = (PopupTimer <= POPUP_FADE_FRAME)
			? 0x08 : PopColors[(elapsed / 3) % POPUP_COLOR_COUNT];

		DrawPopupText(_BG, PopupPos.X, titleY, "* LEVEL UP! *", color);
		if (PopupShotUp) {
			DrawPopupText(_BG, PopupPos.X, y, "SHOT +1", (PopupTimer <= POPUP_FADE_FRAME) ? 0x08 : 0x0F);
		}
	}
}

void EffectReset()
{
	for (int i = 0; i < MAX_EFFECT;++i) {
		effects[i].Active = false;
	}
}

void LevelUpSpawn(COORD pos, bool shotUp) {
	PopupPos = pos;
	PopupTimer = POPUP_FRAME;
	PopupShotUp = shotUp;
}

