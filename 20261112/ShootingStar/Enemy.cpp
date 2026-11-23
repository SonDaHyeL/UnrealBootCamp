#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

#define MAX_ENEMY 20
#define MAX_EBULLET 30
static Enemy enemys[MAX_ENEMY] = {};
static Bullet eBullets[MAX_EBULLET] = {};

static const char* EnemyShape = "<=O";
static const int EnemyWidth = 3;

static int SpawnTimer = 0;
static int MoveDelay = 0;

void EnemySpawn()
{
	for (int i = 0; i < MAX_ENEMY;++i) {
		if (!enemys[i].Active) {
			enemys[i].Active = true;
			enemys[i].Position.X = DF_BG_SIZEX - EnemyWidth;
			enemys[i].Position.Y = (SHORT)(rand() % DF_BG_SIZEY);
			enemys[i].NextFireTick = GetTickCount64() + 1000 + rand() % 2000;
			return;
		}
	}
}

void EnemyUpdate()
{
	if (++SpawnTimer >= 50) { //20프레임 당 한 개체 생성
		SpawnTimer = 0;
		EnemySpawn();
	}

	//총알 발사.
	ULONGLONG now = GetTickCount64();
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!enemys[i].Active) continue;

		if (now >= enemys[i].NextFireTick) {
			EBulletFire({ (SHORT)(enemys[i].Position.X - 1),enemys[i].Position.Y });
			enemys[i].NextFireTick = now + 3000;
		}
	}

	if (++MoveDelay < 5) return; //Enemy 속도 조절 (2프레임에 1칸)
	MoveDelay = 0;

	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!enemys[i].Active) continue;

		enemys[i].Position.X--;
		if (enemys[i].Position.X < 0) enemys[i].Active = false;
	}

	
}

void EnemyDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!enemys[i].Active) continue;
		for (int j = 0; j < EnemyWidth; ++j) {

			Tile& t = _BG[enemys[i].Position.Y][enemys[i].Position.X + j];
			t.Text = EnemyShape[j];
			t.Color = 0x0D;
		}
	}
}

bool EnemyHitCheck(COORD pos, int width) {
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!enemys[i].Active) continue;

		if (pos.Y == enemys[i].Position.Y &&
			pos.X < enemys[i].Position.X + EnemyWidth &&
			enemys[i].Position.X < pos.X + width) {
			enemys[i].Active = false;
			return true;
		}
	}
	return false;
}

void EBulletFire(COORD pos)
{
	for (int i = 0; i < MAX_EBULLET; ++i) {
		if (!eBullets[i].Active) {
			eBullets[i].Active = true;
			eBullets[i].Position = pos;
			return;
		}
	}
}

void EBulletUpdate()
{
	for (int i = 0; i < MAX_EBULLET;++i) {
		if (!eBullets[i].Active) { continue; }
		eBullets[i].Position.X--;

		if (eBullets[i].Position.X < 0) { 
			eBullets[i].Active = false; 
			continue;
		}

		if (PlayerHitCheck(eBullets[i].Position)) {
			eBullets[i].Active = false;
		}
	}
}

void EBulletDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	for (int i = 0; i < MAX_EBULLET; ++i) {
		if (!eBullets[i].Active) { continue; }
		Tile& t = _BG[eBullets[i].Position.Y][eBullets[i].Position.X];
		t.Text = '*';
		t.Color = 0x0C; //밝은 빨강색
	}
}
