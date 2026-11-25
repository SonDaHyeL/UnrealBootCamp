#include <iostream>
#include "Game.h"
#include "Battle.h"
#include "SaveLoad.h"
#include "Input.h"
using namespace std;

void Game::Run() {
	LoadOrCreate();
	MainMenu();
}

void Game::LoadOrCreate() {
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
				cout << ">> " << player.GetName() << "님! 다시 오신 걸 환영합니다!\n";
			}
			else {
				cout << "<!> 저장 데이터를 불러오지 못했습니다.\n";
				cout << "새로운 모험을 시작합니다...\n";
			}
			system("pause");
		}
	}

	if (!loaded) {
		player.Create();
	}
}


void Game::MainMenu() {
	while (1) {
		system("cls");
		cout << "\n------------------------\n";
		cout << "       MAIN MENU\n";
		cout << "------------------------\n\n\n\n\n";
		cout << "\n------------------------\n";
		cout << " 1.플레이어 정보 확인\n 2.여행 시작\n 3.저장하기\n 4.종료\n";

		int input = Select();
		switch (input)
		{
		case 1:
			player.ShowInfo();
			printf(">> 여행을 시작해 봅시다! \n> 좋아요!\n");
			system("pause");
			break;
		case 2:
			Adventure();
			break;
		case 3:
			Save();
			system("pause");
			break;
		case 4:
			Save();
			cout << ">> 모험을 종료합니다!\n";
			return;
		default:
			cout << " (!) 잘못된 입력입니다!\n";
			system("pause");
			break;
		}
	}
}


void Game::Save() {
	if (SaveData(player)) {
		cout << ">> 저장했습니다!\n";
	}
	else {
		cout << " (!) 저장에 실패했습니다.\n";
	}
}


void Game::Adventure() {
	system("cls");
	cout << "\n------------------------\n";
	cout << ":: 여행을 시작합니다! ::\n";
	cout << "------------------------\n";
	Scene current = Scene::START;

	while (current != Scene::QUIT) {
		switch (current) {
		case Scene::START:
			current = Start(); break;
		case Scene::TOWN:
			current = Town(); break;
		case Scene::STORE:
			current = Store(); break;
		case Scene::HUNTING:
			current = Hunting(); break;
		default:
			current = Scene::QUIT; break;
		}
	}
}

Game::Scene Game::Start() {
	while (true) {
		cout << " 0.인벤토리 확인.\n";
		cout << " 1.마을로 간다.\n";
		cout << " 2.사냥터로 간다.\n";
		cout << " 3.모험을 종료한다.\n\n";
		switch (Select())
		{
		case 0: ShowInven(player); system("pause"); break;
		case 1: return Scene::TOWN;
		case 2: return Scene::HUNTING;
		case 3: return Scene::QUIT;
		default:
			cout << " (!) 잘못된 입력입니다.\n";
			system("pause");
			break;
		}
	}
}

Game::Scene Game::Town() {
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
		switch (Select()) {
		case 0:
			ShowInven(player); system("pause");
			break;
		case 1: TalkToChief(); break;
		case 2: return Scene::STORE;
		case 3: return Scene::HUNTING;
		case 4: return Scene::QUIT;
		default:
			cout << " (!) 잘못된 입력입니다.\n";
			system("pause");
			break;
		}
	}
}


