#include "Effect.h"

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
}

void EffectUpdate()
{
	for (int i = 0; i < MAX_EFFECT;++i) {
		if (!effects[i].Active) continue;

		if (++effects[i].Frame >= STAGE_COUNT * FRAMES_PER_STAGE) {
			effects[i].Active = false; //현재 총 9프레임 후 종료
		}
	}
	
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
}

void EffectReset()
{
	for (int i = 0; i < MAX_EFFECT;++i) {
		effects[i].Active = false;
	}
}
