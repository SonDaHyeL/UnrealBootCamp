#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include <string>

const int MAX_LEVEL = 100;

struct Player {
	std::string name = "";
	std::string jobName = "";

	int level = 1;		int exp = 0;

	int job = 0;
	int attack = 0;		int defense = 0;		int basehp = 0;
	int growAtk = 0;		int growDef = 0;	int growHP = 0;

	int hp = 0;

	int gold = 0;
	bool quest = false;

	ITEM inven[INVEN_SIZE];
	ITEM equip[EQUIPSLOT_END];

	void Init();
	void ShowInfo();
	void AddGold(int amount) { gold += amount; };
	int GetAttack() const { return attack + growAtk * (level - 1); }
	int GetDefense() const { return defense + growDef * (level - 1); }
	int GetMaxHp() const { return basehp + growHP * (level - 1); }
	int GetNeedExp() const { return level * 20; } //다음 레벨까지 경험치 요구량
};

void PlayerInit(Player& player);
void GainExp(Player& player, int amount);


#endif


