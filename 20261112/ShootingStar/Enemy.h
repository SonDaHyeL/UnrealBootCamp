#pragma once
#include "Struct.h"

void EnemySpawn();
void EnemyUpdate();
void EnemyDraw(Tile(*_BG)[DF_BG_SIZEX]);
bool EnemyHitCheck(COORD pos, int width = 1);

void EnemyReset();

//적 총알 발사

void EBulletFire(COORD pos);
void EBulletUpdate();
void EBulletDraw(Tile(*_BG)[DF_BG_SIZEX]);