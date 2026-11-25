#include "Player.h"
#include "UI.h"


using namespace std;

void Player::Init() {
	switch (job) {
	case 1: //전사
		jobName = "전사";
		attack = 3;		defense = 3;	basehp = 150;
		growAtk = 1;	growDef = 2;	growHP = 15;
		break;
	case 2: //궁수
		jobName = "궁수";
		attack = 3;		defense = 2;	basehp = 100;
		growAtk = 2;	growDef = 2;	growHP = 10;
		break;
	case 3: //도적
		jobName = "도적";
		attack = 3;		defense = 1;	basehp = 120;
		growAtk = 3;	growDef = 1;	growHP = 10;
		break;
	case 4: //마법사
		jobName = "마법사";
		attack = 4;		defense = 2;	basehp = 100;
		growAtk = 3;	growDef = 1;	growHP = 10;
		break;
	}
	level = 1;
	exp = 0;
	hp = GetMaxHp();
}

void Player::ShowInfo() const {
	UI::Log("");
	UI::Log(":: " + name + " [" + jobName + "] ::");
	UI::Log(" > 레벨 : " + to_string(level) + "  (다음 LV까지 " + to_string(GetNeedExp() - exp) + " 필요)");
	UI::Log(" > HP : " + to_string(hp) + " / " + to_string(GetMaxHp()));
	UI::Log(" > 공격력 : " + to_string(GetAttack()) + "  방어력 : " + to_string(GetDefense()));
	UI::Log(" > 소지금 : " + to_string(gold) + "G");
	UI::Log(" > 퀘스트 : " + string(quest ? "진행 중" : "없음"));
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
		UI::Log("=======★ Level UP!!! ★======= LV " + to_string(level - 1) + " -> LV " + to_string(level));
	}
}

bool Player::SpendGold(int amount)
{
	if (gold < amount) return false;
	gold -= amount;
	return true;
}