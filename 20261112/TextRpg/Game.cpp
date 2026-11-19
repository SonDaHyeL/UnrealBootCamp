#include <iostream>
#include <limits>
#include "Game.h"
#include "Battle.h"
using namespace std;

int Select() {
	int input = 0;
	cout << "\n어떻게 할까요?\n> ";
	if (!(cin >> input)) { //입력값 오류 방지(숫자 외 입력값 거름)
		cin.clear();                                          // 실패 상태 해제
		cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 남은 입력 버리기 (<limits> 필요)
		return -1;                                            // 각 메뉴의 default로 감
	}
	return input;
}

void StartGame(Player& player) {
	system("cls");
	cout << "\n------------------------\n";
	cout << ":: 여행을 시작합니다! ::\n";
	cout << "------------------------\n";
	Scene current = Scene::START;
	int input = 0;

	while (current != Scene::QUIT) {
		switch (current) {
		case Scene::START:

			cout << " 0.인벤토리 확인.\n";
			cout << " 1.마을로 간다.\n";
			cout << " 2.사냥터로 간다.\n";
			cout << " 3.모험을 종료한다.\n\n";
			input = Select();
			if (input == 0) { ShowInven(player); system("cls"); }
			else if (input == 1) current = Scene::TOWN;
			else if (input == 2) current = Scene::HUNTING;
			else if (input == 3) current = Scene::QUIT;
			else {
				cout << " (!) 잘못된 입력입니다.\n";
				system("pause");
			}
			break;
		case Scene::TOWN:
			current = Town(player);
			break;
		case Scene::STORE:
			current = Store(player);
			break;
		case Scene::HUNTING:
			current = Hunting(player);
			break;
		case Scene::QUIT:
			current = Scene::QUIT;
			break;
		default:
			current = Scene::START;
			break;
		}
	}
}

Scene Town(Player& player) {
	while (true) {
		system("cls");
		cout << "\n------------------------\n";
		cout << "     :: 마을 ::     \n";
		cout << "------------------------\n";
		cout << "한적하고 평화로운 곳입니다.\n\n";

		cout << " 0.인벤토리 확인.\n";
		cout << " 1.주변을 탐색한다.\n";
		cout << " 2.상점으로 간다.\n";
		cout << " 3.사냥터로 이동한다.\n";
		cout << " 4.모험을 종료한다.\n\n";
		int input = Select();

		switch (input) {
		case 0:
			ShowInven(player); system("cls");
			break;
		case 1:
			//주변 탐색
			system("cls");
			cout << "\n> 마을 촌장이 거리에서 서성이고 있습니다.\n문제라도 있는 걸까요?\n\n";
			cout << " 1.대화를 시도한다.\n";
			cout << " 2.무시한다.\n\n";
			input = Select();
			switch (input) {
			case 1:
				cout << "\n> 당신은 촌장에게 말을 걸었습니다...\n\n";
				cout << "'거기 모험가 양반, 혹시 괜찮다면 사냥터의 슬라임을 잡아다 주지 않겠나?'\n\n";
				cout << " 1.승낙한다.\n";
				cout << " 2.거절한다.\n\n";
				input = Select();

				if (input == 1) {
					system("cls");
					cout << "\n------------------------\n";
					cout << "    :: 퀘스트 승낙! ::    \n";
					cout << "------------------------\n";
					cout << "사냥터를 어지럽히는 슬라임을 잡자!(0/1)\n\n";
					player.quest = true;
					cout << "\n\n\n > 아자!\n";
					system("pause");
				}
				else {
					cout << "\n'에잇, 요즘 젊은이들은 통 야박해...'\n";
					system("pause");
				}
				break;
			case 2:
				cout << "\n> 당신은 촌장을 무시했습니다...\n\n";
				cout << "어쩐지 뒤통수가 따가운 기분이 듭니다.\n\n\n";
				system("pause");
				break;
			default:
				cout << "(!) 잘못된 입력입니다! \n\n";
				system("pause");
				break;
			}
			break;
		case 2:
			//상점
			return Scene::STORE;
		case 3:
			//사냥터
			return Scene::HUNTING;
		case 4:
			//모험 종료
			return Scene::QUIT;
		default:
			cout << " (!) 잘못된 입력입니다.\n";
			system("pause");
			break;
		}
	}
}

Scene Store(Player& player) {
	system("cls");
	cout << "\n------------------------\n";
	cout << "    :: 오늘은 영업 안함. ::    \n";
	cout << "------------------------\n";
	cout << "\n\n\n > 이런...\n";
	system("pause");
	return Scene::TOWN;

	//미 구현 내용
	//상점 구매
	//상점 판매
	// int idx = SelectInvenSlot(player);
	// if (idx != -1) SellItem(player, idx);
	//나가기
}

Scene Hunting(Player& player) {
	Enemy enemy;
	int input = 0;

	while (true) {
		system("cls");
		cout << "\n------------------------\n";
		cout << "     :: 사냥터 ::     \n";
		cout << "------------------------\n";
		cout << "사나운 몬스터 주의! 팻말이 붙어있습니다.\n\n";
		cout << " 0.인벤토리 확인.\n";
		cout << " 1.슬라임을 사냥한다.\n";
		cout << " 2.다람쥐를 사냥한다.\n";
		cout << " 3.마을로 돌아간다.\n";
		cout << " 4.모험을 종료한다.\n\n";
		input = Select();
		switch (input) {
		case 0:
			ShowInven(player); system("cls");
			break;
		case 1:
		case 2:
			system("cls");
			cout << "\n------------------------\n";
			cout << "      :: 전투! ::      \n";
			cout << "------------------------\n";
			enemy.Init(input);
			cout << enemy.eName << "이/가 나타났습니다!\n";
			Fight(player, enemy);

			if (player.hp <= 0) {
				cout << "\n>> GAME OVER... \n";
				cout << "\n\n\n > 이런...(겨우 체력을 회복하고 다시 일어나기.) \n";
				player.hp = 1;
				system("pause");
				return Scene::QUIT;
			}
			break;
		case 3:
			return Scene::TOWN;
		case 4:
			cout << ">> 모험을 종료합니다!\n";
			system("pause");
			return Scene::QUIT;
		default:
			cout << " (!) 잘못된 입력입니다.\n";
			system("pause");
			break;
		}
	}
}

