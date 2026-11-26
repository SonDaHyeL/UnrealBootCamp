#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Player;
const int INVEN_SIZE = 8;
const int ACCESSORY_COUNT = 3;

enum ITEMLIST {
	noItem = 0,
	slime_shard,
	nuts,
	wooden_sword,
	leaf_crown,
	flower_ring,
	heal_potion,
	ITEM_COUNT
};

enum ITEMTYPE {
	NONE_EQUIP = -1,
	EQUIP_WEAPON,
	EQUIP_HEAD,
	EQUIP_CHEST,
	EQUIP_LEG,
	EQUIP_FOOT,
	EQUIP_ACCESSORY,
};

enum EQUIPSLOT {
	SLOT_WEAPON,
	SLOT_HEAD,
	SLOT_CHEST,
	SLOT_LEG,
	SLOT_FOOT, //무기~발까지. ITEMTYPE과 순서 맞출것!!!!
	SLOT_ACCESSORY1, //악세사리 칸 3개
	SLOT_ACCESSORY2,
	SLOT_ACCESSORY3,
	EQUIPSLOT_END
};

const char* const EQUIPSLOT_NAME[EQUIPSLOT_END]{
	"무기", "머리", "몸", "다리", "발", "장신구1", "장신구2", "장신구3"
};

struct ItemData {
	const char* name;
	int buyPrice;
	int sellPrice;
	ITEMTYPE slot;
	int atk;
	int def;
	int plushp;
	int heal;
};

const ItemData  ITEM_TABLE[ITEM_COUNT] = {
	//새로운 것 추가는 늘 맨 아래. Enum 순서와 맞게 쓸 것
	//   이름   | 구매가 | 판매가 | 종류 | 공격 | 방어 | 추가 HP | 힐량
	{ "비어 있음", 0, 0, NONE_EQUIP, 0, 0, 0, 0 },
	{ "슬라임 조각", 10, 5, NONE_EQUIP, 0, 0, 0, 0 },
	{ "견과류", 5, 3, NONE_EQUIP, 0, 0, 0, 0 },
	{ "나무 검", 10, 7, 	EQUIP_WEAPON, 3, 0, 0, 0 },
	{ "나뭇잎 왕관", 8, 4, EQUIP_HEAD, 0, 2, 10, 0 },
	{ "꽃 반지", 6, 4, EQUIP_ACCESSORY, 0, 1, 30, 0 },
	{ "회복 포션", 5, 2, NONE_EQUIP, 0, 0, 0, 30 },
};

struct ITEM {
	ITEMLIST ItemID = noItem;

	const char* GetName() const { return ITEM_TABLE[ItemID].name; }
	int GetBuyPrice() const { return ITEM_TABLE[ItemID].buyPrice; }
	int GetSellPrice() const { return ITEM_TABLE[ItemID].sellPrice; }
		
	bool IsEmpty() const { return ItemID == noItem; }
	bool IsEquipment() const { return GetEquipType() != NONE_EQUIP; }
	bool IsUseble() const { return GetHeal() > 0; }

	//아이템 효과 호출
	ITEMTYPE GetEquipType() const { return ITEM_TABLE[ItemID].slot; }
	int GetAtk() const {return ITEM_TABLE[ItemID].atk; }
	int GetDef() const {return ITEM_TABLE[ItemID].def; }
	int GetPlusHp() const {return ITEM_TABLE[ItemID].plushp; }
	int GetHeal() const {return ITEM_TABLE[ItemID].heal; }
};

//인벤토리 유틸리티
int SelectInvenSlot(Player& player);
std::string GetItemInfo(const ITEM& item); //아이템 효과 출력

//인벤토리 추가 삭제
void GetItem(Player& player, ITEMLIST ItemID);
int DropItem(Player& player);

//상점 구매 판매
int BuyItem(Player& player, ITEMLIST ItemID);
int SellItem(Player& player, int idx);

//아이템 사용
bool UseItem(Player& player, int idx);
void DropItemAt(Player& player, int idx);



//장비 관리 (미구현)
//int  FindEquipSlot(const Player& player, ITEMTYPE type);
//bool EquipItem(Player& player, int idx);
//bool UnequipItem(Player& player, int slot);

#endif