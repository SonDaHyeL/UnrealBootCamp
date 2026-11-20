#include "Player.h"
#include "Bullet.h"
static const char* PlayerShape = "~(0,0)>>";
static const int PlayerWidth = 8;

enum KeyType {
	SPACE_KEY = 32,
	UP_KEY = 72,
	LEFT_KEY = 75,
	RIGHT_KEY = 77,
	DOWN_KEY = 80,
};

static Player player = {};

void PlayerStart(COORD pos)
{
	player.Position = pos;
}



void PlayerUpdate()
{
	if (!_kbhit()) return; //눌린 키가 없으면 바로 리턴. 화면이 플레이어가 누르지 않아도 진행되게 함.

	int PressKey = _getch();
	if(PressKey == 224) PressKey = _getch();

	switch (PressKey)
	{
	case LEFT_KEY:
		if(player.Position.X > 0) player.Position.X--;
		break;
	case RIGHT_KEY:
		if (player.Position.X < DF_BG_SIZEX - PlayerWidth) player.Position.X++;
		break;
	case UP_KEY:
		if (player.Position.Y > 0)
			player.Position.Y--;
		break;
	case DOWN_KEY:
		if (player.Position.Y < DF_BG_SIZEY - 1) player.Position.Y++;
		break;
	case SPACE_KEY:
		BulletFire({ (SHORT)(player.Position.X + PlayerWidth),player.Position.Y });
		break;
	default:
		break;
	}
}



void PlayerDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	unsigned short PlayerColor = 0x0E; //노란색!
	for(int i = 0; i < PlayerWidth; i++)
	{
		_BG[player.Position.Y][player.Position.X + i].Text = PlayerShape[i];
		_BG[player.Position.Y][player.Position.X + i].Color = PlayerColor;
	}
}
