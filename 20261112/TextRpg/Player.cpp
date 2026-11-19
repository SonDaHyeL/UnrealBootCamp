#include "Player.h"
#include "Game.h"
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
void Player::ShowInfo() {
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

void PlayerInit(Player& player) {
	cout << ">> 당신의 이름을 입력하세요.\n";
	cout << "> ";
	cin >> player.name;

	cout << "\n>> 당신의 직업을 선택하세요.\n";
	cout << " 1.전사\n";
	cout << " 2.궁수\n";
	cout << " 3.도적\n";
	cout << " 4.마법사\n";

	while (1) {
		player.job = Select();
		switch (player.job)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			player.Init();
			return;
		default:
			cout << " <!> 잘못된 입력입니다! \n";
			break;
		}
	}
	
}

void GainExp(Player& player, int amount) {
	player.exp += amount;
	cout << " >> 경험치 " << amount << " 획득! (" << player.exp << "/" << player.GetNeedExp() << ")\n";
	
	while (player.exp >= player.GetNeedExp()) {
		player.exp -= player.GetNeedExp();

		if (player.level + 1 > MAX_LEVEL) {
			cout << ">> 이미 최대 레벨입니다!" << endl;
			cout << ":: 레벨 초과 보너스 보상 : 20G :: " << endl;
			player.AddGold(20);
			continue;
		}

		player.level++;
		player.hp = player.GetMaxHp();
		cout << "=======★ Level UP!!! ★======= " << endl;
		cout << ">> LV " << player.level - 1 << " -> LV " << player.level << endl;
	}
	
	return;
}
