#include "Player.h"
#include "Quest.h"
#include "Skill.h"
#include "UI.h"


using namespace std;

void Player::Init() {
	switch (job) {
	case 1: //전사: 튼튼하고 꾸준함 (HP·방어 높음)
		attack = 4;  defense = 3;  basehp = 150;  baseMp = 10;
		growAtk = 2; growDef = 2;  growHP = 15;   growMp = 2;
		break;
	case 2: //궁수: 균형형
		attack = 4;  defense = 2;  basehp = 110;  baseMp = 15;
		growAtk = 2; growDef = 1;  growHP = 10;   growMp = 3;
		break;
	case 3: //도적: 공격적이지만 약함 (공격 높음, 방어·HP 낮음)
		attack = 5;  defense = 1;  basehp = 110;  baseMp = 15;
		growAtk = 3; growDef = 1;  growHP = 8;    growMp = 2;
		break;
	case 4: //마법사: 기본 공격은 약하고 스킬로 싸움 (MP 높음)
		attack = 3;  defense = 1;  basehp = 90;   baseMp = 30;
		growAtk = 2; growDef = 1;  growHP = 8;    growMp = 5;
		break;
	}
	jobName = JOB_NAME[job];
	level = 1;
	exp = 0;
	hp = GetMaxHp();
	mp = GetMaxMp();
}

void Player::ShowInfo() const {
	UI::Log("");
	UI::Log(":: " + name + " [" + jobName + "] ::");
	UI::Log(" > 레벨 : " + to_string(level) + "  (다음 LV까지 " + to_string(GetNeedExp() - exp) + " 필요)");
	UI::Log(" > HP : " + to_string(hp) + " / " + to_string(GetMaxHp()));
	UI::Log(" > MP : " + to_string(mp) + " / " + to_string(GetMaxMp()));
	UI::Log(" > 공격력 : " + to_string(GetAttack()) + "  방어력 : " + to_string(GetDefense()));
	UI::Log(" > 소지금 : " + to_string(gold) + "G");

	string skills;
	for (int id : GetSkills()) skills += (skills.empty() ? "" : ", ") + GetSkillData(id).name;
	UI::Log(" > 스킬 : " + (skills.empty() ? string("없음") : skills));
	if (HasQuest()) {
		const QuestData& q = GetQuestData(questId);
		UI::Log(" > 퀘스트 : " + q.name + " (" + to_string(questKills) + "/" + to_string(q.count) + ")");
	}
	else {
		UI::Log(" > 퀘스트 : 없음");
	}
}

void Player::Create() {
	UI::Log("");
	UI::Log(">> 당신의 이름을 입력하세요.");
	name = UI::AskText(*this, " 이름 > ");

	UI::Log(">> " + name + "님, 당신의 직업을 선택하세요.");
	job = UI::Menu(*this, { "전사", "궁수", "도적", "마법사" });   // 1~4만 돌아옴 → while 불필요
	Init();

	UI::Log(">> [" + jobName + "] " + name + "의 모험이 시작됩니다!");
}

void Player::AddExp(int amount) {
	exp += amount;
	UI::Log(" >> " + to_string(amount) + " 경험치 획득! (" + to_string(exp) + "/" + to_string(GetNeedExp()) + ")");

	while (exp >= GetNeedExp()) {
		exp -= GetNeedExp();

		if (level + 1 > MAX_LEVEL) {
			UI::Log(">> 이미 최대 레벨입니다! :: 레벨 초과 보너스 보상 : 20G ::");
			AddGold(20);
			continue;
		}

		level++;
		hp = GetMaxHp();
		mp = GetMaxMp();
		UI::Log("");
		UI::Log("=======** Level UP!!! **======= LV " + to_string(level - 1) + " -> LV " + to_string(level));
		// 이번 레벨에서 새로 배우는 스킬
		for (int id : GetSkillsFor(job, level)) {
			if (GetSkillData(id).level == level) {
				UI::Log("");
				UI::Log("*** 새 스킬 [" + GetSkillData(id).name + "] 습득! ***");
			}
		}
	}
}

bool Player::SpendGold(int amount)
{
	if (gold < amount) return false;
	gold -= amount;
	return true;
}


// ***플레이어 장비 관리***
Player::EquipStats Player::getEquipStats() const {
	EquipStats s;
	for (int i = 0; i < EQUIPSLOT_END; ++i) {
		s.atk += equip[i].GetAtk();
		s.def+= equip[i].GetDef();
		s.hp+= equip[i].GetPlusHp();
	}
	return s;
}

//아이템과 장비 슬롯 매칭
int Player::FindEquipSlot(ITEMTYPE type) const {
	if (type == NONE_EQUIP) return -1; //장비 아이템이 아님
	if (type != EQUIP_ACCESSORY) return type; //장신구 제외 장비. => ITEMTYPE == EQUIPSLOT! 그대로 리턴

	//장신구 지정. 1번 칸부터 우선 장착
	for (int s = SLOT_ACCESSORY1; s <= SLOT_ACCESSORY3; ++s) { 
		if (equip[s].IsEmpty()) return s;
	}
	return SLOT_ACCESSORY1; //셋 다 차있으면 1번부터 교체
}

//**장비 장착
bool Player::Equip(int invenIdx) { 
	if (inven.IsEmptyAt(invenIdx)) return false;
	ITEM item = inven.Get(invenIdx);

	int slot = FindEquipSlot(item.GetEquipType());
	if (slot == -1) {
		UI::Log(">> [" + string(item.GetName()) + "]은/는 장착할 수 없는 아이템입니다.");
		return false;
	}

	ITEM old = equip[slot];
	equip[slot] = item;

	//장착할 칸을 봐서 빈자리면 빈칸에 장착, 인벤토리에서 뺌. 
	//이미 장착되어 있는 장비가 있으면 장착되어 있던 장비는 인벤토리로 넣어줌
	if (old.IsEmpty()) {
		inven.Remove(invenIdx);
	}
	else {
		inven.SetSlot(invenIdx, old.ItemID);
		UI::Log(">> [" + string(old.GetName()) + "]을/를 벗었습니다."); //기존 장비 벗기
	}

	UI::Log(">> [" + string(item.GetName()) + "]을/를 장착했습니다!"); //장비 장착하기

	if (hp > GetMaxHp()) hp = GetMaxHp();       // 최대 HP가 줄어드는 교체 (반지 → 왕관 등)
	return true;
}

//**장비 해제
bool Player::Unequip(int slot) {
	if (slot < 0 || slot >= EQUIPSLOT_END || equip[slot].IsEmpty()) return false; //해제 불가. (범위 외 or 빈 슬롯)
	if (inven.IsFull()) { //해제 시 인벤토리가 가득 찬 경우
		UI::Log(">> 가방이 가득 차서 장비를 해제할 수 없습니다!");
		return false;
	}


	//해제가 가능한 경우
	ITEM item = equip[slot];
	inven.Add(item.ItemID);
	equip[slot].ItemID = noItem; //뺀 칸은 빈칸으로
	UI::Log(">> [" + string(item.GetName()) + "]을/를 해제했습니다.");

	if (hp > GetMaxHp()) hp = GetMaxHp();	// 최대 HP 장비를 벗으면 현재 HP도 맞춰 줄임
	return true;
}

//**마나 관리
bool Player::SpendMp(int amount) {
	if (mp < amount) return false;
	mp -= amount;
	return true;
}

void Player::RestoreMp(int amount) {
	mp += amount;
	if (mp > GetMaxMp()) mp = GetMaxMp();
}

vector<int> Player::GetSkills() const {
	return GetSkillsFor(job, level);
}