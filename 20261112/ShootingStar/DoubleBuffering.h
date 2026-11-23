#pragma once

#include "Struct.h"
#include <Windows.h>
void ScreenInit();
void ScreenPrint(Tile(*_BG)[DF_BG_SIZEX]);
void ScreenClear();
void ScreenFlipping();
void ScreenRelease();