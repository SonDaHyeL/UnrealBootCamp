#pragma once

#include <string>
#include "Object.h"

// quests.csv 한 줄
struct QuestData {
	bool exists = false;
	std::string name;
	int target = 0;              // 대상 몬스터 ID (monsters.csv)
	int count = 1;               // 목표 처치 수
	int rewardGold = 0;
	int rewardExp = 0;
	ITEMLIST rewardItem = noItem;
	std::string request;         // 의뢰 대사
	std::string complete;        // 완료 대사
};

// ===== 퀘스트 표 (DataTable.cpp가 quests.csv를 읽어 채운다) =====
const QuestData& GetQuestData(int id);
int GetNextQuestId(int lastDone);    // lastDone 다음 퀘스트 ID. 더 없으면 0

