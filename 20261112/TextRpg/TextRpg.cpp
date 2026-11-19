
#include <iostream>
#include "SaveLoad.h"
#include "Player.h"
#include "Game.h"

using namespace std;

void ShowMainMenu(Player& player) {
	cout << "\n------------------------\n";
	cout << "  Text_Rpg에 어서오세요!\n";
	cout << "------------------------\n";
	cout << "\n\n\n\n\n";

	bool loaded = false;
	if (HasSaveData()) {
		cout << "\n------------------------\n";
		cout << "       Load Player!\n";
		cout << "------------------------\n";
		cout << "저장된 데이터가 있습니다.\n";
		cout << "1.이어하기\n";
		cout << "2.새로 시작하기\n";
		if (Select() == 1) {
			loaded = LoadData(player);
			if (loaded) {
				cout << ">> " << player.name << "님! 다시 오신 걸 환영합니다!\n";
			}
			else {
				cout << "<!> 저장 데이터를 불러오지 못했습니다.\n";
				cout << "새로운 모험을 시작합니다...\n";
			}
			system("pause");
		}
	}

	if (!loaded) {
		PlayerInit(player);
	}

	while (1) {
		system("cls");
		cout << "\n------------------------\n";
		cout << "       MAIN MENU\n";
		cout << "------------------------\n";
		cout << "\n\n\n\n";
		cout << "\n------------------------\n";
		cout << " 1.플레이어 정보 확인\n";
		cout << " 2.여행 시작\n";
		cout << " 3.저장하기\n";
		cout << " 4.종료\n";
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
			if (SaveData(player)) {
				cout << ">> 저장했습니다!\n";
			}
			else {
				cout << " (!) 저장에 실패했습니다.\n";
			}
			system("pause");
			break;
		case 4:
			SaveData(player);
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
	ShowMainMenu(player);
	return 0;
}