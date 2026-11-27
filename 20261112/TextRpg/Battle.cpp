#include <random>
#include <algorithm>
#include <string>
#include "UI.h"
#include "Sound.h"
#include "Battle.h"
#include "Quest.h"
#include "Skill.h"

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

	// power: 데미지 배율 (1.0 = 기본) / forceCrit: 반드시 치명타 (회피도 안 됨)
	DamageResult CalcDamage(int attack, int defense, double power = 1.0, bool forceCrit = false) {
		DamageResult result;

		if (!forceCrit && RandomRange(0.0, 1.0) < MISS_CHANCE) {
			result.miss = true;
			return result;
		}

		double damage = attack * ATTACK_SCALE * power;
		damage *= RandomRange(1.0 - VARIANCE, 1.0 + VARIANCE);
		damage *= 100.0 / (100.0 + defense * DEFENSE_WEIGHT);

		if (forceCrit || RandomRange(0.0, 1.0) < CRIT_CHANCE) {
			result.critical = true;
			damage *= CRIT_MULTIPLIER;
		}

		result.damage = max(1, static_cast<int>(damage + 0.5));
		return result;
	}

	// attacker → target 공격. 맞는 쪽의 방어 상태(GetGuard)는 자동으로 반영
	// power / pierce / forceCrit: 스킬용 (데미지 배율 / 방어 무시 / 확정 치명타)
	bool Attack(const Character& attacker, Character& target,
		double power = 1.0, bool pierce = false, bool forceCrit = false) {
		int defense = pierce ? 0 : (int)(target.GetDefense() * target.GetGuard());
		DamageResult r = CalcDamage(attacker.GetAttack(), defense, power, forceCrit);
		if (r.miss) {
			UI::Log(attacker.GetName() + "의 공격! " + target.GetName() + "이/가 공격을 피했습니다!");
			return false;
		}
		UI::Log(attacker.GetName() + "의 공격! " + (r.critical ? "치명타! " : "")
			+ target.GetName() + "에게 " + to_string(r.damage) + "의 데미지를 주었습니다.");
		target.TakeDamage(r.damage);
		return true;
	}

	// 스킬 효과 설명 한 줄 (스킬 목록에서 커서를 올렸을 때)
	string SkillEffectText(const SkillData& s) {
		string hits = (s.hits > 1) ? " x " + to_string(s.hits) + "회" : "";
		switch (s.effect) {
		case SkillEffect::ATTACK:   return "데미지 " + to_string(s.power) + "%" + hits;
		case SkillEffect::PIERCE:   return "방어 무시, 데미지 " + to_string(s.power) + "%" + hits;
		case SkillEffect::CRITICAL: return "확정 치명타, 데미지 " + to_string(s.power) + "%" + hits;
		case SkillEffect::GUARD:    return "이번 턴 방어력 " + to_string(s.power) + "%";
		case SkillEffect::HEAL:     return "최대 HP의 " + to_string(s.power) + "% 회복";
		}
		return "";
	}
	string StatusEffectText(const SkillData& s) {
		if (s.status == Status::NONE) return "";
		string name = (s.status == Status::POISON) ? "독" : "기절";
		return "추가 효과: " + name + " (확률 " + to_string(s.statusChance) + "%, " + to_string(s.statusTurns) + "턴)";
	}

	// 확률에 당첨되면 target에게 스킬의 상태이상을 건다
	void ApplyStatus(Character& target, const SkillData& s) {
		if (s.status == Status::NONE || target.IsDead()) return;
		if (RandomRange(0.0, 100.0) >= s.statusChance) return;

		if (s.status == Status::POISON) {
			target.AddPoison(s.statusTurns);
			UI::Log(target.GetName() + "은/는 독에 걸렸습니다! (" + to_string(s.statusTurns) + "턴)");
		}
		else {
			bool wasStunned = target.IsStunned();
			target.AddStun(s.statusTurns);
			if (!wasStunned)
				UI::Log(target.GetName() + "은/는 기절했습니다! (" + to_string(s.statusTurns) + "턴 동안 행동 불가)");
		}
	}

	// 스킬 효과 처리 (플레이어·몬스터 공용). user가 target에게 스킬 s를 쓴다
	void ApplySkill(Character& user, Character& target, const SkillData& s) {
		UI::Log(user.GetName() + "의 [" + s.name + "]!");
		switch (s.effect) {
		case SkillEffect::ATTACK:
		case SkillEffect::PIERCE:
		case SkillEffect::CRITICAL: {
			bool anyHit = false;
			for (int i = 0; i < s.hits && !target.IsDead(); ++i) {
				if (Attack(user, target, s.power / 100.0,
					s.effect == SkillEffect::PIERCE, s.effect == SkillEffect::CRITICAL))
					anyHit = true;
			}
			if (anyHit) ApplyStatus(target, s);       // 한 대라도 맞았으면 상태이상 시도
			break;
		}
		case SkillEffect::GUARD:
			user.SetGuard(s.power / 100.0);
			UI::Log(user.GetName() + "은/는 몸을 단단히 굳혔습니다! (방어력 " + to_string(s.power) + "%)");
			break;
		case SkillEffect::HEAL: {
			int amount = user.GetMaxHp() * s.power / 100;
			user.Heal(amount);
			UI::Log(user.GetName() + "의 HP가 " + to_string(amount) + " 회복되었습니다!");
			break;
		}
		}
	}

	// 플레이어: 스킬을 골라 사용. 턴을 썼으면 true (취소·MP 부족이면 false)
	bool UseSkill(Player& player, Enemy& enemy) {
		vector<int> skills = player.GetSkills();
		if (skills.empty()) {
			UI::Log(">> 아직 배운 스킬이 없습니다.");
			return false;
		}

		vector<UI::ListEntry> entries;
		for (int id : skills) {
			const SkillData& s = GetSkillData(id);
			UI::ListEntry e;
			e.label = s.name;
			e.tag = "MP " + to_string(s.mp);
			e.info.push_back("[" + s.name + "] " + SkillEffectText(s));
			e.info.push_back(s.description);
			if (s.status != Status::NONE) e.info.push_back(StatusEffectText(s));
			entries.push_back(e);
		}

		int cursor = 0;
		int pick = UI::SelectList(player, "스 킬", entries, "사용할 스킬을 고르세요. (뒤로가기: 취소)", cursor);
		if (pick == -1) return false;

		const SkillData& s = GetSkillData(skills[pick]);
		if (!player.SpendMp(s.mp)) {
			UI::Log(">> MP가 부족합니다! ([" + s.name + "] MP " + to_string(s.mp) + " 필요)");
			return false;
		}
		ApplySkill(player, enemy, s);
		return true;
	}

	// 플레이어: 전투 중 아이템 사용. 턴을 썼으면 true
	bool UseItemInBattle(Player& player) {
		int idx = SelectInvenSlot(player);
		if (idx == -1) return false;                       // 취소
		if (player.GetInven().Get(idx).IsEquipment()) {
			UI::Log(">> 전투 중에는 장비를 바꿀 수 없습니다!");
			return false;
		}
		return UseItem(player, idx);                       // 쓸 필요 없으면 false
	}

	// 턴 시작 처리 (플레이어·몬스터 공용): 방어 초기화 → 독 데미지 → 기절 확인
	// 이번 턴에 행동할 수 있으면 true
	bool StartTurn(Character& c) {
		c.ResetGuard();                                    // 지난 턴의 방어 효과 끝

		int poison = c.TickPoison();
		if (poison > 0) {
			UI::Log(c.GetName() + "은/는 독 때문에 " + to_string(poison) + "의 데미지를 입었습니다!");
			if (c.IsDead()) return false;
		}
		if (c.TickStun()) {
			UI::Log(c.GetName() + "은/는 기절해서 움직일 수 없습니다!");
			return false;
		}
		return true;
	}

	// 플레이어 행동 고르기. 턴을 쓰는 행동을 할 때까지 반복. 도망쳤으면 true
	bool PlayerTurn(Player& player, Enemy& enemy) {
		while (true) {
			int input = UI::Menu(player, { "공격한다", "스킬 사용", "방어한다", "아이템 사용", "도망간다" }, 1, &enemy);
			switch (input) {
			case 1: Attack(player, enemy); return false;
			case 2: if (UseSkill(player, enemy)) return false; break;      // 취소·MP 부족이면 다시 고르기
			case 3:
				player.SetGuard(2.0);
				UI::Log(player.GetName() + "은/는 방어 자세를 취했습니다! (방어력 2배)");
				return false;
			case 4: if (UseItemInBattle(player)) return false; break;      // 취소면 다시 고르기
			case 5:
				UI::Log("> 당신은 도망쳤습니다... 이런...");
				return true;
			}
		}
	}

	// 몬스터 행동: 스킬확률에 당첨되면 스킬, 아니면 일반 공격
	void EnemyAct(Enemy& enemy, Player& player) {
		const SkillData& s = GetSkillData(enemy.GetSkill());
		if (s.exists && RandomRange(0.0, 100.0) < enemy.GetSkillChance())
			ApplySkill(enemy, player, s);
		else
			Attack(enemy, player);
	}
}


