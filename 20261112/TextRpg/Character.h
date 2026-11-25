#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
//player와 enemy의 공통 부모 클래스. 이름과 체력 관리 담당

class Character {
protected:
	std::string name = "";
	int hp = 0;

public:
	virtual ~Character() = default; //상속용 클래스의 소멸자

	//공통 내용 
	const std::string& GetName() const { return name; }
	int GetHp() const { return hp; }
	bool IsDead() const { return hp <= 0; }


	//자식 클래스에서 구현해야 할 것
	virtual int GetAttack() const = 0;
	virtual int GetDefense() const = 0;
	virtual int GetMaxHp() const = 0;

	//공통 동작
	void TakeDamage(int amount) {
		hp -= amount;
		if (hp < 0) hp = 0;
	}

	void Heal(int amount) {
		hp += amount;
		if (hp > GetMaxHp()) hp = GetMaxHp();
	}
};



#endif