#include "Enemy.h"

void Enemy::Init(int input) {
	switch (input) {
	case 1:
		eName = "슬라임";
		attack = 4;
		defense = 1;
		hp = 20;
		break;
	case 2:
		eName = "다람쥐";
		attack = 3;
		defense = 2;
		hp = 15;
		break;
	}
}