#include "Enemy.h"

Enemy::Enemy(int type) {
	switch (type) {
	case 1:
		name = "슬라임";
		attack = 4;
		defense = 1;
		maxHp = 20;
		dropItem = 1;
		dropExp = 4;
		break;
	case 2:
		name = "다람쥐";
		attack = 3;
		defense = 2;
		maxHp = 15;
		dropItem = 2;
		dropExp = 2;
		break;
	}
	hp = maxHp;
}