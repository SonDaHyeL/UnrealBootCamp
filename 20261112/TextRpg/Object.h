#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>

class Player;
const int INVEN_SIZE = 8;


using ITEMLIST = int;
const ITEMLIST noItem = 0;

enum ITEMTYPE {
	NONE_EQUIP = -1,
	EQUIP_WEAPON,
	EQUIP_HEAD,
	EQUIP_CHEST,
	EQUIP_LEG,
	EQUIP_FOOT,
	EQUIP_ACCESSORY,
};

struct ItemData {
	bool exists = false;              // 파일에 있는 ID인지 (중간에 지운 ID는 false)
	std::string name = "비어 있음";
	int buyPrice = 0;
	int sellPrice = 0;
	ITEMTYPE slot = NONE_EQUIP;
	int atk = 0;
	int def = 0;
	int plushp = 0;
	int heal = 0;
	bool inShop = false;              // 상점판매 Y/N
	std::string description;
};

// ===== 아이템 표 (DataTable.cpp가 items.csv를 읽어 채운다) =====
const ItemData& GetItemData(ITEMLIST id);    // 없는 ID면 "비어 있음"
bool IsValidItem(ITEMLIST id);               // 파일에 있는 실제 아이템인지 (0 제외)
std::vector<ITEMLIST> GetShopItems();        // 상점판매 = Y 인 아이템 ID들


struct ITEM {
	ITEMLIST ItemID = noItem;

	const char* GetName() const { return GetItemData(ItemID).name.c_str(); }
	int GetBuyPrice() const { return GetItemData(ItemID).buyPrice; }
	int GetSellPrice() const { return GetItemData(ItemID).sellPrice; }

	bool IsEmpty() const { return ItemID == noItem; }
	bool IsEquipment() const { return GetEquipType() != NONE_EQUIP; }
	bool IsUseble() const { return GetHeal() > 0; }

	//아이템 효과 호출
	ITEMTYPE GetEquipType() const { return GetItemData(ItemID).slot; }
	int GetAtk() const { return GetItemData(ItemID).atk; }
	int GetDef() const { return GetItemData(ItemID).def; }
	int GetPlusHp() const { return GetItemData(ItemID).plushp; }
	int GetHeal() const { return GetItemData(ItemID).heal; }

	const std::string& GetDescription() const { return GetItemData(ItemID).description; }
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



//인벤토리 유틸리티
int SelectInvenSlot(Player& player);
std::string GetItemEffect(const ITEM& item);   // 효과만 (예: "ATK +3 DEF +2")
std::string GetItemInfo(const ITEM& item); //아이템 효과 출력

//인벤토리 추가 삭제
void GetItem(Player& player, ITEMLIST ItemID);
void DropItem(Player& player);

//상점 구매 판매
int BuyItem(Player& player, ITEMLIST ItemID);
void SellItem(Player& player, int idx);

//아이템 사용
bool UseItem(Player& player, int idx);
void DropItemAt(Player& player, int idx);


#endif