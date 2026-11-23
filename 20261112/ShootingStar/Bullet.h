#pragma once
#include "Struct.h"

void BulletFire(COORD pos, float vx, float vy);
void BulletUpdate();
void BulletDraw(Tile(*_BG)[DF_BG_SIZEX]);

void BulletReset();