#include "Player.h"
#include "Bullet.h"
static const char* PlayerShape = "~(0,0)>>";
static const int PlayerWidth = 8;


static int FireCooldown = 0; //발사 후 남은 대기 프레임
static int MoveYDelay = 0; //세로 이동 후 남은 대기 프레임

static Player player = {};
static bool IsKeyDown(int vk) {
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

void PlayerStart(COORD pos)
{
	player.Position = pos;
}

void PlayerUpdate()
{
	//새로운 키 입력 방식!
	// 가로 이동 : 매 프레임당 1칸 이동
	if (IsKeyDown(VK_LEFT) && player.Position.X > 0) player.Position.X--;
	if (IsKeyDown(VK_RIGHT) && player.Position.X < DF_BG_SIZEX - PlayerWidth) player.Position.X++;


	// 세로 이동 : 2프레임에 1칸 (콘솔 글자 한 칸은 가로보다 세로가 약 2배 길기 때문에 가로와 같은 속도면 차이가 많이 나 보임)
	if (MoveYDelay > 0) --MoveYDelay;
	if (MoveYDelay == 0) {
		if (IsKeyDown(VK_UP) && player.Position.Y > 0) {
			player.Position.Y--;
			MoveYDelay = 2;
		}
		else if (IsKeyDown(VK_DOWN) && player.Position.Y < DF_BG_SIZEY - 1) {
			player.Position.Y++;
			MoveYDelay = 2;
		}
	}

	// 총알 발사! (키다운시 연사 가능)
	if (FireCooldown > 0) --FireCooldown;
	if (IsKeyDown(VK_SPACE) && FireCooldown == 0) {
		BulletFire({ (SHORT)(player.Position.X + PlayerWidth),player.Position.Y });
		FireCooldown = 3;
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
