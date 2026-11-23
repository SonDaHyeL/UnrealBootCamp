#include "Bullet.h"
#include "Enemy.h"
#include "UI.h"

#define MAX_BULLET 30 //한 화면에 나올 수 이는 최대 총알 갯수
#define BULLET_SPEED 2 //총알 속도
static Bullet bullets[MAX_BULLET] = {}; //총알을 미리 만들어두고 시작함 무한 발사를 막기 위함


void BulletFire(COORD pos)
{
	for (int i = 0; i < MAX_BULLET; ++i) {
		if (!bullets[i].Active) {
			bullets[i].Active = true;
			bullets[i].Position = pos;
			return;
		}
	}
}

void BulletUpdate()
{
	for (int i = 0; i < MAX_BULLET; ++i) {
		if (!bullets[i].Active) continue;

		if (EnemyHitCheck(bullets[i].Position)) {
			bullets[i].Active = false;
			ScoreAdd(100);
			continue;
		}

		for (int j = 0; j < BULLET_SPEED; ++j) {
			bullets[i].Position.X++;
			if (bullets[i].Position.X >= DF_BG_SIZEX) {
				bullets[i].Active = false; //화면 밖으로 나가면 활동 종료
				break;
			}
			if (EnemyHitCheck(bullets[i].Position)) {
				bullets[i].Active = false;
				ScoreAdd(100);
				break;
			}
		}
		
	}
}

void BulletDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	for (int i = 0; i < MAX_BULLET; ++i) {
		if (!bullets[i].Active) continue;
		Tile& t = _BG[bullets[i].Position.Y][bullets[i].Position.X];
		t.Text = '-';
		t.Color = 0x0A; //밝은 빨강색
	}
}
