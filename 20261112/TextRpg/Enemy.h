#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "Object.h"
#include "Character.h"

class Enemy : public Character {
private:
	int attack = 0;
	int defense = 0;
	int maxHp = 0;
	ITEMLIST dropItem = noItem;
	int dropExp = 0;

public:
	void Init(int type);

	int GetAttack() const override { return attack; }
	int GetDefense() const override { return defense; }
	int GetMaxHp() const override { return maxHp; }

	ITEMLIST GetDropItem() const { return dropItem; }
	int GetDropExp() const { return dropExp; }

};

#endif // !ENEMY_H
