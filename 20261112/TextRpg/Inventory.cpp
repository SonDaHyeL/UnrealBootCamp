#include "Inventory.h"
#include <iostream>
using namespace std;

bool Inventory::Add(ITEMLIST id) {
	if (id <= noItem || id >= ITEM_COUNT) return false;

	int idx = FindEmptySlot();
	if (idx == -1) return false;

	slots[idx].ItemID = id;
	return true;
}

void Inventory::Remove(int idx) {
	if (!IsValidIndex(idx)) return;
	for (int i = idx; i < INVEN_SIZE - 1; ++i) {
		slots[i] = slots[i + 1];
	}
	slots[INVEN_SIZE - 1].ItemID = noItem;
}

int Inventory::FindEmptySlot() const {
	for (int i = 0; i < INVEN_SIZE; ++i) {
		if (slots[i].IsEmpty()) return i;
	}
	return -1; //빈자리 없다
}

const ITEM& Inventory::Get(int idx) const {
	static const ITEM EMPTY_ITEM; // 범위 밖 요청에 돌려줄 빈 아이템
	if (!IsValidIndex(idx)) return EMPTY_ITEM;
	return slots[idx];
}

void Inventory::Show() const {
	for (int i = 0; i < INVEN_SIZE; i++) {
		cout << (i + 1) << "." << slots[i].GetName() << endl;
	}
}

void Inventory::SetSlot(int idx, ITEMLIST id) {
	if (!IsValidIndex(idx)) return;
	slots[idx].ItemID = id;
}
