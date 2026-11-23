#include "Bullet.h"
#include "Enemy.h"

#define MAX_ENEMY 20
static Enemy enemys[MAX_ENEMY] = {};

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
	if (++MoveDelay < 2) return; //Enemy 속도 조절 (2프레임에 1칸)
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
