#include <iostream>
#include "Game.h"
#include "Battle.h"
#include "SaveLoad.h"

using namespace std;

void Game::Run() {
	UI::Init();
	LoadOrCreate();
	MainMenu();
}

void Game::LoadOrCreate() {
	UI::SetTitle("Text_Rpg");
	UI::ClearLog();
	UI::Log(":: Text_Rpg에 어서오세요! ::");

	if (HasSaveData()) {
		UI::Log("");
		UI::Log("저장된 데이터가 있습니다.");
		if (UI::Menu(player, { "이어하기", "새로 시작하기" }) == 1) {
			if (LoadData(player)) {
				UI::Log(">> " + player.GetName() + "님! 다시 오신 걸 환영합니다!");
				return;
			}
			UI::Log("<!> 저장 데이터를 불러오지 못했습니다. 새로운 모험을 시작합니다...");
		}
	}
	player.Create();
}



void Game::MainMenu() {
	UI::SetTitle("MAIN MENU");               // 로그는 비우지 않음 → 환영/생성 메시지가 남아 보임

	while (true) {
		switch (UI::Menu(player, { "상세 정보 보기", "여행 시작", "저장하기", "종료" })) {
		case 1: player.ShowInfo(); break;
		case 2:
			Adventure();
			UI::SetTitle("MAIN MENU");
			UI::ClearLog();
			UI::Log(">> 여행에서 돌아왔습니다.");
			break;
		case 3: Save(); break;
		case 4:
			Save();
			system("cls");
			cout << "\n >> 모험을 종료합니다! 다음에 또 만나요!\n\n";
			return;
		}
	}
}


void Game::Save() {
	if (SaveData(player)) UI::Log(">> 저장했습니다!");
	else                  UI::Log(" (!) 저장에 실패했습니다.");
}

void Game::InvenMenu() {
	UI::Log("");
	UI::Log(":: 인벤토리 관리 ::");

	while (true) {
		int idx = SelectInvenSlot(player);       // 취소하거나 빈 칸이면 -1
		if (idx == -1) return;

		UI::Log("[" + string(player.GetInven().Get(idx).GetName()) + "]을/를 선택했습니다.");
		switch (UI::Menu(player, { "사용한다", "버린다", "그만둔다" })) {
		case 1: UseItem(player, idx); break;
		case 2: DropItemAt(player, idx); break;
		case 3: break;
		}
	}
}


void Game::Adventure() {
	Scene current = Scene::START;

	while (current != Scene::QUIT) {
		switch (current) {
		case Scene::START:   current = Start();   break;
		case Scene::TOWN:    current = Town();    break;
		case Scene::STORE:   current = Store();   break;
		case Scene::HUNTING: current = Hunting(); break;
		default:             current = Scene::QUIT; break;
		}
	}
}

Game::Scene Game::Start() {
	UI::SetTitle("여행의 시작");
	UI::ClearLog();
	UI::Log(":: 여행을 시작합니다! ::");
	UI::Log("어디로 가볼까요?");

	while (true) {
		switch (UI::Menu(player, { "인벤토리 관리", "마을로 간다", "사냥터로 간다", "모험을 종료한다" }, 0)) {
		case 0: InvenMenu(); break;
		case 1: return Scene::TOWN;
		case 2: return Scene::HUNTING;
		case 3: return Scene::QUIT;
		}
	}
}

Game::Scene Game::Town() {
	
	UI::SetTitle("마 을");
	UI::ClearLog();
	UI::Log("한적하고 평화로운 곳입니다.");

	while (true) {

		switch (UI::Menu(player, { 
			"인벤토리 관리", 
			"주변을 탐색한다", 
			"상점으로 간다",
			"사냥터로 이동한다", 
			"모험을 종료한다" },
			0)) {
		case 0: InvenMenu(); break;
		case 1: TalkToChief(); break;
		case 2: return Scene::STORE;
		case 3: return Scene::HUNTING;
		case 4: return Scene::QUIT;
		}
	}
}


// 마을 - 주변 탐색: 촌장과 대화. 끝나면 Town()의 반복으로 돌아간다.
void Game::TalkToChief() {
	UI::Log("");
	UI::Log("> 마을 촌장이 거리에서 서성이고 있습니다. 문제라도 있는 걸까요?");
	if (UI::Menu(player, { "대화를 시도한다", "무시한다" }) == 1) {
		UI::Log("> 당신은 촌장에게 말을 걸었습니다...");
		UI::Log("'거기 모험가 양반, 혹시 괜찮다면 사냥터의 슬라임을 잡아다 주지 않겠나?'");
		if (UI::Menu(player, { "승낙한다", "거절한다" }) == 1) {
			player.AcceptQuest();
			UI::Log("★ 퀘스트 승낙! 사냥터를 어지럽히는 슬라임을 잡자! (0/1)");
		}
		else {
			UI::Log("'에잇, 요즘 젊은이들은 통 야박해...'");
		}
	}
	else {
		UI::Log("> 당신은 촌장을 무시했습니다... 어쩐지 뒤통수가 따가운 기분이 듭니다.");
	}
}

