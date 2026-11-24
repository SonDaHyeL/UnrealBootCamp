#pragma once
#include "Struct.h"

void EffectSpawn(COORD center);
void EffectUpdate();
void EffectDraw(Tile(*_BG)[DF_BG_SIZEX]);
void EffectReset();