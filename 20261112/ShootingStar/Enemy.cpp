#include "Bullet.h"
#include "Enemy.h"
#include "Effect.h"
#include "Player.h"

#define MAX_ENEMY 20
static Enemy enemys[MAX_ENEMY] = {};

static const char* EnemyShape = "<=O";
static const int EnemyWidth = 3;

static int SpawnTimer = 0;
static int MoveDelay = 0;

void EnemyReset() {
	for (int i = 0; i < MAX_ENEMY; ++i) {
		enemys[i].Active = false;
	}
	
	SpawnTimer = 0;
	MoveDelay = 0;
}

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
	if (++SpawnTimer >= 20) { //20프레임 당 한 개체 생성
		SpawnTimer = 0;
		EnemySpawn();
	}

	//총알 발사.
	ULONGLONG now = GetTickCount64();
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!enemys[i].Active) continue;

		if (now >= enemys[i].NextFireTick) {
			BulletFire({ (SHORT)(enemys[i].Position.X - 1),enemys[i].Position.Y }, -1.0f, 0.0f, ETEAM);
			enemys[i].NextFireTick = now + 3000;
		}
	}

	if (++MoveDelay < 3) return; //Enemy 속도 조절 (2프레임에 1칸)
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
			EffectSpawn({ (SHORT)(enemys[i].Position.X + EnemyWidth / 2),enemys[i].Position.Y });
			enemys[i].Active = false;
			return true;
		}
	}
	return false;
}


