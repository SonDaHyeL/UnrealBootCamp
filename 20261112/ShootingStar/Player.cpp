#include "Player.h"
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
	//if(!_kbhit)
	char PressKey = _getch();
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
	default:
		break;
	}
}



void PlayerDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	const char* Shape = "~(0,0)>>";
	unsigned short PlayerColor = 0x0E; //노란색!
	for(int i = 0; i < PlayerWidth; i++)
	{
		_BG[player.Position.Y][player.Position.X + i].Text = PlayerShape[i];
		_BG[player.Position.Y][player.Position.X + i].Color = PlayerColor;
	}
}
