#include "Game.h"
#include <iostream>
using namespace std;


void ShowInven(Player& player) {
	system("cls");
	cout << " ====== INVENTORY ======" << endl;
	for (int i = 0; i < INVEN_SIZE; i++) {
		cout << (i + 1) << "." << player.inven[i].GetName() << endl;
	}
	cout << " ====== ========= ======" << endl;
	system("pause");
}


int FindEmptySlot(const Player& player) { //인벤토리 빈 자리 판단
	for (int i = 0; i < INVEN_SIZE; ++i) {
		if (player.inven[i].IsEmpty()) return i;
	}
	return -1; //빈 자리 없다
}

int SelectInvenSlot(Player& player) {
	ShowInven(player);
	cout << ">> 선택할 인벤토리 슬롯을 선택하세요. (0: 취소)\n";
	int input = Select();
	if (input < 1 || input > INVEN_SIZE) return -1;
	return input - 1;
}


void GetItem(Player& player, ITEMLIST ItemID) {
	ITEM newItem;
	newItem.ItemID = ItemID;

	while (1) {
		int idx = FindEmptySlot(player);
		if (idx != -1) {
			player.inven[idx] = newItem;
			cout << newItem.GetName() << "을/를 획득했습니다!\n";
			return;
		}
		cout << " >> 인벤토리가 가득 찼습니다! (" << INVEN_SIZE << "/" << INVEN_SIZE << ")\n";
		cout << "1. 기존 아이템을 버린다.\n";
		cout << "2. 아이템 습득을 포기한다.\n";
		if (Select() == 1) {
			int slot = DropItem(player);
			if (slot >= 0) {
				player.inven[slot] = newItem;
				cout << newItem.GetName() << "을/를 획득했습니다!\n";
				return;
			}
		}
		else {
			cout << newItem.GetName() << "을/를 포기했습니다...\n";
			system("pause");
			return;
		}
	}
}

int DropItem(Player& player) {
	int idx = SelectInvenSlot(player);
	if (idx < 0) { return -1; }
	cout << player.inven[idx].GetName() << "을/를 버렸습니다. \n";
	player.inven[idx].ItemID = noItem;
	return idx;
}


int BuyItem(Player& player, ITEMLIST ItemID) { // -1 골드 부족 | 0 인벤토리 가득 참 | 1 구매 성공
	ITEM newItem;
	newItem.ItemID = ItemID;

	int price = newItem.GetBuyPrice();

	if (player.gold < price) { //골드 부족으로 구매 실패
		cout << "<< 보유한 골드가 부족합니다!\n";
		return -1; 
	}

	int idx = FindEmptySlot(player);
	if (idx != -1) {
		player.inven[idx] = newItem;
		player.gold -= price;
		cout << newItem.GetName() << "을/를 구매했습니다!\n";
		return 1;
	}
	cout << " >> 인벤토리가 가득 찼습니다! (" << INVEN_SIZE << "/" << INVEN_SIZE << ")\n";
	return 0;
}

int SellItem(Player& player, int idx) {
	
	if (idx < 0 || idx >= INVEN_SIZE || player.inven[idx].IsEmpty()) {
		cout << " >> 해당 칸을 판매할 수 없습니다!\n";
		return -1;
	}

	int price = player.inven[idx].GetSellPrice();
	cout << player.inven[idx].GetName() << "을/를 판매해 " << price << "골드를 얻었습니다!\n";
	player.inven[idx].ItemID = noItem;
	player.getGold(price);
	return 1;
}