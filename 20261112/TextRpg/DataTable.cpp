#include "DataTable.h"
#include "Object.h"
#include "Enemy.h"
#include "Quest.h"
#include "Skill.h"
#include "Player.h"
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

namespace {
	vector<ItemData> itemTable;     // 칸 번호 = 아이템 ID (0번은 빈 칸)
	vector<EnemyData> enemyTable;   // 칸 번호 = 몬스터 ID (0번은 안 씀)
	vector<QuestData> questTable;   // 칸 번호 = 퀘스트 ID (0번은 안 씀)
	vector<SkillData> skillTable;   // 칸 번호 = 스킬 ID (0번은 안 씀)

	// ===================== 글자 다루기 =====================

	// 앞뒤 공백 제거
	string Trim(const string& s) {
		size_t b = s.find_first_not_of(" \t");
		size_t e = s.find_last_not_of(" \t");
		return (b == string::npos) ? "" : s.substr(b, e - b + 1);
	}

	// 한 줄을 쉼표로 나눈다. "..."로 감싼 칸 안의 쉼표는 나누지 않고, "" 는 " 하나로 읽는다. (엑셀 CSV 규칙)
	vector<string> SplitCsv(const string& line) {
		vector<string> fields;
		string cur;
		bool inQuotes = false;
		for (size_t i = 0; i < line.size(); ++i) {
			char c = line[i];
			if (inQuotes) {
				if (c == '"' && i + 1 < line.size() && line[i + 1] == '"') { cur += '"'; ++i; }
				else if (c == '"') inQuotes = false;
				else cur += c;
			}
			else if (c == '"') inQuotes = true;
			else if (c == ',') { fields.push_back(Trim(cur)); cur.clear(); }
			else cur += c;
		}
		fields.push_back(Trim(cur));
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
		for (int i = SLOT_WEAPON; i <= SLOT_FOOT; ++i) {
			if (s == EQUIPSLOT_NAME[i]) { out = (ITEMTYPE)i; return true; }
		}
		return false;
	}

	// "전사" → 1, "궁수" → 2 ...
	bool ToJob(const string& s, int& out) {
		if (s == "몬스터") { out = 0; return true; }
		for (int j = 1; j <= JOB_COUNT; ++j) {
			if (s == JOB_NAME[j]) { out = j; return true; }
		}
		return false;
	}

	// "공격" → SkillEffect::ATTACK ...  (enum 순서와 같은 순서로 적을 것)
	bool ToEffect(const string& s, SkillEffect& out) {
		const char* names[] = { "공격", "방어무시", "치명타", "방어", "회복" };
		for (int i = 0; i < 5; ++i) {
			if (s == names[i]) { out = (SkillEffect)i; return true; }
		}
		return false;
	}
	bool ToStatus(const string& s, Status& out) {
		if (s.empty() || s == "-") { out = Status::NONE;   return true; }
		if (s == "독") { out = Status::POISON; return true; }
		if (s == "기절") { out = Status::STUN;   return true; }
		return false;
	}
	// ===================== CSV 공통 처리 =====================

	// CSV 한 줄 (제목 줄과 빈 줄은 이미 걸러진 상태)
	struct CsvRow {
		int id = 0;                 // 첫 칸 (1 이상 보장)
		vector<string> f;           // 모든 칸
		string where;               // 오류 메시지 앞부분 (예: "items.csv 3번째 줄: ")
	};

	// 파일을 읽어 줄 목록을 만든다. 모든 표가 "ID, 이름, ..."으로 시작하므로 그 두 칸까지 여기서 검사.
	bool ReadCsv(const string& path, int fieldCount, vector<CsvRow>& rows, string& error) {
		ifstream in(path);
		if (!in) { error = path + " 파일을 열 수 없습니다."; return false; }

		string line;
		int lineNo = 1;
		getline(in, line);                                  // 첫 줄은 제목 줄
		while (getline(in, line)) {
			++lineNo;
			if (!line.empty() && line.back() == '\r') line.pop_back();

			CsvRow row;
			row.f = SplitCsv(line);
			if (row.f[0].empty()) continue;                 // 빈 줄 (엑셀이 남기는 ",,,,," 포함)

			row.where = path + " " + to_string(lineNo) + "번째 줄: ";
			if ((int)row.f.size() < fieldCount) { error = row.where + "칸 수가 부족합니다."; return false; }
			if (!ToInt(row.f[0], row.id) || row.id <= 0) { error = row.where + "ID는 1 이상의 숫자여야 합니다."; return false; }
			if (row.f[1].empty()) { error = row.where + "이름이 비어 있습니다."; return false; }

			rows.push_back(row);
		}
		return true;
	}