// 마을 - 주변 탐색: 촌장과 대화. 끝나면 Town()의 반복으로 돌아간다.
void Game::TalkToChief() {
	system("cls");
	cout << "\n> 마을 촌장이 거리에서 서성이고 있습니다.\n문제라도 있는 걸까요?\n\n";
	cout << " 1.대화를 시도한다.\n";
	cout << " 2.무시한다.\n\n";

	switch (Select()) {
	case 1:
		cout << "\n> 당신은 촌장에게 말을 걸었습니다...\n\n";
		cout << "'거기 모험가 양반, 혹시 괜찮다면 사냥터의 슬라임을 잡아다 주지 않겠나?'\n\n";
		cout << " 1.승낙한다.\n";
		cout << " 2.거절한다.\n\n";
		if (Select() == 1) {
			system("cls");
			cout << "\n------------------------\n";
			cout << "    :: 퀘스트 승낙! ::    \n";
			cout << "------------------------\n";
			cout << "사냥터를 어지럽히는 슬라임을 잡자!(0/1)\n\n";
			player.AcceptQuest();
			cout << "\n\n\n > 아자!\n";
		}
		else {
			cout << "\n'에잇, 요즘 젊은이들은 통 야박해...'\n";
		}
		system("pause");
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
}

Game::Scene Game::Store() {
	while (true) {
		system("cls");
		cout << "\n------------------------\n";
		cout << "       :: 상점 ::    \n";
		cout << "------------------------\n";
		cout << "오래된 목재로 지어진 작은 상점입니다.\n진열대에는 다양한 물건들이 가지런히 놓여 있습니다.\n\n";

		cout << "'어서 오세요! 모험가님! 필요한 물건이라도 있으신가요?'\n\n";
		cout << "------------------------\n";
		cout << "1.아이템을 구매한다.\n";
		cout << "2.아이템을 판매한다.\n";
		cout << "3.상점을 나간다.\n";
		switch (Select())
		{
		case 1: BuyMenu(); break;			
		case 2: SellMenu(); break;
		case 3:
			system("cls");
			cout << "'안녕히가세요! 또 오시면 좋고요!'\n\n";
			cout << ">> 당신은 거리로 돌아갑니다.\n";
			system("pause");
			return Scene::TOWN;
		default:
			cout << " (!) 잘못된 입력입니다.\n";
			system("pause");
			break;
		}
	}
}

void Game::BuyMenu() {
	bool shopping = true;
	while (shopping) {
		system("cls");
		cout << "\n'늘 좋은 물건만 들여오고 있답니다!'\n";
		cout << "---------------------\n";
		cout << "      판매 목록\n";
		cout << "---------------------\n";
		cout << " :: 소지금 : " << player.GetGold() << "G\n";
		cout << "1.체력 포션(5G) : 별로 맛은 없다. ( HP +30 )\n";
		cout << "2.나무 검(10G) : 어설프지만 튼튼하다! ( ATK +3 )\n";
		cout << "3.나뭇잎 왕관(8G) : 나는 숲속의 왕이다. ( DEF +2 & MAX HP +10 )\n";
		cout << "4.꽃 반지(6G) : 정성스레 엮었다. ( DEF +1 & MAX HP +30 )\n";
		cout << "\n0.아무것도 사지 않는다.\n\n";
		ITEMLIST buyID = noItem;
		switch (Select())
		{
		case 1: buyID = heal_potion; break;
		case 2: buyID = wooden_sword; break;
		case 3: buyID = leaf_crown; break;
		case 4: buyID = flower_ring; break;
		case 0:
			system("cls");
			cout << "\n'에이, 조금만 더 보고 가시지!'\n\n";
			system("pause");
			shopping = false;
			continue;
		default:
			cout << " (!) 잘못된 입력입니다.\n";
			system("pause");
			continue;
		}

		switch (BuyItem(player, buyID)) {
		case 1:
			cout << "'좋은 선택이에요! 다음에도 꼭 들러주세요!'\n";
			break;
		case 0:
			cout << "'어라? 가방이 이미 꽉 찬 것 같은데요?'\n";
			break;
		case -1:
			cout << "'어라? 돈이 부족하신 것 같은데요?'\n";
			break;
		}
		system("pause");
	}
}

void Game::SellMenu() {
	bool shopping = true;
	while (shopping) {
		system("cls");
		cout << "\n'오늘은 어떤 재밌는 걸 가져오셨나요?'\n";
		cout << "---------------------\n";
		cout << " :: 소지금 : " << player.GetGold() << "G\n";
		ShowInven(player);

		cout << "\n0.아무것도 팔지 않는다.\n\n";
		cout << ">> 팔고 싶은 아이템을 선택해주세요!\n";

		int input = Select();

		if (input == 0) {
			system("cls");
			cout << "\n'에이, 좋게 잘 봐드릴 수 있는데요!'\n";
			shopping = false;
		}
		else {
			SellItem(player, input);
		}
		system("pause");
	}
}


Game::Scene Game::Hunting() {
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
			ShowInven(player); system("pause");
			break;
		case 1:
		case 2:
			system("cls");
			cout << "\n------------------------\n";
			cout << "      :: 전투! ::      \n";
			cout << "------------------------\n";
			enemy.Init(input);
			cout << enemy.GetName() << "이/가 나타났습니다!\n";
			Fight(player, enemy);

			if (player.IsDead()) {
				cout << "\n>> GAME OVER... \n";
				cout << "\n\n\n > 이런...(겨우 체력을 회복하고 다시 일어나기.) \n";
				system("pause");
				player.Revive();
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
