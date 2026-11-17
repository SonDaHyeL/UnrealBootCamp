#ifndef ENEMY_H
#define ENEMY_H

#include <string>

struct Enemy {
	std::string eName = "";
	int attack = 0;
	int defense = 0;
	int hp = 0;

	void Init(int input);
};

#endif // !ENEMY_H
