#pragma once
#include "Struct.h"


void PlayerStart(COORD pos);
void PlayerUpdate();
void PlayerDraw(Tile(*_BG)[DF_BG_SIZEX]);
int PlayerGetHP();
bool PlayerHitCheck(COORD pos);
void PlayerSetShotLevel(int level);
static void PlayerDamage();