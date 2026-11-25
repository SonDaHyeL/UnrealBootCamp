#include "Input.h"
#include "Player.h"
#include <iostream>
using namespace std;


// ::인벤토리:: 기본 조작
int SelectInvenSlot(Player& player) { //인벤토리 슬롯 내 선택
	ShowInven(player);
	cout << ">> 인벤토리 슬롯을 선택하세요. (0: 취소)\n";

	int input = Select();

	if (input < 1 || input > INVEN_SIZE) {
		cout << " >> 아무것도 선택되지 않았습니다." << endl;
		return -1;
	}
	int idx = input - 1;
	if (player.GetInven().IsEmptyAt(idx)) {
		cout << " >> 빈 칸입니다." << endl;
		return -1;
	}
	return idx;
}
void ShowInven(Player& player) { //인벤토리 출력하기 
	system("cls");
	cout << " ====== INVENTORY ======" << endl;
	player.GetInven().Show();
	cout << " :: 소지금 : " << player.GetGold() << "G" << endl;
	cout << " ====== ========= ======" << endl;
}

// ::인벤토리:: 아이템 획득 / 버림
void GetItem(Player& player, ITEMLIST ItemID) {
	if (ItemID <= noItem || ItemID >= ITEM_COUNT) return;
	ITEM newItem;
	newItem.ItemID = ItemID;
		
	while (1) {
		if (player.GetInven().Add(ItemID)) {
			cout << "--[" << newItem.GetName() << "]을/를 획득했습니다!--\n";
			return;
		}
		cout << " >> 인벤토리가 가득 찼습니다! (" << INVEN_SIZE << "/" << INVEN_SIZE << ")\n";
		cout << "1. 기존 아이템을 버린다.\n";
		cout << "2. [" << newItem.GetName() << "]을/를 포기한다.\n";
		int input = Select();
		if (input == 1) {
			DropItem(player);
		}
		else if(input == 2) {
			cout << "--[" << newItem.GetName() << "]을/를 포기했습니다...--\n";
			return;
		}
		else {
			cout << " <!> 잘못된 입력입니다. \n";
		}
	}
}

void DropItemAt(Player& player, int idx) {
	cout << "[" << player.GetInven().Get(idx).GetName() << "]을/를 버렸습니다. \n";
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
		cout << ">> [" << item.GetName() << "]은/는 사용할 수 없는 아이템입니다." << endl;
		return false;
	}
	
	if (player.GetHp() >= player.GetMaxHp()) { //체력이 이미 가득 차 있다면 사용하지 않음
		cout << ">> 이미 HP가 가득 차 있습니다!" << endl;
		return false;
	}

	cout << ">> [" << item.GetName() << "] 사용! HP를 " << item.GetHeal() << "만큼 회복합니다.\n";
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
		cout << "<< 보유한 골드가 부족합니다!\n";
		return -1; 
	}

	if (!player.GetInven().Add(ItemID)) {
		cout << " >> 인벤토리가 가득 찼습니다! (" << INVEN_SIZE << "/" << INVEN_SIZE << ")\n";
		return 0;
	}
	cout << "--[" << newItem.GetName() << "]을/를 구매했습니다!--\n";
	player.SpendGold(price);
	return 1;
}

int SellItem(Player& player, int idx) {
	idx -= 1;
	if (player.GetInven().IsEmptyAt(idx)) {
		cout << " >> 해당 칸은 판매할 수 없습니다!\n";
		return -1;
	}

	const ITEM& item = player.GetInven().Get(idx);
	int price = item.GetSellPrice();
	cout << item.GetName() << "을/를 판매해 " << price << "골드를 얻었습니다!\n";
	player.AddGold(price);
	player.GetInven().Remove(idx);
	return 1;
}