void Fight(Player& player, Enemy& enemy) {
	Sound::PlayBgm("Battle.mp3");
	UI::SetTitle("전 투");
	UI::ClearLog();
	UI::Log(enemy.GetName() + "이/가 나타났습니다!");
	if (!enemy.GetDescription().empty()) UI::Log(enemy.GetDescription());

	while (!player.IsDead() && !enemy.IsDead()) {
		// ── 내 턴 ──
		UI::Log("");
		if (StartTurn(player) && PlayerTurn(player, enemy)) {   // 행동할 수 있으면 행동, 도망치면 끝
			player.ClearStatus();
			return;
		}
		if (player.IsDead() || enemy.IsDead()) break;

		// ── 적의 턴 ──
		if (StartTurn(enemy)) EnemyAct(enemy, player);
	}
	player.ClearStatus();                                      // 전투가 끝나면 상태이상도 끝

	if (player.IsDead()) {
		UI::Log(">> 당신은 쓰러졌습니다...");
		return;
	}

	UI::Log(enemy.GetName() + "이/가 쓰러졌습니다!");
	UI::Log("> 이겼다!");

	// 승리 보상: 최대 MP의 20% 회복 (레벨업으로 MP가 가득 차면 그쪽이 우선)
	int mpGain = player.GetMaxMp() * 20 / 100;
	if (mpGain > 0 && player.GetMp() < player.GetMaxMp()) {
		player.RestoreMp(mpGain);
		UI::Log(">> 전투의 여운으로 MP가 " + to_string(mpGain) + " 회복되었습니다.");
	}

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

	// 퀘스트 대상이면 처치 수 +1 (목표 수를 넘어서는 세지 않음)
	if (player.HasQuest()) {
		const QuestData& q = GetQuestData(player.GetQuestId());
		if (enemy.GetId() == q.target && player.GetQuestKills() < q.count) {
			player.AddQuestKill();
			UI::Log("★ [" + q.name + "] " + enemy.GetName() + " 처치 ("
				+ to_string(player.GetQuestKills()) + "/" + to_string(q.count) + ")");
			if (player.GetQuestKills() == q.count)
				UI::Log("★ 퀘스트 목표 달성! 마을 촌장에게 보고하자.");
		}
	}
	UI::Log("> 야호!");

}
