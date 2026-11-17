
#include <iostream>
#include "Player.h"
#include "Game.h"

using namespace std;

void ShowMainMenu(Player& player) {
	cout << "\n------------------------\n";
	cout << "  Text_Rpg에 어서오세요!\n";
	cout << "------------------------\n";
	cout << "\n\n\n\n\n";

	PlayerInit(player);

	while (1) {
		system("cls");
		cout << "\n------------------------\n";
		cout << "       MAIN MENU\n";
		cout << "------------------------\n";
		cout << "\n\n\n\n";
		cout << "\n------------------------\n";
		cout << ">> 어떤 행동을 할까요?\n";
		cout << " 1.플레이어 정보 확인\n";
		cout << " 2.여행 시작\n";
		cout << " 3.종료\n";
		int input = Select();
		switch (input)
		{
		case 1:
			player.ShowInfo();
			printf(">> 여행을 시작해 봅시다! \n");
			printf("> 좋아요!\n");
			system("pause");
			break;
		case 2:
			StartGame(player);
			break;
		case 3:
			cout << ">> 모험을 종료합니다!\n";
			return;
		default:
			cout << " (!) 잘못된 입력입니다!\n";
			system("pause");
			break;
		}
	}
}

int main() {
	Player player;
	Scene current = Scene::MENU;
	ShowMainMenu(player);
	return 0;
}