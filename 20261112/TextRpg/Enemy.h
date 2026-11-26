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
	//explicit : 생성자나 변환 함수가 의도치 않게 자동 형변환되는 것을 막는 키워드 (암시적 형변환(Implicit Conversion)을 막음)
	// 발자가 명시적으로 객체를 생성하도록 강제하는 키워드!
	explicit Enemy(int type);

	int GetAttack() const override { return attack; }
	int GetDefense() const override { return defense; }
	int GetMaxHp() const override { return maxHp; }

	ITEMLIST GetDropItem() const { return dropItem; }
	int GetDropExp() const { return dropExp; }

};

#endif // !ENEMY_H