	// first번째 칸부터 숫자 칸들을 차례로 읽는다. names[i] 칸의 값 → *targets[i]
	bool ReadInts(const CsvRow& row, int first, const vector<const char*>& names,
		const vector<int*>& targets, string& error) {
		for (size_t c = 0; c < targets.size(); ++c) {
			if (!ToInt(row.f[first + c], *targets[c])) {
				error = row.where + names[c] + " 칸이 숫자가 아닙니다.";
				return false;
			}
		}
		return true;
	}

	// 아이템 ID 칸 검사: 0(없음)이거나 items.csv에 있는 ID여야 한다
	bool CheckItemId(const CsvRow& row, const string& colName, ITEMLIST id, string& error) {
		if (id == noItem || IsValidItem(id)) return true;
		error = row.where + colName + " " + to_string(id) + "번은 items.csv에 없습니다.";
		return false;
	}

	// 표(table)의 row.id 칸에 d를 넣는다. 표가 작으면 늘리고(중간 빈 ID는 exists = false), 이미 있으면 중복 오류.
	// T는 ItemData, EnemyData, QuestData 중 하나 (셋 다 exists 멤버가 있어야 함)
	template <typename T>
	bool PutRow(vector<T>& table, const CsvRow& row, const T& d, string& error) {
		if (row.id >= (int)table.size()) table.resize(row.id + 1);
		if (table[row.id].exists) { error = row.where + "ID " + to_string(row.id) + "번이 중복되었습니다."; return false; }
		table[row.id] = d;
		return true;
	}

	// 표에서 id를 찾는다. 범위 밖이거나 없는 ID면 빈 값
	template <typename T>
	const T& Find(const vector<T>& table, int id) {
		static const T EMPTY;
		if (id <= 0 || id >= (int)table.size() || !table[id].exists) return EMPTY;
		return table[id];
	}

	// ===================== 표마다 다른 부분 =====================

	bool LoadItems(const string& path, string& error) {
		vector<CsvRow> rows;
		if (!ReadCsv(path, 13, rows, error)) return false;

		itemTable.assign(1, ItemData());                    // 0번 = 빈 칸
		for (const CsvRow& row : rows) {
			ItemData d;
			d.exists = true;
			d.name = row.f[1];
			if (!ToType(row.f[2], d.slot)) {
				error = row.where + "종류는 -, 무기, 머리, 몸, 다리, 발, 장신구 중 하나여야 합니다."; return false;
			}
			if (!ReadInts(row, 3, { "구매가", "판매가", "공격", "방어", "추가HP", "회복량", "MP회복"},
				{ &d.buyPrice, &d.sellPrice, &d.atk, &d.def, &d.plushp, &d.heal, &d.mpheal }, error)) return false;
			d.inShop = (row.f[11] == "Y" || row.f[10] == "y");
			d.description = row.f[12];

			if (!PutRow(itemTable, row, d, error)) return false;
		}

		if (itemTable.size() <= 1) { error = path + "에 아이템이 하나도 없습니다."; return false; }
		return true;
	}

	bool LoadEnemies(const string& path, string& error) {
		vector<CsvRow> rows;
		if (!ReadCsv(path, 13, rows, error)) return false;

		enemyTable.assign(1, EnemyData());
		for (const CsvRow& row : rows) {
			EnemyData d;
			d.exists = true;
			d.name = row.f[1];
			if (!ReadInts(row, 2, { "공격", "방어", "HP", "경험치", "골드", "드롭아이템ID", "희귀드롭ID", "희귀확률", "스킬ID", "스킬확률" },
				{ &d.attack, &d.defense, &d.maxHp, &d.exp, &d.gold, &d.dropItem, &d.rareItem, &d.rareChance, &d.skill, &d.skillChance }, error)) return false;
			d.description = row.f[12];

			if (d.maxHp < 1) { error = row.where + "HP는 1 이상이어야 합니다."; return false; }
			if (d.rareChance < 0 || d.rareChance > 100) { error = row.where + "희귀확률은 0~100이어야 합니다."; return false; }
			if (d.rareChance > 0 && d.rareItem == noItem) { error = row.where + "희귀확률이 있으면 희귀드롭ID가 필요합니다."; return false; }
			if (d.skillChance < 0 || d.skillChance > 100) { error = row.where + "스킬확률은 0~100이어야 합니다."; return false; }
			if (d.skillChance > 0 && !Find(skillTable, d.skill).exists) {
				error = row.where + "스킬ID " + to_string(d.skill) + "번은 skills.csv에 없습니다."; return false;
			}
			if (!CheckItemId(row, "드롭아이템ID", d.dropItem, error)) return false;
			if (!CheckItemId(row, "희귀드롭ID", d.rareItem, error)) return false;

			if (!PutRow(enemyTable, row, d, error)) return false;
		}

		if (enemyTable.size() <= 1) { error = path + "에 몬스터가 하나도 없습니다."; return false; }
		return true;
	}

