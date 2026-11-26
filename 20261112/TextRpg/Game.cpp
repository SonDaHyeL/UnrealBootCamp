#include <iostream>
#include "Game.h"
#include "Battle.h"
#include "SaveLoad.h"
#include "DataTable.h"

using namespace std;

void Game::Run() {
	string error;
	if (!LoadGameData(error)) {
		cout << "\n (!) 게임 데이터를 불러오지 못했습니다.\n " << error << "\n\n";
		system("pause");
		return;
	}
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
	bool equipPage = false;

	while (true) {
		UI::SetEquipPage(equipPage);

		// 선택지: 1~count 각 칸 | count+1 페이지 넘기기 | count+2 닫기
		int count = equipPage ? EQUIPSLOT_END : INVEN_SIZE;
		vector<string> option;

		for (int i = 0; i < count; ++i) {
			const ITEM& item = equipPage ? player.GetEquip(i) : player.GetInven().Get(i);
			string itemName = item.IsEmpty() ? "-" : item.GetName();
			option.push_back(equipPage ? string(EQUIPSLOT_NAME[i]) + " : " + itemName : itemName);
		}
		option.push_back(equipPage ? "인벤토리 보기" : "장비창 보기");
		option.push_back("닫기");

		int input = UI::Menu(player, option);
		if (input == count + 2) break;										  //닫기
		if (input == count + 1) { equipPage = !equipPage; continue; } //페이지 넘기기

		int idx = input - 1;
		const ITEM& picked = equipPage ? player.GetEquip(idx) : player.GetInven().Get(idx);

		if (picked.IsEmpty()) {
			UI::Log(">> 빈 칸입니다.");
			continue;
		}

		if (equipPage) EquipMenu(idx);
		else ItemMenu(idx);
	}
	UI::SetEquipPage(false);
}


void Game::ItemMenu(int idx) {
	ITEM item = player.GetInven().Get(idx);
	UI::Log("[" + string(item.GetName()) + "] " + GetItemInfo(item));

	// 아이템 종류에 따라 선택지를 만든다. 글자(labels)와 행동(actions)을 같은 순서로 담는다.
	enum class Action { USE, EQUIP, DROP, CANCLE };

	vector<string> labels;
	vector<Action> actions;

	if (item.IsUseble()) {
		labels.push_back("사용한다");
		actions.push_back(Action::USE);
	}

	if (item.IsEquipment()) {
		labels.push_back("장착한다");
		actions.push_back(Action::EQUIP);
	}

	labels.push_back("버린다");
	actions.push_back(Action::DROP);
	labels.push_back("그만둔다");
	actions.push_back(Action::CANCLE);

	switch (actions[UI::Menu(player, labels) - 1]) {
	case Action::USE:		UseItem(player, idx);		break;
	case Action::EQUIP:		player.Equip(idx);			break;
	case Action::DROP:
		if (UI::Menu(player, { "정말 버린다", "그만둔다" }) == 1)
		DropItemAt(player, idx);
		break;
	case Action::CANCLE:	break;
	}
}

void Game::EquipMenu(int slot) {
	ITEM item = player.GetEquip(slot);
	UI::Log("[" + string(EQUIPSLOT_NAME[slot]) + "] " + item.GetName() + "  " + GetItemInfo(item));
	if (UI::Menu(player, { "해제한다", "그만둔다" }) == 1) 
	{	player.Unequip(slot);	}
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
		switch (UI::Menu(player, { "인벤토리 관리", "마을로 간다", "사냥터로 간다", "모험을 종료한다" })) {
		case 1: InvenMenu(); break;
		case 2: return Scene::TOWN;
		case 3: return Scene::HUNTING;
		case 4: return Scene::QUIT;
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
			"모험을 종료한다" })) {
		case 1: InvenMenu(); break;
		case 2: TalkToChief(); break;
		case 3: return Scene::STORE;
		case 4: return Scene::HUNTING;
		case 5: return Scene::QUIT;
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
		switch (UI::Menu(player, { "인벤토리 관리", "아이템을 구매한다", "아이템을 판매한다", "상점을 나간다" })) {
		case 1: InvenMenu(); break;
		case 2: BuyMenu(); break;
		case 3: SellMenu(); break;
		case 4: return Scene::TOWN;
		}
	}
}

void Game::BuyMenu() {
	vector<ITEMLIST> goods = GetShopItems();

	UI::Log("");
	UI::Log("'늘 좋은 물건만 들여오고 있답니다!'");
	UI::Log("");
	for (ITEMLIST id : goods)
		UI::Log(" - " + GetItemData(id).name + " : " + GetItemData(id).description);
	UI::Log("");
	// 선택지: 상품들, 마지막 번호는 나가기
	vector<string> options;
	for (ITEMLIST id : goods)
		options.push_back(GetItemData(id).name + " (" + to_string(GetItemData(id).buyPrice) + "G)");
	options.push_back("아무것도 사지 않는다");

	while (true) {
		int input = UI::Menu(player, options);
		if (input == (int)options.size()) {
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
		// 1~8번 = 인벤토리 칸 번호 그대로, 마지막 번호는 나가기
		vector<string> options;
		for (int i = 0; i < INVEN_SIZE; ++i) {
			const ITEM& item = player.GetInven().Get(i);
			options.push_back(item.IsEmpty() ? "-"
				: string(item.GetName()) + " (" + to_string(item.GetSellPrice()) + "G)");
		}
		options.push_back("아무것도 팔지 않는다");

		int input = UI::Menu(player, options);
		if (input == (int)options.size()) {
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

	// 선택지: 1 인벤토리 | 2~ 몬스터들 (표에서 자동) | 마을 | 종료
	vector<int> monsters = GetEnemyIds();
	vector<string> options = { "인벤토리 관리" };

	for (int id : monsters)	options.push_back(GetEnemyData(id).name + "을/를 사냥한다");
	options.push_back("마을로 돌아간다");
	options.push_back("모험을 종료한다");

	const int TOWN_NO = (int)options.size() - 1;
	const int QUIT_NO = (int)options.size();

	while (true) {
		int input = UI::Menu(player, options);
		if (input == 1) { InvenMenu(); continue; }
		if (input == TOWN_NO) return Scene::TOWN;
		if (input == QUIT_NO) return Scene::QUIT;

		Enemy enemy(monsters[input - 2]);   // 2번부터 몬스터
		Fight(player, enemy);
		UI::SetTitle("사 냥 터");

		if (player.IsDead()) {
			UI::Log("");
			UI::Log(">> GAME OVER...");
			UI::Log("> 이런...(겨우 체력을 회복하고 다시 일어나기.)");
			UI::Menu(player, { "메인 메뉴로 돌아간다" });
			player.Revive();
			return Scene::QUIT;
		}
	}
}
