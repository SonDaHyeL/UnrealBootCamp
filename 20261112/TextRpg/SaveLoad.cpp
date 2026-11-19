#include "SaveLoad.h"
#include <fstream>
#include <string>

// 저장 파일 형식 (한 줄씩, 저장/불러오기 순서는 반드시 같아야 함)
//   버전
//   이름
//   직업 이름
//   직업 번호
//   공격력 방어력
//   최대HP 현재HP
//   골드 퀘스트(0/1)
//   인벤토리 아이템 ID x INVEN_SIZE
//   장비 아이템 ID x EQUIPSLOT_END
//
// Player에 필드를 추가하면 SaveData/LoadData 양쪽을 같은 위치에 수정하고 SAVE_VERSION을 올릴 것.
namespace {
	const int SAVE_VERSION = 1;
	const char* SAVE_PATH = "Playerdata.txt";

	// 파일에서 읽은 숫자가 유효한 아이템 ID인지 확인
	bool IsValidItemId(int id) {
		return id >= 0 && id < ITEM_COUNT;
	}
}

bool HasSaveData() {
	std::ifstream in(SAVE_PATH);
	return in.good();
}

bool SaveData(const Player& player) {
	std::ofstream out(SAVE_PATH);
	if (!out) {
		return false;
	}

	out << SAVE_VERSION << '\n';
	out << player.name << '\n';
	out << player.jobName << '\n';
	out << player.job << '\n';
	out << player.attack << ' ' << player.defense << '\n';
	out << player.maxHp << ' ' << player.hp << '\n';
	out << player.gold << ' ' << player.quest << '\n';

	for (int i = 0; i < INVEN_SIZE; ++i) {
		out << player.inven[i].ItemID << ' ';
	}
	out << '\n';

	for (int i = 0; i < EQUIPSLOT_END; ++i) {
		out << player.equip[i].ItemID << ' ';
	}
	out << '\n';

	return out.good();
}

bool LoadData(Player& player) {
	std::ifstream in(SAVE_PATH);
	if (!in) {
		return false;
	}

	int version = 0;
	in >> version;
	if (!in || version != SAVE_VERSION) {
		return false;
	}

	// 읽는 도중 실패해도 기존 player가 망가지지 않도록 임시 객체에 먼저 읽는다.
	Player loaded = player;

	in.ignore(); // 숫자 뒤에 남은 줄바꿈 제거 (getline 전에 필수)
	std::getline(in, loaded.name);
	std::getline(in, loaded.jobName);

	in >> loaded.job;
	in >> loaded.attack >> loaded.defense;
	in >> loaded.maxHp >> loaded.hp;
	in >> loaded.gold >> loaded.quest;

	for (int i = 0; i < INVEN_SIZE; ++i) {
		int id = 0;
		in >> id;
		if (!IsValidItemId(id)) {
			return false;
		}
		loaded.inven[i].ItemID = static_cast<ITEMLIST>(id);
	}

	for (int i = 0; i < EQUIPSLOT_END; ++i) {
		int id = 0;
		in >> id;
		if (!IsValidItemId(id)) {
			return false;
		}
		loaded.equip[i].ItemID = static_cast<ITEMLIST>(id);
	}

	if (in.fail()) {
		return false;
	}

	player = loaded;
	return true;
}
