#ifndef INVENTORY_H
#define INVENTORY_H

#include "Object.h"

class Inventory {
public:
	// 빈 칸에 아이템을 넣는다. 꽉 찼거나 잘못된 ID면 false.
	bool Add(ITEMLIST id);

	// idx 칸의 아이템을 지우고, 뒤에 있는 아이템을 한 칸씩 앞으로 당긴다.
	void Remove(int idx);

	// 첫 번째 빈 칸의 번호. 빈 칸이 없으면 -1.
	int FindEmptySlot() const;
	bool IsFull() const { return FindEmptySlot() == -1; }

	//유효 범위 검사
	bool IsValidIndex(int idx) const { return idx >= 0 && idx < INVEN_SIZE; }
	bool IsEmptyAt(int idx) const { return !IsValidIndex(idx) || slots[idx].IsEmpty(); }

	// idx 칸의 아이템. 범위 밖이면 "비어 있음" 아이템을 돌려준다.
	const ITEM& Get(int idx) const;

	// 저장 파일 불러오기 전용: 칸에 ID를 그대로 써 넣는다. 
	void SetSlot(int idx, ITEMLIST id);

private:
	ITEM slots[INVEN_SIZE];
};

#endif // !INVENTORY_H
