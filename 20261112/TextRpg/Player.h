#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include <string>

struct Player {
	std::string name = "";
	std::string jobName = "";
	int job = 0;
	int attack = 0;
	int defense = 0;
	int hp = 0;
	int gold = 0;
	bool quest = false;

	ITEM inven[INVEN_SIZE];
	ITEM equip[EQUIPSLOT_END];

	void Init();
	void ShowInfo();
};

void PlayerInit(Player& player);

#endif


