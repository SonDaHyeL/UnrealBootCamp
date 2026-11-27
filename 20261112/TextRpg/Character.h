#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
//player와 enemy의 공통 부모 클래스. 이름과 체력 관리 담당

enum class Status{ NONE, POISON, STUN }; //상태 이상
const int POISON_PERCENT = 6; //독: 턴마다 최대 HP의 6% 데미지 (최소 1)


class Character {
protected:
	std::string name = "";
	int hp = 0;
	double guard = 1.0;
	int poisonTurns = 0; //독 남은 턴
	int stunTurns = 0; //기절 남은 턴

public:
	virtual ~Character() = default; //상속용 클래스의 소멸자

	//공통 내용 
	const std::string& GetName() const { return name; }
	int GetHp() const { return hp; }
	bool IsDead() const { return hp <= 0; }
	double GetGuard() const { return guard; }
	void SetGuard(double mul) { guard = mul; }
	void ResetGuard() { guard = 1.0; }

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

	//***상태이상***
	void AddPoison(int turns) { if (turns > poisonTurns) poisonTurns = turns; }   // 이미 걸려 있으면 더 긴 쪽으로
	void AddStun(int turns) { if (stunTurns == 0) stunTurns = turns; }          // 기절 중엔 다시 걸리지 않음 (무한 기절 방지)

	bool IsPoisoned() const { return poisonTurns > 0; }
	bool IsStunned() const { return stunTurns > 0; }
	void CurePoison() { poisonTurns = 0; }
	void CureStun() { stunTurns = 0; }
	void ClearStatus() { poisonTurns = 0; stunTurns = 0; }

	// 턴 시작 때: 독 데미지를 주고 남은 턴을 줄인다. 준 데미지를 돌려줌 (독이 아니면 0)
	int TickPoison() {
		if (poisonTurns <= 0) return 0;
		int dmg = GetMaxHp() * POISON_PERCENT / 100;
		if (dmg < 1) dmg = 1;
		TakeDamage(dmg);
		--poisonTurns;
		return dmg;
	}

	// 턴 시작 때: 기절이면 남은 턴을 줄이고 true (이번 턴 행동 불가)
	bool TickStun() {
		if (stunTurns <= 0) return false;
		--stunTurns;
		return true;
	}

	// 화면 표시용 (예: "[독 2] [기절]"). 없으면 빈 문자열
	std::string GetStatusText() const {
		std::string s;
		if (poisonTurns > 0) s += "[독 " + std::to_string(poisonTurns) + "] ";
		if (stunTurns > 0)   s += "[기절] ";
		return s;
	}
};



#endif