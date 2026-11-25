#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Inventory.h"
#include "Character.h"
#include <string>

const int MAX_LEVEL = 100;


class Player : public Character {
private:
	void Init(); //캐릭터 초기화 
	std::string jobName = "";

	int level = 1;		int exp = 0;

	int job = 0;
	int attack = 0;		int defense = 0;		int basehp = 0;
	int growAtk = 0;		int growDef = 0;	int growHP = 0;

	int gold = 0;
	bool quest = false;

	Inventory inven;
	ITEM equip[EQUIPSLOT_END];

public:
	void Create(); //캐릭터 생성
	
	//***캐릭터 정보 조회***
	int GetLevel() const { return level; }
	const std::string& GetJobName() const { return jobName; }
	int GetGold() const { return gold; }
	int GetAttack() const override { return attack + growAtk * (level - 1); } //레벨에 따른 공격력 계산 
	int GetDefense() const override { return defense + growDef * (level - 1); } //레벨에 따른 방어력 계산
	int GetMaxHp() const override { return basehp + growHP * (level - 1); } //레벨에 따른 최대 체력 계산
	int GetNeedExp() const { return level * 20; } //다음 레벨까지 경험치 요구량

	bool HasQuest() const { return quest; } //캐릭터 소지 여부 판정 
	void ShowInfo() const; //캐릭터 정보창 출력


	//*** HP ***
	void Revive() { hp = 1; } //사망 후 부활

	//*** 골드 ***
	void AddGold(int amount) { gold += amount; };
	bool SpendGold(int amount);


	//***경험치와 퀘스트***
	void AddExp(int amount);
	void AcceptQuest() { quest = true; }
	void CompleteQuest() { quest = false; }
	
	//***인벤토리***
	Inventory& GetInven() { return inven; }
	const Inventory& GetInven() const { return inven; }

	// 저장/불러오기는 모든 필드를 직접 읽고 써야 해서 예외로 허용
	friend bool SaveData(const Player& player);
	friend bool LoadData(Player& player);

};



#endif