#include <iostream>
#include <random>
#include <algorithm>
#include <string>
#include "Input.h"
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
	void Attack(const Character& attacker, Character& target, int defenseMul = 1) { //attacker -> target 공격. defenseMul은 방어자세(2)
		DamageResult r = CalcDamage(attacker.GetAttack(), target.GetDefense() * defenseMul);
		cout << attacker.GetName() << "의 공격! ";
		if (r.miss) {
			cout << target.GetName() << "이/가 공격을 피했습니다!" << endl;
			return;
		}
		if (r.critical) { cout << "치명타! "; }
		cout << target.GetName() << "에게 " << r.damage << "의 데미지를 주었습니다." << endl;
		target.TakeDamage(r.damage);
	}
}


void Fight(Player& player, Enemy& enemy) {
	int input = 0;
	while (!player.IsDead() && !enemy.IsDead()) {
		system("cls");
		cout << "\n------------------------\n";
		cout << "      !!! 전투 !!!      \n";
		cout << "------------------------\n";
		cout << player.GetName() << "의 HP (잔여/최대) :" << player.GetHp() << "/" << player.GetMaxHp() << endl;
		cout << enemy.GetName() << "의 HP :" << enemy.GetHp() << endl;
		cout << " 1.공격한다.\n";
		cout << " 2.방어한다.\n";
		cout << " 3.도망간다.\n\n\n";

		input = Select();
		switch (input) {
		case 1:
			cout << "------------------------\n";
			Attack(player, enemy);
			if (enemy.IsDead()) {
				cout << enemy.GetName() << "이/가 쓰러졌습니다!\n";
				cout << "> 이겼다!\n ";
				player.AddExp(enemy.GetDropExp());
				GetItem(player, enemy.GetDropItem());
				system("pause"); 
				break;
			}
			Attack(enemy, player);
			system("pause");
			break;
		case 2:
			cout << "------------------------\n";
			cout << player.GetName() << "은/는 방어 자세를 취했습니다! (방어력 2배)" << endl;
			Attack(enemy, player, 2);
			system("pause");
			break;
		case 3:
			cout << "------------------------\n";
			cout << "\n> 당신은 도망쳤습니다...\n";
			cout << "> 이런...\n ";
			system("pause");
			return;
		}
	}

	if (player.IsDead()) {
		cout << "------------------------\n";
		cout << "\n>> 당신은 쓰러졌습니다...\n";
		cout << "> 이런...\n";
		system("pause");
		return;
	}

	system("cls");
	cout << "\n------------------------\n";
	cout << "     :: 전투 종료! ::     \n";
	cout << "------------------------\n";

	if (enemy.GetName() == "슬라임" && enemy.IsDead() && player.HasQuest()) {
		cout << "\n------------------------\n";
		cout << "    :: 퀘스트 성공! ::    \n";
		cout << "------------------------\n";
		cout << "사냥터를 어지럽히는 슬라임을 잡자!(1/1)\n\n";
		cout << "\n\n >> 퀘스트 완료 보상으로 10골드를 얻었습니다!\n";
		player.AddGold(10);
		player.CompleteQuest();
	}
	printf("> 야호!\n");
	system("pause");
	return;
}