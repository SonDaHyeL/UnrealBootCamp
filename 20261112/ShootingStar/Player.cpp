#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
static const char* PlayerShape = "~(0,0)>>";
static const char* PlayerDeadShape = "~(X,X)>>";
static const int PlayerWidth = 8;


static int FireCooldown = 0; //발사 후 남은 대기 프레임
static int MoveYDelay = 0; //세로 이동 후 남은 대기 프레임
static int PlayerHP = PLAYER_MAXHP; //플레이어 체력

static int NAttackTimer = 0; //잔여 무적시간 프레임 타이머
static const int NATTACK_FRAME = 45; //무적시간 45프레임(약 1.5초)
static const int BLINK_INTERVAL = 4; //깜빡임 주기

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


	//적과 충돌시 체력 감소
	//무적 시간 감소
	if (NAttackTimer > 0) --NAttackTimer;
	
	//무적이 아닐때에만 충돌 감지.
	if (NAttackTimer == 0 && EnemyHitCheck(player.Position, PlayerWidth)) {
		if (PlayerHP > 0) PlayerHP--;
		NAttackTimer = NATTACK_FRAME;
	}
}


void PlayerDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	bool isDead = (PlayerHP <= 0);

	unsigned short PlayerColor = 0x0E; //노란색!
	if (!isDead && NAttackTimer > 0 &&
		(NAttackTimer / BLINK_INTERVAL) % 2 == 0) {
		PlayerColor = 0x0C;
	}


	const char* shape = isDead ? PlayerDeadShape : PlayerShape;
	

	for(int i = 0; i < PlayerWidth; i++)
	{
		_BG[player.Position.Y][player.Position.X + i].Text = shape[i];
		_BG[player.Position.Y][player.Position.X + i].Color = PlayerColor;
	}
}

int PlayerGetHP()
{
	return PlayerHP;
}
