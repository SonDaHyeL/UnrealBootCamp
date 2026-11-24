#include "Boss.h"
#include "Bullet.h"
#include "Effect.h"
#include "Player.h"
#include "UI.h"
#include <cstring>
#include <cmath>

//R"B( ... )B" : 그림 안에 ) 와 \ 가 많아서 구분자 B를 붙인 raw string 사용
static const char* BossShape[] = {
	R"B(    /\_            _/\)B",
	R"B(   /   \___^\___/    \)B",
	R"B(  (                    ))B",
	R"B( (     o   ____   o     ))B",
	R"B( (          \/          ))B",
	R"B(  (                    ))B",
	R"B(     \__/\__/\__/\__/)B",
};
static const int BOSS_H = sizeof(BossShape) / sizeof(BossShape[0]);
static const int MOUTH_ROW = 4;            //입(\/)이 있는 줄 -> 총알 발사 위치

static const int BOSS_SCORE = 10000;       //등장 점수 간격
static const int BOSS_MAXHP = 60;
static const int WARNING_FRAME = 120;      //경고 표시 시간 (약 2초)
static const int DYING_FRAME = 60;         //폭발 연출 시간
static const int REWARD_SCORE = 5000;
static const int REWARD_EXP = 200;

enum BossPhase { BOSS_NONE, BOSS_WARNING, BOSS_ENTER, BOSS_FIGHT, BOSS_DYING };

static BossPhase Phase = BOSS_NONE;
static float BX = 0, BY = 0;               //보스 그림의 왼쪽 위 좌표
static float MoveDir = 1.0f;               //+1 아래, -1 위
static int HP = 0;
static int Timer = 0;                      //단계별 남은 프레임
static int FireTimer = 0;
static int Pattern = 0;
static int HitFlash = 0;
static int NextBossScore = BOSS_SCORE;

//──── 유틸 ────
static int BossWidth() {
	int w = 0;
	for (int i = 0; i < BOSS_H; ++i) {
		int len = (int)strlen(BossShape[i]);
		if (len > w) w = len;
	}
	return w;
}

//row 줄에서 공백이 아닌 첫 글자와 마지막 글자 위치 -> 외곽선 안쪽만 그리고, 판정함
static void RowSpan(int row, int& first, int& last) {
	const char* s = BossShape[row];
	int len = (int)strlen(s);
	first = 0;       while (first < len && s[first] == ' ') ++first;
	last = len - 1;  while (last >= 0 && s[last] == ' ') --last;
}

static bool IsRage() { return HP <= BOSS_MAXHP / 2; }
static int TargetX() { return DF_BG_SIZEX - BossWidth() - 4; }

static void DrawText(Tile(*_BG)[DF_BG_SIZEX], int x, int y, const char* s, unsigned short color) {
	for (int i = 0; s[i] != '\0'; ++i) {
		if (x + i < 0 || x + i >= DF_BG_SIZEX) continue;
		_BG[y][x + i].Text = s[i];
		_BG[y][x + i].Color = color;
	}
}

//──── 공격 패턴 ────
static void BossFire()
{
	COORD mouth = { (SHORT)(BX - 1), (SHORT)(BY + MOUTH_ROW) };

	switch (Pattern) {
	case 0: //부채꼴 7발
		for (int k = -3; k <= 3; ++k)
			BulletFire(mouth, -1.0f, k * 0.25f, ETEAM);
		break;

	case 1: { //플레이어 조준 3연발
		COORD p = PlayerGetPosition();
		float dx = (float)(p.X - mouth.X);
		float dy = (float)(p.Y - mouth.Y);
		float len = sqrtf(dx * dx + dy * dy);
		if (len < 1.0f) len = 1.0f;
		const float SPEED = 1.2f;
		float vx = dx / len * SPEED;   //방향(단위 벡터) x 속도
		float vy = dy / len * SPEED;
		for (int k = -1; k <= 1; ++k)
			BulletFire(mouth, vx, vy + k * 0.15f, ETEAM);
		break;
	}

	case 2: //세로 탄막 벽 : 보스 키만큼 한 줄로
		for (int row = 0; row < BOSS_H; ++row)
			BulletFire({ (SHORT)(BX - 1), (SHORT)(BY + row) }, -0.8f, 0.0f, ETEAM);
		break;
	}
	Pattern = (Pattern + 1) % 3;
}

