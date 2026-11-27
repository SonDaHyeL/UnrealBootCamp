#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <vector>
#include "Object.h"
#include "Character.h"

struct EnemyData {
	std::string name;
	std::string description;
	std::string area;            // 나오는 지역 (예: 들판, 어둠의 숲, 용의 둥지)
	int level = 1;               // 적정 레벨 (사냥터 메뉴 표시용)
	bool boss = false;           // 보스: 도망칠 수 없음

	bool exists = false;
	int attack = 0;
	int defense = 0;
	int maxHp = 0;
	int exp = 0;
	int gold = 0;

	int skill = 0;               // 스킬 ID (skills.csv, 0 = 없음)
	int skillChance = 0;         // 스킬 사용 확률 %
	int skill2 = 0;              // 두 번째 스킬 (스킬1이 안 나왔을 때 굴림)
	int skill2Chance = 0;

	int rareChance = 0;
	ITEMLIST dropItem = noItem;
	ITEMLIST rareItem = noItem;
};


const EnemyData& GetEnemyData(int id);
std::vector<int> GetEnemyIds();
std::vector<std::string> GetAreas();                      // 지역 이름들 (monsters.csv에 처음 나온 순서)
std::vector<int> GetEnemyIdsIn(const std::string& area);  // 그 지역 몬스터 ID들 (레벨 낮은 순)

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
	int GetSkill2() const { return Data().skill2; }
	int GetSkill2Chance() const { return Data().skill2Chance; }

	int GetLevel() const { return Data().level; }
	bool IsBoss() const { return Data().boss; }

	int GetRareChance() const { return Data().rareChance; }

	const std::string& GetDescription() const { return Data().description; }
};

#endif // !ENEMY_H
