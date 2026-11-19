#include "Game.h"
#include <iostream>
using namespace std;


// ::인벤토리:: 기본 조작
int FindEmptySlot(const Player& player) { //인벤토리 빈 자리 판단
	for (int i = 0; i < INVEN_SIZE; ++i) {
		if (player.inven[i].IsEmpty()) return i;
	}
	return -1; //빈자리 없다
}

int SelectInvenSlot(Player& player) { //인벤토리 슬롯 내 선택
	ShowInven(player);
	cout << ">> 인벤토리 슬롯을 선택하세요. (0: 취소)\n";

	int input = Select();

	if (input < 1 || input > INVEN_SIZE) {
		cout << " >> 아무것도 선택되지 않았습니다." << endl;
		return -1;
	}
	int idx = input - 1;
	if (player.inven[idx].IsEmpty()) {
		cout << " >> 빈 칸입니다." << endl;
		return -1;
	}
	return idx;
}

void ShowInven(Player& player) { //인벤토리 출력하기 
	system("cls");
	cout << " ====== INVENTORY ======" << endl;
	for (int i = 0; i < INVEN_SIZE; i++) {
		cout << (i + 1) << "." << player.inven[i].GetName() << endl;
	}
	cout << " :: 소지금 : " << player.gold << "G" << endl;
	cout << " ====== ========= ======" << endl;
}

void RemoveItem(Player& player, int idx) {
	if (idx < 0 || idx >= INVEN_SIZE) return;
	for (int i = idx; i < INVEN_SIZE - 1; ++i) {
		player.inven[i] = player.inven[i + 1];
	}
	player.inven[INVEN_SIZE - 1].ItemID = noItem;
}


// ::인벤토리:: 아이템 획득 / 버림
void GetItem(Player& player, ITEMLIST ItemID) {
	ITEM newItem;
	newItem.ItemID = ItemID;

	if (ItemID <= noItem || ItemID >= ITEM_COUNT) return;

	while (1) {
		int idx = FindEmptySlot(player);
		if (idx != -1) {
			player.inven[idx] = newItem;
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
			system("pause");
			return;
		}
		else {
			cout << " <!> 잘못된 입력입니다. \n";
		}
	}
}

int DropItem(Player& player) {
	int idx = SelectInvenSlot(player);
	if (idx == -1) { return false; }
	cout << "[" << player.inven[idx].GetName() << "]을/를 버렸습니다. \n";
	RemoveItem(player, idx);
	return 1;
}


// ::상점:: 아이템 구매 / 판매
int BuyItem(Player& player, ITEMLIST ItemID) { // -1 골드 부족 | 0 인벤토리 가득 참 | 1 구매 성공
	ITEM newItem;
	newItem.ItemID = ItemID;

	int price = newItem.GetBuyPrice();

	if (player.gold < price) { //골드 부족으로 구매 실패
		cout << "<< 보유한 골드가 부족합니다!\n";
		return -1; 
	}

	int idx = FindEmptySlot(player);
	if (idx == -1) {
		cout << " >> 인벤토리가 가득 찼습니다! (" << INVEN_SIZE << "/" << INVEN_SIZE << ")\n";
		return 0;
	}
	player.gold -= price;
	player.inven[idx] = newItem;
	cout << "--[" << newItem.GetName() << "]을/를 구매했습니다!--\n";
	return 1;
}

int SellItem(Player& player, int idx) {
	
	if (idx < 0 || idx >= INVEN_SIZE || player.inven[idx].IsEmpty()) {
		cout << " >> 해당 칸은 판매할 수 없습니다!\n";
		return -1;
	}

	int price = player.inven[idx].GetSellPrice();
	cout << player.inven[idx].GetName() << "을/를 판매해 " << price << "골드를 얻었습니다!\n";
	player.AddGold(price);
	RemoveItem(player, idx);
	return 1;
}