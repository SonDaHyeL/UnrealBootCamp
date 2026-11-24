#pragma once

#include "Struct.h"

void ScoreAdd(int amount);
void UIDraw(Tile(*_BG)[DF_BG_SIZEX]);
void GameOverUI(Tile(*_BG)[DF_BG_SIZEX]);

void ScoreReset();
//메인 타이틀 화면 출력
void TitleDraw(Tile(*_BG)[DF_BG_SIZEX]);
int GetScore();