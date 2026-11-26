#include <random>
#include <algorithm>
#include <string>
#include "UI.h"
#include "Battle.h"

using namespace std;

// ===== 데미지 산정 =====
// 1) 기본 데미지   = 공격력 * ATTACK_SCALE
// 2) 랜덤 편차     = 기본 데미지 * (0.85 ~ 1.15)
// 3) 방어 감소     = 데미지 * 100 / (100 + 방어력 * DEFENSE_WEIGHT)   (방어력이 높을수록 줄지만 0이 되진 않음)
// 4) 치명타/회피   = 확률로 1.5배 / 0
// 5) 최소 데미지 1 보장 (회피 제외)
namespace {
	const int    ATTACK_SCALE    = 3;
	const int    DEFENSE_WEIGHT  = 10;   // 방어력 1당 약 10%p씩 감소 효과
	const double VARIANCE        = 0.15; // ±15%
	const double CRIT_CHANCE     = 0.10; // 10%
	const double CRIT_MULTIPLIER = 1.5;
	const double MISS_CHANCE     = 0.05; // 5%

	mt19937& Rng() {
		static mt19937 rng(random_device{}());
		return rng;
	}

	double RandomRange(double min, double max) {
		uniform_real_distribution<double> dist(min, max);
		return dist(Rng());
	}

	struct DamageResult {
		int  damage = 0;
		bool critical = false;
		bool miss = false;
	};

	DamageResult CalcDamage(int attack, int defense) {
		DamageResult result;

		if (RandomRange(0.0, 1.0) < MISS_CHANCE) {
			result.miss = true;
			return result;
		}

		double damage = attack * ATTACK_SCALE;
		damage *= RandomRange(1.0 - VARIANCE, 1.0 + VARIANCE);
		damage *= 100.0 / (100.0 + defense * DEFENSE_WEIGHT);

		if (RandomRange(0.0, 1.0) < CRIT_CHANCE) {
			result.critical = true;
			damage *= CRIT_MULTIPLIER;
		}

		result.damage = max(1, static_cast<int>(damage + 0.5));
		return result;
	}

	// 공격 결과를 출력.
	void Attack(const Character& attacker, Character& target, int defenseMul = 1) {
		DamageResult r = CalcDamage(attacker.GetAttack(), target.GetDefense() * defenseMul);
		if (r.miss) {
			UI::Log(attacker.GetName() + "의 공격! " + target.GetName() + "이/가 공격을 피했습니다!");
			return;
		}
		UI::Log(attacker.GetName() + "의 공격! " + (r.critical ? "치명타! " : "")
			+ target.GetName() + "에게 " + to_string(r.damage) + "의 데미지를 주었습니다.");
		target.TakeDamage(r.damage);
	}
}


void Fight(Player& player, Enemy& enemy) {
	UI::SetTitle("전 투");
	UI::ClearLog();
	UI::Log(enemy.GetName() + "이/가 나타났습니다!");
	if (!enemy.GetDescription().empty()) UI::Log(enemy.GetDescription());

	while (!player.IsDead() && !enemy.IsDead()) {
		int input = UI::Menu(player, { "공격한다", "방어한다", "아이템 사용","도망간다" }, 1, &enemy);
		UI::Log("");                               // 턴 구분용 빈 줄
		switch (input) {
		case 1:
			Attack(player, enemy);
			if (!enemy.IsDead()) Attack(enemy, player);
			break;
		case 2:
			UI::Log(player.GetName() + "은/는 방어 자세를 취했습니다! (방어력 2배)");
			Attack(enemy, player, 2);
			break;
		case 3: {
			int idx = SelectInvenSlot(player);
			if (idx == -1) break;	//아이템 사용 취소. 턴 소모하지 않음.

			if (player.GetInven().Get(idx).IsEquipment()) {
				UI::Log(">> 전투 중에는 장비를 바꿀 수 없습니다!");
				break; // 취소 처리. 턴 소모하지 않음.
			}

			if (UseItem(player, idx))
				Attack(enemy, player); //적의 반격! 턴 소모됨
			break;
		}
		case 4:
			UI::Log("> 당신은 도망쳤습니다... 이런...");
			return;
		}
	}

	if (player.IsDead()) {
		UI::Log(">> 당신은 쓰러졌습니다...");
		return;
	}

	UI::Log(enemy.GetName() + "이/가 쓰러졌습니다!");
	UI::Log("> 이겼다!");
	player.AddExp(enemy.GetDropExp());

	if (enemy.GetDropGold() > 0) {
		player.AddGold(enemy.GetDropGold());
		UI::Log(">> " + to_string(enemy.GetDropGold()) + "골드를 얻었습니다!");
	}

	GetItem(player, enemy.GetDropItem());                     // 0(없음)이면 GetItem이 알아서 넘어감

	if (RandomRange(0.0, 100.0) < enemy.GetRareChance()) {    // 희귀확률 %
		UI::Log("!! 희귀 아이템 발견 !!");
		GetItem(player, enemy.GetRareItem());
	}

	if (enemy.GetName() == "슬라임" && player.HasQuest()) {
		UI::Log("");
		UI::Log(":: 퀘스트 성공! :: 사냥터를 어지럽히는 슬라임을 잡자! (1/1)");
		UI::Log(">> 퀘스트 완료 보상으로 10골드를 얻었습니다!");
		player.AddGold(10);
		player.CompleteQuest();
	}
	UI::Log("> 야호!");
}