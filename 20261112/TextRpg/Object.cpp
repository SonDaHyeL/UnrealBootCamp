#include "Game.h"
#include <iostream>
using namespace std;

const char* ItemName(ITEMTYPE id) {
	switch (id) {
	case slime_shard: return "슬라임 조각";
	case nuts:        return "견과류";
	default:          return "(비어 있음)";
	}
}

void ShowInven(Player& player) {
	system("cls");
	cout << " ====== INVENTORY ======" << endl;
	for (int i = 0; i < INVEN_SIZE; i++) {
		cout << i << "." << ItemName((ITEMTYPE)player.inven[i]) << endl;
	}
	cout << " ====== ========= ======" << endl;
}
void GetItem(Player& player, ITEMTYPE ItemID) {
	while (1) {
		for (int i = 0; i < INVEN_SIZE; i++) {
			
			if (player.inven[i] == noItem) {
				player.inven[i] = ItemID;
				cout << ItemName(ItemID) << "을/를 획득했습니다!\n";
				return;
			}
		}
		cout << " >> 인벤토리가 가득 찼습니다! (" << INVEN_SIZE << "/" << INVEN_SIZE << ")\n";
		cout << "1. 기존 아이템을 버린다.\n";
		cout << "2. 아이템 습득을 포기한다.\n";
		if (Select() == 1) {
			int slot = DropItem(player);
			if (slot >= 0) {
				player.inven[slot] = ItemID;
				cout << ItemName(ItemID) << "을/를 획득했습니다!\n";
				return;
			}
		}
		else {
			cout << ItemName(ItemID) << "을/를 포기했습니다...\n";
			system("pause");
			return;
		}
	}
}

int DropItem(Player& player) {
	ShowInven(player);
	cout << ">> 버릴 아이템 번호를 선택하세요. (-1로 취소)" << endl;
	int idx = Select();
	if (idx < 0 || idx >= INVEN_SIZE) return -1;
	cout << ItemName((ITEMTYPE)player.inven[idx]) << "을/를 버렸습니다. \n";
	player.inven[idx] = noItem;
	return idx;
}