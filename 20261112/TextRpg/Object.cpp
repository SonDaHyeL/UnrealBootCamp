#include "UI.h"
#include "Player.h"
#include <iostream>
using namespace std;


// ::인벤토리:: 기본 조작
int SelectInvenSlot(Player& player) {
	UI::Log(">> 인벤토리 슬롯을 선택하세요.");
	vector<string> options = { "취소" };
	for (int i = 0; i < INVEN_SIZE; ++i) {
		const ITEM& item = player.GetInven().Get(i);
		options.push_back(item.IsEmpty() ? "-" : item.GetName());
	}

	int input = UI::Menu(player, options, 0);
	if (input == 0) {
		UI::Log(" >> 아무것도 선택되지 않았습니다.");
		return -1;
	}
	int idx = input - 1;
	if (player.GetInven().IsEmptyAt(idx)) {
		UI::Log(" >> 빈 칸입니다.");
		return -1;
	}
	return idx;
}

// ::인벤토리:: 아이템 획득 / 버림
void GetItem(Player& player, ITEMLIST ItemID) {
	if (ItemID <= noItem || ItemID >= ITEM_COUNT) return;
	string name = ITEM_TABLE[ItemID].name;

	while (true) {
		if (player.GetInven().Add(ItemID)) {
			UI::Log("--[" + name + "]을/를 획득했습니다!--");
			return;
		}
		UI::Log(" >> 인벤토리가 가득 찼습니다! (" + to_string(INVEN_SIZE) + "/" + to_string(INVEN_SIZE) + ")");
		if (UI::Menu(player, { "기존 아이템을 버린다", "[" + name + "]을/를 포기한다" }) == 2) {
			UI::Log("--[" + name + "]을/를 포기했습니다...--");
			return;
		}
		DropItem(player);
	}
}

void DropItemAt(Player& player, int idx) {
	UI::Log("[" + string(player.GetInven().Get(idx).GetName()) + "]을/를 버렸습니다.");
	player.GetInven().Remove(idx);
}

int DropItem(Player& player) {
	int idx = SelectInvenSlot(player);
	if (idx == -1) { return 0; }
	DropItemAt(player, idx);
	return 1;
}

// ::인벤토리:: 아이템 사용
bool UseItem(Player& player, int idx) {
	if (player.GetInven().IsEmptyAt(idx)) return false; //범위&빈칸 검사 통합
	ITEM item = player.GetInven().Get(idx);

	if (!item.IsUseble()) { //사용할 수 있는 아이템이 아니라면 사용 못함
		UI::Log(">> [" + string(item.GetName()) + "]은/는 사용할 수 없는 아이템입니다.");
		return false;
	}
	
	if (player.GetHp() >= player.GetMaxHp()) { //체력이 이미 가득 차 있다면 사용하지 않음
		UI::Log(">> 이미 HP가 가득 차 있습니다!");
		return false;
	}

	UI::Log(">> [" + string(item.GetName()) + "]은/는 사용할 수 없는 아이템입니다.");
	player.Heal(item.GetHeal());
	player.GetInven().Remove(idx);

	return true;
}

// ::상점:: 아이템 구매 / 판매
int BuyItem(Player& player, ITEMLIST ItemID) { // -1 골드 부족 | 0 인벤토리 가득 참 | 1 구매 성공
	ITEM newItem;
	newItem.ItemID = ItemID;

	int price = newItem.GetBuyPrice();

	if (player.GetGold() < price) { //골드 부족으로 구매 실패
		UI::Log("<< 보유한 골드가 부족합니다!");
		return -1; 
	}

	if (!player.GetInven().Add(ItemID)) {
		UI::Log(" >> 인벤토리가 가득 찼습니다! (" + to_string(INVEN_SIZE) + "/" + to_string(INVEN_SIZE) + ")");
		return 0;
	}
	UI::Log("--[" + string(newItem.GetName()) + "]을/를 구매했습니다!--");
	player.SpendGold(price);
	return 1;
}

int SellItem(Player& player, int idx) {
	idx -= 1;
	if (player.GetInven().IsEmptyAt(idx)) {
		UI::Log(" >> 해당 칸은 판매할 수 없습니다!");
		return -1;
	}

	const ITEM& item = player.GetInven().Get(idx);
	int price = item.GetSellPrice();
	UI::Log(string(item.GetName()) + "을/를 판매해 " + to_string(price) + "골드를 얻었습니다!");
	player.AddGold(price);
	player.GetInven().Remove(idx);
	return 1;
}


