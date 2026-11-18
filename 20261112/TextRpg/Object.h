#ifndef OBJECT_H
#define OBJECT_H

struct Player;
const int INVEN_SIZE = 8;

enum ITEMTYPE {
	noItem = 0,
	slime_shard,
	nuts,

};

struct ITEM {
	ITEMTYPE ItemID = noItem;
	const char* GetName() const {
		switch (ItemID)
		{
		case noItem:
			return "비어 있음";
		case slime_shard:
			return "슬라임 조각";
		case nuts:
			return "견과류";
		default:
			return "알 수 없음";
		}
	}

};

void ShowInven(Player& player);
void GetItem(Player& player, ITEMTYPE ItemID);
int DropItem(Player& player);

#endif