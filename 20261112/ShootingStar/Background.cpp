#include "Background.h"

#define FAR_STAR 60
#define NEAR_STAR 25
#define MAX_STAR (FAR_STAR + NEAR_STAR)

static Star stars[MAX_STAR] = {};

void BackgroundInit()
{
	for (int i = 0; i < MAX_STAR;++i) {
		Star& s = stars[i];
		if (i < FAR_STAR) { s.Speed = 0.15f; s.Text = '.';s.Color = 0x08; }
		else { s.Speed = 0.5f; s.Text = '.';s.Color = 0x07; }

		s.X = (float)(rand() % DF_BG_SIZEX);
		s.Y = (SHORT)(rand() % DF_BG_SIZEY);
	}
}

void BackgroundUpdate()
{
	for (int i = 0; i < MAX_STAR; ++i) {
		Star& s = stars[i];
		s.X -= s.Speed;

		if (s.X < 0) { //왼쪽 끝으로 사라지면 오른쪽 끝 랜덤 Y에서 재생성
			s.X += DF_BG_SIZEX;
			s.Y = (SHORT)(rand() % DF_BG_SIZEY);
		}
	}
}

void BackgroundDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	for (int i = 0; i < MAX_STAR; ++i) {
		Tile& t = _BG[stars[i].Y][(int)stars[i].X];
		t.Text = stars[i].Text;
		t.Color = stars[i].Color;
	}
}
