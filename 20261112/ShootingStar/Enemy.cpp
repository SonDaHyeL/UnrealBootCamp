#include "Bullet.h"
#include "Enemy.h"
#include "Effect.h"
#include "Player.h"
#include "UI.h"
#include <cstring>

#define MAX_ENEMY 20
static Enemy enemys[MAX_ENEMY] = {};



//Enemy 종류별 구조체
struct EnemyInfo {
	const char* Shape;
	unsigned short Color;
	int HP;
	int MoveDelay;
	int FireInterval;
	int Score;
	int Exp;
};

static const EnemyInfo EnemyTable[ENEMY_TYPE_COUNT] = {
	//{"모양" 색상 HP 이동 발사 점수 EXP }
	{ "<=0", 0x0D, 1, 3, 3000, 100, 10 },//ENEMY_NOMAL
	{ "<{##}",0x09, 3, 4, 4000, 300, 10 },//ENEMY_GUNNER

};

static const int UNLOCK_SCORE = 5000;
static const int GUNNER_RATE = 30; //GUNNER 등장 확률
static const int HIT_FLASH_FRAME = 3;

static int SpawnTimer = 0;

static const EnemyInfo& Info(const Enemy& e) { return EnemyTable[e.Type]; }
static int Width(const Enemy& e) { return (int)strlen(Info(e).Shape); }

void EnemyReset() {
	for (int i = 0; i < MAX_ENEMY; ++i) { enemys[i].Active = false; }
	SpawnTimer = 0;
}

void EnemySpawn() {
	EnemyType type = ENEMY_NOMAL;
	if (GetScore() >= UNLOCK_SCORE && rand() % 100 < GUNNER_RATE) {
		type = ENEMY_GUNNER;
	}

	for (int i = 0; i < MAX_ENEMY; ++i) {
		Enemy& e = enemys[i];
		if (e.Active) continue;

		e.Active = true;
		e.Type = type;
		e.HP = EnemyTable[type].HP;
		e.MoveTimer = 0;
		e.HitFlash = 0;
		e.Position.X = (SHORT)(DF_BG_SIZEX - (int)strlen(EnemyTable[type].Shape));
		e.Position.Y = (SHORT)(rand() % DF_BG_SIZEY);
		e.NextFireTick = GetTickCount64() + 1000 + rand() % 2000;
		return;
	}
}

//***적 공격. 종류별 패턴
static void EnemyFire(const Enemy& e) {
	COORD muzzle = { (SHORT)(e.Position.X - 1),e.Position.Y };
	if (muzzle.X < 0) return;

	switch (e.Type)
	{
	case ENEMY_NOMAL:
		BulletFire(muzzle, -1.0f, 0.0f, ETEAM);
		break;
	case ENEMY_GUNNER:
		for (int k = -1; k <= 1; ++k)
		BulletFire(muzzle, -1.0f, k * 0.3f, ETEAM);
		break;
	default:
		break;
	}
}


void EnemyUpdate() {
	if (++SpawnTimer >= 20) {
		SpawnTimer = 0;
		EnemySpawn();
	}

	ULONGLONG now = GetTickCount64();
	for (int i = 0; i < MAX_ENEMY;++i) {
		Enemy& e = enemys[i];
		if (!e.Active) continue;

		if (e.HitFlash > 0) --e.HitFlash;

		if (now >= e.NextFireTick) {
			EnemyFire(e);
			e.NextFireTick = now + Info(e).FireInterval;
		}

		if (++e.MoveTimer >= Info(e).MoveDelay) {
			e.MoveTimer = 0;
			e.Position.X--;
			if (e.Position.X < 0) e.Active = false;
		}
	}
}

void EnemyDraw(Tile(*_BG)[DF_BG_SIZEX]) {
	for (int i = 0; i < MAX_ENEMY; ++i) {
		const Enemy& e = enemys[i];
		if (!e.Active) continue;

		unsigned short color = (e.HitFlash > 0) ? 0x0F : Info(e).Color;

		int w = Width(e);
		for (int j = 0; j < w; ++j) {
			Tile& t = _BG[e.Position.Y][e.Position.X + j];
			t.Text = Info(e).Shape[j];
			t.Color = color;
		}
	}
}


// 충돌 검사
static bool Overlap(const Enemy& e, COORD pos, int width) {
	return pos.Y == e.Position.Y &&
		pos.X < e.Position.X + Width(e) &&
		e.Position.X < pos.X + width;
}

static void EnemyKill(Enemy& e) {
	EffectSpawn({ (SHORT)(e.Position.X + Width(e) / 2),e.Position.Y });
	e.Active = false;
}


// 플레이어 몸통과 충돌한 경우 - 체력과 무관 즉시 파괴, 보상 없음. 
bool EnemyHitCheck(COORD pos, int width) {
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (enemys[i].Active && Overlap(enemys[i], pos, width)) {
			EnemyKill(enemys[i]);
			return true;
		}
	}
	return false;
}


// 플레이어 총알과 충돌한 경우 - 체력-1 0일시 파괴+보상 
bool EnemyBulletHit(COORD pos) {
	for (int i = 0; i < MAX_ENEMY;++i) {
		Enemy& e = enemys[i];
		if (!e.Active || !Overlap(e, pos, 1)) continue;

		if (--e.HP <= 0) {
			ScoreAdd(Info(e).Score);
			AddExp(Info(e).Exp);
			EnemyKill(e);
		}
		else {
			e.HitFlash = HIT_FLASH_FRAME;
		}
		return true;
	}
	return false;
}