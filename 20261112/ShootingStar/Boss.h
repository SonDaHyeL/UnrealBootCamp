#pragma once
#pragma once
#include "Struct.h"

void BossUpdate();
void BossDraw(Tile(*_BG)[DF_BG_SIZEX]);
void BossReset();

bool BossIsActive();                       //보스전 진행 중인지 (일반 적 생성 멈춤용)
bool BossBulletHit(COORD pos);             //플레이어 총알과 충돌
bool BossBodyHit(COORD pos, int width);    //플레이어 몸통과 충돌