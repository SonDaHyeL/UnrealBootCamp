#pragma once
#include "Struct.h"


void PlayerStart(COORD pos);
void PlayerUpdate();
void PlayerDraw(Tile(*_BG)[DF_BG_SIZEX]);
int PlayerGetHP();
bool PlayerHitCheck(COORD pos);


int GetPlayerLevel();
int GetPlayerExp();
int NeedNextExp();
void AddExp(int amount);