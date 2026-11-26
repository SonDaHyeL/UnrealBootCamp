#pragma once
#include <string>

// 게임 시작 때 한 번 호출: items.csv 등 데이터 파일을 읽는다.
// 실패하면 error에 이유(파일, 줄 번호)를 담고 false.
bool LoadGameData(std::string& error);