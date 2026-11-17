#ifndef PLAYER_H
#define PLAYER_H

#include <string>

struct Player {
	std::string name = "";
	std::string jobName = "";
	int job = 0;
	int attack = 0;
	int defense = 0;
	int hp = 0;
	bool quest = false;

	void Init();
	void ShowInfo();
};

void PlayerInit(Player& player);

#endif