//──── 업데이트 ────
void BossUpdate()
{
	switch (Phase) {
	case BOSS_NONE:
		if (GetScore() >= NextBossScore) {
			Phase = BOSS_WARNING;
			Timer = WARNING_FRAME;
		}
		break;

	case BOSS_WARNING:
		if (--Timer <= 0) {
			Phase = BOSS_ENTER;
			BX = (float)DF_BG_SIZEX;                    //화면 오른쪽 밖에서 시작
			BY = (float)((DF_BG_SIZEY - BOSS_H) / 2);
			HP = BOSS_MAXHP;
			HitFlash = 0;
			FireTimer = 60;
			Pattern = 0;
		}
		break;

	case BOSS_ENTER:
		BX -= 0.5f;
		if (BX <= TargetX()) {
			BX = (float)TargetX();
			Phase = BOSS_FIGHT;
		}
		break;

	case BOSS_FIGHT: {
		if (HitFlash > 0) --HitFlash;

		//위아래 왕복. 맨 위 2줄은 HP 바 자리라 비워둠
		float speed = IsRage() ? 0.5f : 0.25f;
		BY += MoveDir * speed;
		if (BY < 2) { BY = 2; MoveDir = 1.0f; }
		if (BY > DF_BG_SIZEY - BOSS_H) { BY = (float)(DF_BG_SIZEY - BOSS_H); MoveDir = -1.0f; }

		if (--FireTimer <= 0) {
			BossFire();
			FireTimer = IsRage() ? 40 : 70;
		}
		break;
	}

	case BOSS_DYING:
		if (Timer % 4 == 0) { //4프레임마다 몸 여기저기서 폭발
			EffectSpawn({ (SHORT)(BX + rand() % BossWidth()), (SHORT)(BY + rand() % BOSS_H) });
		}
		if (--Timer <= 0) {
			Phase = BOSS_NONE;
			NextBossScore = GetScore() + BOSS_SCORE;
		}
		break;
	}
}

//──── 그리기 ────
static void DrawBossHP(Tile(*_BG)[DF_BG_SIZEX])
{
	const int BAR_W = 50;
	int x = (DF_BG_SIZEX - (BAR_W + 5)) / 2;
	DrawText(_BG, x, 0, "BOSS", 0x0C);

	int filled = HP * BAR_W / BOSS_MAXHP;
	for (int i = 0; i < BAR_W; ++i) {
		Tile& t = _BG[0][x + 5 + i];
		t.Text = ' ';
		t.Color = (i < filled) ? 0xC0 : 0x80; //배경 빨강 / 배경 회색
	}
}

void BossDraw(Tile(*_BG)[DF_BG_SIZEX])
{
	if (Phase == BOSS_NONE) return;

	if (Phase == BOSS_WARNING) {
		if ((Timer / 10) % 2 == 0) {
			const char* w1 = "!!   W A R N I N G   !!";
			const char* w2 = "A HUGE ENEMY IS APPROACHING";
			DrawText(_BG, (DF_BG_SIZEX - (int)strlen(w1)) / 2, DF_BG_SIZEY / 2 - 1, w1, 0x0C);
			DrawText(_BG, (DF_BG_SIZEX - (int)strlen(w2)) / 2, DF_BG_SIZEY / 2 + 1, w2, 0x0E);
		}
		return;
	}

	if (Phase == BOSS_DYING && (Timer / 3) % 2 == 0) return; //깜빡이며 사라짐

	bool rage = IsRage();
	unsigned short body = rage ? 0x0C : 0x0D;
	unsigned short eye = (rage && (GetTickCount64() / 150) % 2) ? 0x0C : 0x0E;
	if (HitFlash > 0) body = eye = 0x0F;

	int ox = (int)BX, oy = (int)BY;
	for (int row = 0; row < BOSS_H; ++row) {
		int first, last;
		RowSpan(row, first, last);
		for (int col = first; col <= last; ++col) {   //외곽선 바깥 공백은 투명, 안쪽 공백은 칠함
			int x = ox + col, y = oy + row;
			if (x < 0 || x >= DF_BG_SIZEX || y < 0 || y >= DF_BG_SIZEY) continue;

			char c = BossShape[row][col];
			_BG[y][x].Text = c;
			_BG[y][x].Color = (c == 'o') ? eye : body;
		}
	}

	if (Phase != BOSS_DYING) DrawBossHP(_BG);
}

//──── 충돌 ────
static bool BossOverlap(COORD pos, int width)
{
	if (Phase != BOSS_ENTER && Phase != BOSS_FIGHT) return false;

	int row = pos.Y - (int)BY;
	if (row < 0 || row >= BOSS_H) return false;

	int first, last;
	RowSpan(row, first, last);
	int left = (int)BX + first;
	int right = (int)BX + last;
	return pos.X <= right && left < pos.X + width;
}

bool BossBulletHit(COORD pos)
{
	if (!BossOverlap(pos, 1)) return false;
	if (Phase == BOSS_ENTER) return true;   //등장 중에는 무적 (총알은 막힘)

	HitFlash = 2;
	if (--HP <= 0) {
		Phase = BOSS_DYING;
		Timer = DYING_FRAME;
		ScoreAdd(REWARD_SCORE);
		AddExp(REWARD_EXP);
	}
	return true;
}

bool BossBodyHit(COORD pos, int width) { return BossOverlap(pos, width); }
bool BossIsActive() { return Phase != BOSS_NONE; }

void BossReset()
{
	Phase = BOSS_NONE;
	NextBossScore = BOSS_SCORE;
	HP = 0;
}