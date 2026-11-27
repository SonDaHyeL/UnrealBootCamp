#pragma once

#include <string>
#include <vector>
#include "Character.h"

enum class SkillEffect { ATTACK, PIERCE, CRITICAL, GUARD, HEAL };   // 공격, 방어무시, 치명타, 방어, 회복

// skills.csv 한 줄
struct SkillData {
	bool exists = false;
	std::string name;
	int job = 0;                 // 직업 번호 (JOB_NAME)
	int level = 1;               // 습득 레벨
	int mp = 0;                  // 소모 MP
	SkillEffect effect = SkillEffect::ATTACK;
	int power = 100;             // 배율 %
	int hits = 1;                // 공격 횟수
	Status status = Status::NONE;   // 추가 상태이상
	int statusChance = 0;           // 걸릴 확률 %
	int statusTurns = 0;            // 지속 턴
	std::string description;
};

// ===== 스킬 표 (DataTable.cpp가 skills.csv를 읽어 채운다) =====
const SkillData& GetSkillData(int id);
std::vector<int> GetSkillsFor(int job, int level);   // 그 직업이 그 레벨까지 배운 스킬 ID들
