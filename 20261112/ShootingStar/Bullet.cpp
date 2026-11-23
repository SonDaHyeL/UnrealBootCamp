#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "UI.h"
#include <cmath> //fabsf, ceilf

#define MAX_BULLET 200 //한 화면에 나올 수 있는 최대 총알 갯수
static Bullet bullets[MAX_BULLET] = {}; //총알을 미리 만들어두고 시작함 무한 발사를 막기 위함


static void EnemyKilled() {
	ScoreAdd(100);
	AddExp(10);
}

//pos 위치에서 (vx, vy) 벡터로 총알 발사
void BulletFire(COORD pos, float vx, float vy)
{
	for (int i = 0; i < MAX_BULLET; ++i) {
		if (!bullets[i].Active) {
			bullets[i].Active = true;
			bullets[i].X = pos.X;
			bullets[i].Y = pos.Y;
			bullets[i].vx = vx;
			bullets[i].vy = vy;
			return;
		}
	}
}

//게임 영역 밖으로 나갔는지 확인 (소수 좌표 기준)
static bool IsOutOfScreen(const Bullet& b)
{
	return b.X < 0 || b.X >= DF_BG_SIZEX || b.Y < 0 || b.Y >= DF_BG_SIZEY;
}

void BulletUpdate()
{
	for (int i = 0; i < MAX_BULLET; ++i) {
		Bullet& b = bullets[i];
		if (!b.Active) continue;

		//이동 전에 검사 (적이 총알 쪽으로 다가와 겹친 경우)
		if (EnemyHitCheck(BulletCoord(b))) {
			b.Active = false;
			EnemyKilled();
			continue;
		}

		//한 번에 1칸 이하가 되도록 쪼개서 이동 -> 적을 뚫고 지나가지 않게
		float big = fabsf(b.vx) > fabsf(b.vy) ? fabsf(b.vx) : fabsf(b.vy);
		int steps = (int)ceilf(big);
		if (steps < 1) steps = 1;

		for (int s = 0; s < steps; ++s) {
			b.X += b.vx / steps;
			b.Y += b.vy / steps;

			if (IsOutOfScreen(b)) {
				b.Active = false; //화면 밖으로 나가면 활동 종료
				break;
			}
			if (EnemyHitCheck(BulletCoord(b))) {
				b.Active = false;
				EnemyKilled();
				break;
			}
		}
	}
}

void BulletDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	for (int i = 0; i < MAX_BULLET; ++i) {
		if (!bullets[i].Active) continue;

		COORD c = BulletCoord(bullets[i]);
		Tile& t = _BG[c.Y][c.X];

		t.Text = '*';
		t.Color = 0x0A; //밝은 초록색
	}
}
