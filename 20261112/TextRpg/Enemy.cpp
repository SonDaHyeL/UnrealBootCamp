#include "Enemy.h"

Enemy::Enemy(int enemyId) {
	id = enemyId;
	name = Data().name;
	hp = GetMaxHp();
}