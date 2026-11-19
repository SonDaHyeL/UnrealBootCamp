#ifndef SAVELOAD_H
#define SAVELOAD_H

#include "Player.h"

// 플레이어 진행도를 텍스트 파일로 저장한다. 실패하면 false.
bool SaveData(const Player& player);

// 저장 파일에서 플레이어 진행도를 불러온다.
// 파일이 없거나, 버전이 다르거나, 읽기 중 실패하면 false (이 경우 player는 건드리지 않음).
bool LoadData(Player& player);

// 저장 파일이 존재하는지 확인한다. ("이어하기" 메뉴 표시 여부 판단용)
bool HasSaveData();

#endif // !SAVELOAD_H
