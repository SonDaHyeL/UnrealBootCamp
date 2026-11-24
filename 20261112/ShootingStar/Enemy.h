#pragma once
#include "Struct.h"

void EnemySpawn();
void EnemyUpdate();
void EnemyDraw(Tile(*_BG)[DF_BG_SIZEX]);
bool EnemyHitCheck(COORD pos, int width = 1);

void EnemyReset();

bool EnemyBulletHit(COORD pos);