Game::Scene Game::Store() {
	UI::SetTitle("상 점");
	UI::ClearLog();
	UI::Log("오래된 목재로 지어진 작은 상점입니다.");
	UI::Log("진열대에는 다양한 물건들이 가지런히 놓여 있습니다.");
	UI::Log("");
	UI::Log("'어서 오세요! 모험가님! 필요한 물건이라도 있으신가요?'");

	while (true) {
		switch (UI::Menu(player, { "아이템을 구매한다", "아이템을 판매한다", "상점을 나간다" })) {
		case 1: BuyMenu(); break;
		case 2: SellMenu(); break;
		case 3: return Scene::TOWN;
		}
	}
}

void Game::BuyMenu() {
	const ITEMLIST goods[] = { heal_potion, wooden_sword, leaf_crown, flower_ring };

	UI::Log("");
	UI::Log("'늘 좋은 물건만 들여오고 있답니다!'");
	UI::Log(" - 회복 포션 : 별로 맛은 없다. (HP +30)");
	UI::Log(" - 나무 검 : 어설프지만 튼튼하다! (ATK +3)");
	UI::Log(" - 나뭇잎 왕관 : 나는 숲속의 왕이다. (DEF +2 & MAX HP +10)");
	UI::Log(" - 꽃 반지 : 정성스레 엮었다. (DEF +1 & MAX HP +30)");

	// 선택지: 0번은 나가기, 1~4번은 상품 (이름과 가격은 ITEM_TABLE에서)
	vector<string> options = { "아무것도 사지 않는다" };
	for (ITEMLIST id : goods)
		options.push_back(string(ITEM_TABLE[id].name) + " (" + to_string(ITEM_TABLE[id].buyPrice) + "G)");

	while (true) {
		int input = UI::Menu(player, options, 0);
		if (input == 0) {
			UI::Log("'에이, 조금만 더 보고 가시지!'");
			return;
		}

		switch (BuyItem(player, goods[input - 1])) {
		case 1:  UI::Log("'좋은 선택이에요! 다음에도 꼭 들러주세요!'"); break;
		case 0:  UI::Log("'어라? 가방이 이미 꽉 찬 것 같은데요?'"); break;
		case -1: UI::Log("'어라? 돈이 부족하신 것 같은데요?'"); break;
		}
	}
}

void Game::SellMenu() {
	UI::Log("");
	UI::Log("'오늘은 어떤 재밌는 걸 가져오셨나요?'");

	while (true) {
		// 팔 때마다 인벤토리가 바뀌니까 선택지를 매번 새로 만든다
		vector<string> options = { "아무것도 팔지 않는다" };
		for (int i = 0; i < INVEN_SIZE; ++i) {
			const ITEM& item = player.GetInven().Get(i);
			options.push_back(item.IsEmpty() ? "-"
				: string(item.GetName()) + " (" + to_string(item.GetSellPrice()) + "G)");
		}

		int input = UI::Menu(player, options, 0);
		if (input == 0) {
			UI::Log("'에이, 좋게 잘 봐드릴 수 있는데요!'");
			return;
		}
		SellItem(player, input);   // 1~8번 = 슬롯 번호 (SellItem 안에서 -1 함)
	}
}


Game::Scene Game::Hunting() {
	UI::SetTitle("사 냥 터");
	UI::ClearLog();
	UI::Log("사나운 몬스터 주의! 팻말이 붙어있습니다.");

	Enemy enemy;
	while (true) {
		int input = UI::Menu(player, { "인벤토리 관리", "슬라임을 사냥한다", "다람쥐를 사냥한다",
									   "마을로 돌아간다", "모험을 종료한다" }, 0);
		switch (input) {
		case 0: InvenMenu(); break;
		case 1:
		case 2:
			enemy.Init(input);
			Fight(player, enemy);
			UI::SetTitle("사 냥 터");          // 전투 로그는 남겨두고 제목만 되돌림

			if (player.IsDead()) {
				UI::Log("");
				UI::Log(">> GAME OVER...");
				UI::Log("> 이런...(겨우 체력을 회복하고 다시 일어나기.)");
				UI::Menu(player, { "메인 메뉴로 돌아간다" });   // pause 대신: 확인용 선택지 하나
				player.Revive();
				return Scene::QUIT;
			}
			break;
		case 3: return Scene::TOWN;
		case 4: return Scene::QUIT;
		}
	}
}
