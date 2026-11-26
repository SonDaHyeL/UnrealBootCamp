#include "DataTable.h"
#include "Object.h"
#include "Enemy.h"
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

namespace {
	vector<ItemData> itemTable;   // 칸 번호 = 아이템 ID (0번은 빈 칸)
	vector<EnemyData> enemyTable;   // 칸 번호 = enemy ID (0번은 빈 칸)

	// 앞뒤 공백 제거
	string Trim(const string& s) {
		size_t b = s.find_first_not_of(" \t");
		size_t e = s.find_last_not_of(" \t");
		return (b == string::npos) ? "" : s.substr(b, e - b + 1);
	}

	// 엑셀은 쉼표가 든 칸을 "..."로 감싸고, 안의 " 는 ""로 저장한다 → 원래대로 되돌림
	string Unquote(const string& s) {
		if (s.size() < 2 || s.front() != '"' || s.back() != '"') return s;
		string out;
		for (size_t i = 1; i + 1 < s.size(); ++i) {
			out += s[i];
			if (s[i] == '"' && s[i + 1] == '"') ++i;
		}
		return out;
	}

	// 한 줄을 쉼표로 나눈다. 마지막 칸(설명)은 줄의 나머지 전부 → 설명에는 쉼표가 있어도 됨
	vector<string> SplitCsv(const string& line, int fieldCount) {
		vector<string> fields;
		size_t start = 0;
		for (int i = 0; i < fieldCount - 1; ++i) {
			size_t comma = line.find(',', start);
			if (comma == string::npos) break;
			fields.push_back(line.substr(start, comma - start));
			start = comma + 1;
		}
		fields.push_back(line.substr(start));
		for (string& f : fields) f = Unquote(Trim(f));
		return fields;
	}

	// 칸 전체가 숫자일 때만 성공 ("12a", 빈 칸은 실패)
	bool ToInt(const string& s, int& out) {
		if (s.empty()) return false;
		char* end = nullptr;
		long v = strtol(s.c_str(), &end, 10);
		if (*end != '\0') return false;
		out = (int)v;
		return true;
	}

	// "무기", "머리", ... "장신구", "-" → ITEMTYPE
	bool ToType(const string& s, ITEMTYPE& out) {
		if (s.empty() || s == "-") { out = NONE_EQUIP;      return true; }
		if (s == "장신구") { out = EQUIP_ACCESSORY; return true; }
		for (int i = SLOT_WEAPON; i <= SLOT_FOOT; ++i) {     // 무기~발은 EQUIPSLOT_NAME과 같은 이름
			if (s == EQUIPSLOT_NAME[i]) { out = (ITEMTYPE)i; return true; }
		}
		return false;
	}

	bool LoadItems(const string& path, string& error) {
		ifstream in(path);
		if (!in) { error = path + " 파일을 열 수 없습니다."; return false; }

		const int FIELD_COUNT = 11;
		itemTable.assign(1, ItemData());           // 0번 = 빈 칸

		string line;
		int lineNo = 1;
		getline(in, line);                         // 첫 줄은 제목 줄 → 건너뜀
		while (getline(in, line)) {
			++lineNo;
			if (!line.empty() && line.back() == '\r') line.pop_back();
			vector<string> f = SplitCsv(line, FIELD_COUNT);
			if (f[0].empty()) continue;            // 빈 줄 (엑셀이 남기는 ",,,,,," 포함)

			string where = path + " " + to_string(lineNo) + "번째 줄: ";
			if ((int)f.size() < FIELD_COUNT) { error = where + "칸 수가 부족합니다."; return false; }

			int id = 0;
			if (!ToInt(f[0], id) || id <= 0) { error = where + "ID는 1 이상의 숫자여야 합니다."; return false; }

			ItemData d;
			d.exists = true;
			d.name = f[1];
			if (d.name.empty()) { error = where + "이름이 비어 있습니다."; return false; }
			if (!ToType(f[2], d.slot)) { error = where + "종류는 -, 무기, 머리, 몸, 다리, 발, 장신구 중 하나여야 합니다."; return false; }

			// 숫자 칸 6개 (구매가~회복량)를 차례로 읽는다
			const char* colName[] = { "구매가", "판매가", "공격", "방어", "추가HP", "회복량" };
			int* target[] = { &d.buyPrice, &d.sellPrice, &d.atk, &d.def, &d.plushp, &d.heal };
			for (int c = 0; c < 6; ++c) {
				if (!ToInt(f[3 + c], *target[c])) {
					error = where + colName[c] + " 칸이 숫자가 아닙니다.";
					return false;
				}
			}
			d.inShop = (f[9] == "Y" || f[9] == "y");
			d.description = f[10];

			if (id >= (int)itemTable.size()) itemTable.resize(id + 1);   // 중간 빈 ID는 exists=false
			if (itemTable[id].exists) { error = where + "ID " + to_string(id) + "번이 중복되었습니다."; return false; }
			itemTable[id] = d;
		}

		if (itemTable.size() <= 1) { error = path + "에 아이템이 하나도 없습니다."; return false; }
		return true;
	}


