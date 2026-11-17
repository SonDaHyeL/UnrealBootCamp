#include <iostream>
#include "Game.h"
#include "Battle.h"
using namespace std;


void Fight(Player& player, Enemy& enemy) {
	int input = 0;
	while (player.hp > 0 && enemy.hp > 0) {
		system("cls");
		cout << player.name << "의 HP :" << player.hp << endl;
		cout << enemy.eName << "의 HP :" << enemy.hp << endl;
		cout << " 1.공격한다.\n";
		cout << " 2.방어한다.\n";
		cout << " 3.도망간다.\n\n\n";

		input = Select();
		switch (input) {
		case 1:
			cout << player.name << "의 공격! " << enemy.eName << "에게 " << (player.attack * 3) - enemy.defense << "의 데미지를 주었습니다." << endl;
			enemy.hp -= ((player.attack * 3) - enemy.defense);
			cout << enemy.eName << "의 공격! " << player.name << "에게 " << enemy.attack * 3 << "의 데미지를 주었습니다." << endl;
			player.hp -= (enemy.attack * 3);
			system("pause");
			break;
		case 2:
			cout << enemy.eName << "의 공격! " << endl;
			cout << "하지만 방어하여 " << (enemy.attack * 3) - (player.defense * 2) << "의 데미지만 입었습니다." << endl;
			if (((enemy.attack * 3) - (player.defense * 2)) > 0) {
				player.hp -= (enemy.attack * 3) - (player.defense * 2);
			}
			system("pause");
			break;
		case 3:
			cout << "\n> 당신은 도망쳤습니다...\n";
			cout << "> 이런...\n ";
			system("pause");
			return;
		}
	}

	if (player.hp <= 0) {
		cout << "\n>> 당신은 쓰러졌습니다...\n";
		cout << "> 이런...\n";
		system("pause");
		return;
	}

	system("cls");
	cout << "\n------------------------\n";
	cout << "      :: 전투 종료! ::      \n";
	cout << "------------------------\n";

	if (enemy.eName == "슬라임" && enemy.hp <= 0 && player.quest == true) {
		cout << "\n------------------------\n";
		cout << "    :: 퀘스트 성공! ::    \n";
		cout << "------------------------\n";
		cout << "사냥터를 어지럽히는 슬라임을 잡자!(1/1)\n\n";
		cout << "\n\n >> 퀘스트 완료 보상으로 10골드를 얻었습니다!\n";
		player.quest = false;
	}
	printf("> 야호!\n");
	system("pause");
	return;
}