#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Inventory.h"
#include "Character.h"
#include <string>

const int MAX_LEVEL = 100;
const int JOB_COUNT = 4;
const char* const JOB_NAME[JOB_COUNT + 1] = { "", "전사", "궁수", "도적","마법사" };


class Player : public Character {
private:
	void Init(); //캐릭터 초기화 
	std::string jobName = "";

	int level = 1;		int exp = 0;

	int job = 0;
	int attack = 0;		int defense = 0;		int basehp = 0;
	int growAtk = 0;		int growDef = 0;	int growHP = 0;

	int mp = 0;
	int baseMp = 0; int growMp = 0;

	int gold = 0;
	int questId = 0;          // 진행 중인 퀘스트 ID (0 = 없음)
	int questKills = 0;       // 진행 중인 퀘스트의 처치 수
	int lastQuestDone = 0;    // 마지막으로 완료한 퀘스트 ID (다음 의뢰를 정할 때 사용)

	//아이템 종류와 장비칸 번호 매칭. 장비가 아닐 시 -1 리턴
	int FindEquipSlot(ITEMTYPE type) const;

	Inventory inven;
	ITEM equip[EQUIPSLOT_END];

public:
	void Create(); //캐릭터 생성
	
	//***캐릭터 정보 조회***
	int GetLevel() const { return level; }
	const std::string& GetJobName() const { return jobName; }
	int GetGold() const { return gold; }
	int GetAttack() const override { return attack + growAtk * (level - 1) + getEquipStats().atk; } //레벨 + 장비에 따른 공격력 계산 
	int GetDefense() const override { return defense + growDef * (level - 1) + getEquipStats().def; } //레벨 + 정바에 따른 방어력 계산
	int GetMaxHp() const override { return basehp + growHP * (level - 1) + getEquipStats().hp; } //레벨 + 장비에 따른 최대 체력 계산
	int GetNeedExp() const { return level * 20; } //다음 레벨까지 경험치 요구량

	//마나 관리
	int GetMp() const { return mp; }
	int GetMaxMp() const { return baseMp + growMp * (level - 1); }
	bool SpendMp(int amount);           // 모자라면 false (안 씀)
	void RestoreMp(int amount);         // 최대 MP까지

	std::vector<int> GetSkills() const; // 지금 레벨까지 배운 스킬 ID들

	void ShowInfo() const; //캐릭터 정보창 출력

	//***퀘스트***
	bool HasQuest() const { return questId != 0; }
	int GetQuestId() const { return questId; }
	int GetQuestKills() const { return questKills; }
	int GetLastQuestDone() const { return lastQuestDone; }

	void AcceptQuest(int id) { questId = id; questKills = 0; }
	void AddQuestKill() { ++questKills; }
	void CompleteQuest() { lastQuestDone = questId; questId = 0; questKills = 0; }

	//*** HP ***
	void Revive() { hp = 1; } //사망 후 부활

	//*** 골드 ***
	void AddGold(int amount) { gold += amount; };
	bool SpendGold(int amount);


	//***경험치***
	void AddExp(int amount);
	
	//***인벤토리***
	Inventory& GetInven() { return inven; }
	const Inventory& GetInven() const { return inven; }


	//***장비***
	const ITEM& GetEquip(int slot) const { return equip[slot]; }
	bool Equip(int invenIdx); //인벤토리 -> 장비칸 (장착)
	bool Unequip(int slot); // 장비칸 -> 인벤토리 (해제)

	// 장착 장비 능력치 총합계
	struct EquipStats
	{
		int atk = 0;
		int def = 0;
		int hp = 0;
	};
	EquipStats getEquipStats() const;
	
	// 저장/불러오기는 모든 필드를 직접 읽고 써야 해서 예외로 허용
	friend bool SaveData(const Player& player);
	friend bool LoadData(Player& player);

};



#endif