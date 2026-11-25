#include "Player.h"
#include "Input.h"
#include <iostream>

using namespace std;

void Player::Init() {
	switch (job) {
	case 1: //전사
		jobName = "전사";
		attack = 3;		defense = 3;	basehp = 150;
		growAtk = 1;	growDef = 2;	growHP = 15;
		break;
	case 2: //궁수
		jobName = "궁수";
		attack = 3;		defense = 2;	basehp = 100;
		growAtk = 2;	growDef = 2;	growHP = 10;
		break;
	case 3: //도적
		jobName = "도적";
		attack = 3;		defense = 1;	basehp = 120;
		growAtk = 3;	growDef = 1;	growHP = 10;
		break;
	case 4: //마법사
		jobName = "마법사";
		attack = 4;		defense = 2;	basehp = 100;
		growAtk = 3;	growDef = 1;	growHP = 10;
		break;
	}
	level = 1;
	exp = 0;
	hp = GetMaxHp();
}

void Player::ShowInfo() const {
	system("cls");
	cout << "\n------------------------\n";

	cout << " > 플레이어 이름(LV." << level << ") : " << name << endl;
	cout << " > 직업 : " << jobName << endl;
	cout << " > HP (최대/현재) : " << GetMaxHp() << " / " << hp << endl;
	cout << " > 공격력 : " << GetAttack() << endl;
	cout << " > 방어력 : " << GetDefense() << endl;
	cout << " :: 소지금 : " << gold << "G ::" << endl;

	cout << endl << " >> 다음 LV까지 " << GetNeedExp() - exp << "필요!" << endl;
}

void Player::Create() {
	cout << ">> 당신의 이름을 입력하세요.\n>";
	cin >> name;

	cout << "\n>> 당신의 직업을 선택하세요.\n";
	cout << " 1.전사\n 2.궁수\n 3.도적\n 4.마법사\n";
	while (1) {
		job = Select();
		if (job >= 1 && job <= 4) { Init(); return; }
		cout << " <!> 잘못된 입력입니다! \n";
	}
}

void Player::AddExp(int amount) {
	exp += amount;
	cout << " >> " << amount << " 경험치 획득! (" << exp << "/" << GetNeedExp() << ")\n";

	while (exp >= GetNeedExp()) {
		exp -= GetNeedExp();

		if (level + 1 > MAX_LEVEL) {
			cout << ">> 이미 최대 레벨입니다!" << endl;
			cout << ":: 레벨 초과 보너스 보상 : 20G :: " << endl;
			AddGold(20);
			continue;
		}

		level++;
		hp = GetMaxHp();
		cout << "=======★ Level UP!!! ★======= " << endl;
		cout << ">> LV " << level - 1 << " -> LV " << level << endl;
	}
}

bool Player::SpendGold(int amount)
{
	if (gold < amount) return false;
	gold -= amount;
	return true;
}