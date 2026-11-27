#include "SaveLoad.h"
#include "Quest.h"
#include <fstream>
#include <string>
#include <limits>

// 저장 파일 형식 (한 줄씩, 저장/불러오기 순서는 반드시 같아야 함)
//   버전
//   이름
//   직업 번호 레벨 경험치 현재HP 현재MP
//   (버전 2: 골드 퀘스트(0/1))
//	 버전 3 : 퀘스트 표 도입 (진행 퀘스트, 처치 수, 완료 기록)
//   인벤토리 아이템 ID x INVEN_SIZE
//   장비 아이템 ID x EQUIPSLOT_END
//
// 직업 이름, 기본 능력치, 성장치, 최대HP는 저장하지 않는다.
// 불러올 때 직업 번호로 Init()을 호출해 다시 계산한다. (밸런스를 바꿔도 예전 저장 파일에 반영됨)
//
// Player에 필드를 추가하면 SaveData/LoadData 양쪽을 같은 위치에 수정하고 SAVE_VERSION을 올릴 것.


namespace {
	const int SAVE_VERSION = 4;   // 4: 사운드, 스킬 시스템 도입 
	const char* SAVE_PATH = "Playerdata.txt";

	// Player::Init()이 처리하는 직업 번호 범위 (직업을 추가하면 같이 수정)
	const int JOB_MIN = 1;
	const int JOB_MAX = 4;

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
	out << player.job << ' ' << player.level << ' ' << player.exp << ' ' << player.hp << ' ' << player.mp << '\n';
	out << player.gold << ' ' << player.questId << ' ' << player.questKills << ' ' << player.lastQuestDone << '\n';

	for (int i = 0; i < INVEN_SIZE; ++i) {
		out << player.inven.Get(i).ItemID << ' ';
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
	if (!in || (version != SAVE_VERSION)) {
		return false;
	}

	// 읽는 도중 실패해도 기존 player가 망가지지 않도록 임시 객체에 먼저 읽는다.
	Player loaded = player;

	// 버전 숫자 뒤에 남은 내용(줄바꿈, 공백 등)을 줄 끝까지 버린다. (getline 전에 필수)
	in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(in, loaded.name);
	if (!in || loaded.name.empty()) {
		return false;
	}

	int job = 0, level = 1, exp = 0, hp = 0, mp = 0;
	in >> job >> level >> exp >> hp >> mp;
	if (!in || job < JOB_MIN || job > JOB_MAX || level < 1 || level > MAX_LEVEL || exp < 0) {
		return false;
	}

	// 직업 기본값·성장치·직업 이름을 다시 채운다. (Init은 level/exp/hp를 초기화하므로 반드시 먼저 호출)
	loaded.job = job;
	loaded.Init();

	// Init 뒤에 저장된 진행도를 덮어쓴다.
	loaded.level = level;
	loaded.exp = exp;
	loaded.hp = hp;
	loaded.mp = mp;
	
	// 경험치가 이미 다음 레벨 요구량 이상이면 "다음 LV까지"가 음수로 보이므로 직전 값으로 맞춘다.
	if (loaded.exp >= loaded.GetNeedExp()) loaded.exp = loaded.GetNeedExp() - 1;

	in >> loaded.gold >> loaded.questId >> loaded.questKills >> loaded.lastQuestDone;
	if (!in || loaded.gold < 0) {
		return false;
	}

	// 퀘스트 표에서 지워진 퀘스트를 진행 중이었으면 취소
	if (loaded.questId != 0 && !GetQuestData(loaded.questId).exists) {
		loaded.questId = 0;
		loaded.questKills = 0;
	}

	for (int i = 0; i < INVEN_SIZE; ++i) {
		int id = 0;
		in >> id;
		loaded.inven.SetSlot(i, IsValidItem(id) ? id : noItem);   // 지워진 아이템은 빈 칸으로
	}

	for (int i = 0; i < EQUIPSLOT_END; ++i) {
		int id = 0;
		in >> id;
		loaded.equip[i].ItemID = IsValidItem(id) ? id : noItem;
	}

	if (in.fail()) {
		return false;
	}

	if (loaded.hp > loaded.GetMaxHp()) loaded.hp = loaded.GetMaxHp(); // 밸런스 변경으로 최대HP가 줄었을 때
	if (loaded.hp < 1) loaded.hp = 1;

	if (loaded.mp > loaded.GetMaxMp()) loaded.mp = loaded.GetMaxMp(); // 밸런스 변경으로 최대MP가 줄었을 때
	if (loaded.mp < 0) loaded.mp = 0;

	player = loaded;
	return true;
}