	bool LoadEnemies(const string& path, string& error) {
		ifstream in(path);
		if (!in) { error = path + " 파일을 열 수 없습니다."; return false; }

		const int FIELD_COUNT = 11;
		enemyTable.assign(1, EnemyData());

		string line;
		int lineNo = 1;
		getline(in, line);                         // 제목 줄 건너뜀
		while (getline(in, line)) {
			++lineNo;
			if (!line.empty() && line.back() == '\r') line.pop_back();
			vector<string> f = SplitCsv(line, FIELD_COUNT);
			if (f[0].empty()) continue;

			string where = path + " " + to_string(lineNo) + "번째 줄: ";
			if ((int)f.size() < FIELD_COUNT) { error = where + "칸 수가 부족합니다."; return false; }

			int id = 0;
			if (!ToInt(f[0], id) || id <= 0) { error = where + "ID는 1 이상의 숫자여야 합니다."; return false; }

			EnemyData d;
			d.exists = true;
			d.name = f[1];
			if (d.name.empty()) { error = where + "이름이 비어 있습니다."; return false; }

			// 숫자 칸 8개 (공격~희귀확률)
			const char* colName[] = { "공격", "방어", "HP", "경험치", "골드", "드롭아이템ID", "희귀드롭ID", "희귀확률" };
			int* target[] = { &d.attack, &d.defense, &d.maxHp, &d.exp, &d.gold, &d.dropItem, &d.rareItem, &d.rareChance };
			for (int c = 0; c < 8; ++c) {
				if (!ToInt(f[2 + c], *target[c])) {
					error = where + colName[c] + " 칸이 숫자가 아닙니다.";
					return false;
				}
			}
			d.description = f[10];

			// 값 검사 (아이템 ID는 items.csv에 실제로 있어야 함)
			if (d.maxHp < 1) { error = where + "HP는 1 이상이어야 합니다."; return false; }
			if (d.dropItem != noItem && !IsValidItem(d.dropItem)) {
				error = where + "드롭아이템ID " + to_string(d.dropItem) + "번은 items.csv에 없습니다."; return false;
			}
			if (d.rareChance < 0 || d.rareChance > 100) { error = where + "희귀확률은 0~100이어야 합니다."; return false; }
			if (d.rareChance > 0 && !IsValidItem(d.rareItem)) {
				error = where + "희귀드롭ID " + to_string(d.rareItem) + "번은 items.csv에 없습니다."; return false;
			}

			if (id >= (int)enemyTable.size()) enemyTable.resize(id + 1);
			if (enemyTable[id].exists) { error = where + "ID " + to_string(id) + "번이 중복되었습니다."; return false; }
			enemyTable[id] = d;
		}

		if (enemyTable.size() <= 1) { error = path + "에 몬스터가 하나도 없습니다."; return false; }
		return true;
	}
}

// ===== 공개 함수 =====
bool LoadGameData(string& error) {
	return LoadItems("items.csv", error) && LoadEnemies("monsters.csv", error);
}

const ItemData& GetItemData(ITEMLIST id) {
	static const ItemData EMPTY;            // 없는 ID에 돌려줄 "비어 있음"
	if (!IsValidItem(id)) return EMPTY;
	return itemTable[id];
}

bool IsValidItem(ITEMLIST id) {
	return id > 0 && id < (int)itemTable.size() && itemTable[id].exists;
}

vector<ITEMLIST> GetShopItems() {
	vector<ITEMLIST> list;
	for (int id = 1; id < (int)itemTable.size(); ++id)
		if (itemTable[id].exists && itemTable[id].inShop) list.push_back(id);
	return list;
}

const EnemyData& GetEnemyData(int id) {
	static const EnemyData EMPTY;
	if (id <= 0 || id >= (int)enemyTable.size() || !enemyTable[id].exists) return EMPTY;
	return enemyTable[id];
}

vector<int> GetEnemyIds() {
	vector<int> list;
	for (int id = 1; id < (int)enemyTable.size(); ++id)
		if (enemyTable[id].exists) list.push_back(id);
	return list;
}