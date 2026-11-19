#include "Player.h"
#include "Game.h"
#include <iostream>

using namespace std;

void Player::Init() {
	switch (job) {
	case 1: //전사
		jobName = "전사";
		attack = 5;
		defense = 3;
		maxHp = 150;
		hp = 150;
		break;
	case 2: //궁수
		jobName = "궁수";
		attack = 3;
		defense = 2;
		maxHp = 100;
		hp = 100;
		break;
	case 3: //도적
		jobName = "도적";
		attack = 3;
		defense = 1;
		maxHp = 120;
		hp = 120;
		break;
	case 4: //마법사
		jobName = "마법사";
		attack = 4;
		defense = 2;
		maxHp = 100;
		hp = 100;
		break;
	}
}
void Player::ShowInfo() {
	system("cls");
	cout << "\n------------------------\n";
	cout << " > 플레이어 이름 : " << name << endl;
	cout << " > 직업 : " << jobName << endl;
	cout << " > HP (최대/현재) : " << maxHp << " / " << hp << endl;
	cout << " > 공격력 : " << attack << endl;
	cout << " > 방어력 : " << defense << endl;
	cout << " :: 소지금 : " << gold << "G" << endl;

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


