#pragma once
#include "Struct.h"


void PlayerStart(COORD pos);
void PlayerUpdate();
void PlayerDraw(Tile(*_BG)[DF_BG_SIZEX]);
int PlayerGetHP();