	bool LoadQuests(const string& path, string& error) {
		vector<CsvRow> rows;
		if (!ReadCsv(path, 9, rows, error)) return false;

		questTable.assign(1, QuestData());
		for (const CsvRow& row : rows) {
			QuestData d;
			d.exists = true;
			d.name = row.f[1];
			if (!ReadInts(row, 2, { "대상몬스터ID", "목표수", "보상골드", "보상경험치", "보상아이템ID" },
				{ &d.target, &d.count, &d.rewardGold, &d.rewardExp, &d.rewardItem }, error)) return false;
			d.request = row.f[7];
			d.complete = row.f[8];

			if (!Find(enemyTable, d.target).exists) {
				error = row.where + "대상몬스터ID " + to_string(d.target) + "번은 monsters.csv에 없습니다."; return false;
			}
			if (d.count < 1) { error = row.where + "목표수는 1 이상이어야 합니다."; return false; }
			if (!CheckItemId(row, "보상아이템ID", d.rewardItem, error)) return false;

			if (!PutRow(questTable, row, d, error)) return false;
		}
		return true;     // 퀘스트는 0개여도 됨 (촌장이 할 말이 없을 뿐)
	}

	bool LoadSkills(const string& path, string& error) {
		vector<CsvRow> rows;
		if (!ReadCsv(path, 12, rows, error)) return false;

		skillTable.assign(1, SkillData());
		for (const CsvRow& row : rows) {
			SkillData d;
			d.exists = true;
			d.name = row.f[1];
			if (!ToJob(row.f[2], d.job)) { error = row.where + "직업은 전사, 궁수, 도적, 마법사, 몬스터 중 하나여야 합니다."; return false; }
			if (!ReadInts(row, 3, { "습득Lv", "MP" }, { &d.level, &d.mp }, error)) return false;
			if (!ToEffect(row.f[5], d.effect)) { error = row.where + "효과는 공격, 방어무시, 치명타, 방어, 회복 중 하나여야 합니다."; return false; }
			if (!ReadInts(row, 6, { "배율%", "횟수" }, { &d.power, &d.hits }, error)) return false;
			if (!ToStatus(row.f[8], d.status)) { error = row.where + "상태이상은 -, 독, 기절 중 하나여야 합니다."; return false; }
			if (!ReadInts(row, 9, { "확률%", "지속턴" }, { &d.statusChance, &d.statusTurns }, error)) return false;
			d.description = row.f[11];

			if (d.level < 1 || d.mp < 0 || d.power < 0 || d.hits < 1) {
				error = row.where + "습득Lv와 횟수는 1 이상, MP와 배율%는 0 이상이어야 합니다."; return false;
			}
			if (d.status != Status::NONE && (d.statusChance < 1 || d.statusChance > 100 || d.statusTurns < 1)) {
				error = row.where + "상태이상이 있으면 확률%는 1~100, 지속턴은 1 이상이어야 합니다."; return false;
			}
			if (!PutRow(skillTable, row, d, error)) return false;
		}
		return true;     // 스킬이 0개여도 됨
	}
}

// ===== 공개 함수 =====
bool LoadGameData(string& error) {
	// 읽는 순서 중요!
	return LoadItems("items.csv", error)
		&& LoadSkills("skills.csv", error)
		&& LoadEnemies("monsters.csv", error)
		&& LoadQuests("quests.csv", error);
}

const ItemData& GetItemData(ITEMLIST id) { return Find(itemTable, id); }
bool IsValidItem(ITEMLIST id) { return Find(itemTable, id).exists; }
const EnemyData& GetEnemyData(int id) { return Find(enemyTable, id); }
const QuestData& GetQuestData(int id) { return Find(questTable, id); }
const SkillData& GetSkillData(int id) { return Find(skillTable, id); }

vector<int> GetSkillsFor(int job, int level) {
	vector<int> list;
	for (int id = 1; id < (int)skillTable.size(); ++id) {
		const SkillData& s = skillTable[id];
		if (s.exists && s.job == job && s.level <= level) list.push_back(id);
	}
	return list;
}

vector<ITEMLIST> GetShopItems() {
	vector<ITEMLIST> list;
	for (int id = 1; id < (int)itemTable.size(); ++id)
		if (itemTable[id].exists && itemTable[id].inShop) list.push_back(id);
	return list;
}

vector<int> GetEnemyIds() {
	vector<int> list;
	for (int id = 1; id < (int)enemyTable.size(); ++id)
		if (enemyTable[id].exists) list.push_back(id);
	return list;
}

int GetNextQuestId(int lastDone) {
	for (int id = lastDone + 1; id < (int)questTable.size(); ++id)
		if (questTable[id].exists) return id;
	return 0;
}