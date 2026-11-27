#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <vector>
#include "Object.h"
#include "Character.h"

struct EnemyData {
	std::string name;
	std::string description;

	bool exists = false;
	int attack = 0;
	int defense = 0;
	int maxHp = 0;
	int exp = 0;
	int gold = 0;

	int skill = 0;               // 스킬 ID (skills.csv, 0 = 없음)
	int skillChance = 0;         // 스킬 사용 확률 %

	int rareChance = 0;
	ITEMLIST dropItem = noItem;
	ITEMLIST rareItem = noItem;
};


const EnemyData& GetEnemyData(int id);
std::vector<int> GetEnemyIds();

class Enemy : public Character {
private:
	int id = 0;
	const EnemyData& Data() const { return GetEnemyData(id); }


public:
	explicit Enemy(int ememyId);

	int GetId() const { return id; }
	int GetAttack() const override { return Data().attack; }
	int GetDefense() const override { return Data().defense; }
	int GetMaxHp() const override { return Data().maxHp; }

	int GetDropExp() const { return Data().exp; }
	int GetDropGold() const { return Data().gold; }
	ITEMLIST GetDropItem() const { return Data().dropItem; }
	ITEMLIST GetRareItem() const { return Data().rareItem; }

	int GetSkill() const { return Data().skill; }
	int GetSkillChance() const { return Data().skillChance; }

	int GetRareChance() const { return Data().rareChance; }

	const std::string& GetDescription() const { return Data().description; }
};

#endif // !ENEMY_H
