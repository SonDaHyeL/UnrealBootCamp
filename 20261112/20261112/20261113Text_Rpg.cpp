#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

int main() {
	int input = 0;
	string player[2]; //이름, 직업
	int state[5]; // 능력치(공격,방어,민첩,HP,퀘스트 소유 여부)

	printf("\n------------------------\n");
	printf("Text_Rpg에 어서오세요!\n");
	printf("------------------------\n");
	printf("\n\n\n\n\n");

	printf(">> 당신의 이름을 입력하세요.\n");
	printf("> ");
	cin >> player[0];

	printf(">> 당신의 직업을 선택하세요.\n");
	printf(" 1.전사\n");
	printf(" 2.궁수\n");
	printf(" 3.도적\n");
	printf(" 4.마법사\n");
	printf("> ");
	scanf_s("%d", &input);
	switch (input)
	{
	case 1:
		player[1] = "전사";
		state[0] = 5;
		state[1] = 3;
		state[2] = 1;
		state[3] = 150;
		break;
	case 2:
		player[1] = "궁수";
		state[0] = 3;
		state[1] = 2;
		state[2] = 4;
		state[3] = 100;
		break;
	case 3:
		player[1] = "도적";
		state[0] = 3;
		state[1] = 1;
		state[2] = 5;
		state[3] = 120;
		break;
	case 4:
		player[1] = "마법사";
		state[0] = 4;
		state[1] = 2;
		state[2] = 3;
		state[3] = 100;
		break;
	default:
		printf(" ! 잘못된 입력입니다. ! \n");
		break;
	}

START:
	printf("\n------------------------\n");
	printf(">> 어떤 행동을 할까요?\n");
	printf(" 1.플레이어 정보 확인\n");
	printf(" 2.여행 시작\n");
	printf(" 3.종료\n");
	printf("> ");
	scanf_s("%d", &input);

	switch (input) {
	case 1:
		printf("\n------------------------\n");
		printf(":: 플레이어 정보 확인 ::\n");
		printf("------------------------\n\n");
		cout << " > 플레이어 이름 : " << player[0] << endl;
		cout << " > 직업 : " << player[1] << endl;
		cout << " > 공격 : " << state[0] << endl;
		cout << " > 방어 : " << state[1] << endl;
		cout << " > 민첩 : " << state[2] << endl;
		cout << " > 최대 HP : " << state[3] << endl;

		printf(">> 여행을 시작해 봅시다! \n");
		printf("> 좋아요! (ENTER를 눌러 돌아가기.) ");
		cin.ignore(10000, '\n');
		cin.get();
		goto START;
		
	case 2:
	
		printf("\n------------------------\n");
		printf(":: 여행을 시작합니다! ::\n");
		printf("------------------------\n");
		printf(" 1.마을로 간다.\n");
		printf(" 2.사냥터로 간다.\n");
		printf(" 3.모험을 종료한다.");
		printf("\n");
		printf("\n");

		while (input != 3) {
			printf(">> 어떻게 할까요?\n");
			printf("> ");
			scanf_s("%d", &input);

			switch (input) {
			case 1:
				goto TOWN;
			case 2:
				goto HUNT;
			case 3:
				END: 
				printf(">> 모험을 종료합니다!\n");
				break;
			default:
				printf(" (!) 잘못된 입력입니다!\n");
				break;
			}
		}
	case 3:
		return 0;
	}
TOWN:
	printf("\n------------------------\n");
	printf("     :: 마을 ::     \n");
	printf("------------------------\n");
	printf("한적하고 평화로운 곳입니다.\n\n");
	printf(" 1.주변을 탐색한다.\n");
	printf(" 2.상점으로 간다.\n");
	printf(" 3.사냥터로 이동한다.\n");
	printf(" 4.모험을 종료한다.");
	printf("\n");
	printf("\n");

	printf(">> 어떻게 할까요?\n");
	printf("> ");
	scanf_s("%d", &input);
	
	switch (input)
	{
	case 1:
		printf("> 마을 촌장이 거리에서 서성이고 있습니다.\n문제라도 있는 걸까요?\n\n");
		printf(" 1.대화를 시도한다.\n");
		printf(" 2.무시한다.\n\n");
		printf(">> 어떻게 할까요?\n");
		printf("> ");
		scanf_s("%d", &input);
		switch (input) {
		case 1:
			printf("> 당신은 촌장에게 말을 걸었습니다...\n\n");
			printf("'거기 모험가 양반, 혹시 괜찮다면 사냥터의 슬라임을 잡아다 주지 않겠나?'\n\n");
			printf(" 1.승낙한다.\n");
			printf(" 2.거절한다.\n\n");
			printf(">> 어떻게 할까요?\n");
			printf("> ");
			scanf_s("%d", &input);

			if (input == 1) {
				printf("\n------------------------\n");
				printf("    :: 퀘스트 승낙! ::    \n");
				printf("------------------------\n");
				printf("사냥터를 어지럽히는 슬라임을 잡자!(0/1)\n\n");
				state[4] = 1;
			}
			else printf("'에잇, 요즘 젊은이들은 통 야박해...'\n");
			goto TOWN;
		case 2:
			printf("> 당신은 촌장을 무시했습니다...\n\n");
			printf("어쩐지 뒤통수가 따가운 기분이 듭니다.\n\n\n");
			goto TOWN;
		default:
			printf("(!) 잘못된 입력입니다! \n\n");
			break;
		}
	case 2:
		printf("\n------------------------\n");
		printf("    :: 미완성 기능입니다. ::    \n");
		printf("------------------------\n");
		goto TOWN;
	case 3:
		goto HUNT;
	default:
		break;
	}


HUNT:
	int enemy[3]; //에너미 종류, 체력, 공격력
	string eName = ""; //에너미 이름
	printf("\n------------------------\n");
	printf("     :: 사냥터 ::     \n");
	printf("------------------------\n");
	printf("사나운 몬스터 주의! 팻말이 붙어있습니다.\n\n");
	printf(" 1.슬라임을 사냥한다.\n");
	printf(" 2.다람쥐를 사냥한다.\n");
	printf(" 3.마을로 돌아간다.\n");
	printf(" 4.모험을 종료한다.");
	printf("\n");
	printf("\n");

	printf(">> 어떻게 할까요?\n");
	printf("> ");
	scanf_s("%d", &input);

	switch (input)
	{
	case 1:
		enemy[0] = 1;
		goto FIGHT;
	case 2:
		enemy[0] = 2;
		goto FIGHT;
	case 3:
		goto TOWN;
	case 4:
		goto START;
	default:
		printf("(!) 잘못된 입력입니다! \n\n");
		break;
	}



FIGHT:
	printf("\n------------------------\n");
	printf("      :: 전투! ::      \n");
	printf("------------------------\n");
	switch (enemy[0]) {
	case 1: //슬라임 스탯 설정
		eName = "슬라임";
		enemy[1] = 20;
		enemy[2] = 3;
		printf("슬라임이 나타났습니다!\n");
		break;
	case 2: //다람쥐 스탯 설정
		eName = "다람쥐";
		enemy[1] = 10;
		enemy[2] = 4;
		printf("다람쥐가 나타났습니다!\n");
		break;
	default:
		printf("아무것도 나타나지 않았습니다.\n");
		printf("> 이런! (ENTER를 눌러 돌아가기.) ");
		cin.ignore(10000, '\n');
		cin.get();
		goto HUNT;
	}

	while (state[3] >=0 && enemy[1] >= 0) {

		cout << player[0] <<"의 HP :" << state[3] << endl;
		cout << eName << "의 HP :" << enemy[1] << endl;
		printf(" 1.공격한다.\n");
		printf(" 2.방어한다.\n");
		printf(" 3.도망간다.\n");
		printf("\n");
		printf("\n");

		printf(">> 어떻게 할까요?\n");
		printf("> ");
		scanf_s("%d", &input);

		switch (input) {
		case 1:
			cout << player[0] << "의 공격! " << eName << "에게 " << state[1] * 3 << "의 데미지를 주었습니다." << endl;
			enemy[1] -= (state[1] * 3);
			cout << eName << "의 공격! " << player[0] << "에게 " << enemy[2] * 3 << "의 데미지를 주었습니다." << endl;
			state[3] -= (enemy[2] * 3);
			break;
		case 2:
			cout << eName << "의 공격! " << endl;
			cout << "하지만 방어하여 " << (enemy[2] * 3) - (state[1] * 2) << "의 데미지만 입었습니다." << endl;
			state[3] -= (enemy[2] * 3) - (state[1] * 2);
			break;
		case 3:
			printf("> 당신은 도망쳤습니다...\n");
			printf("> 이런... (ENTER를 눌러 돌아가기.) ");
			cin.ignore(10000, '\n');
			cin.get();
			goto HUNT;
		}
	}

	printf("\n------------------------\n");
	printf("      :: 전투 종료! ::      \n");
	printf("------------------------\n");

	if (eName == "슬라임" && state[4] == 1) {
		printf("\n------------------------\n");
		printf("    :: 퀘스트 성공! ::    \n");
		printf("------------------------\n");
		printf("사냥터를 어지럽히는 슬라임을 잡자!(1/1)\n\n");
		printf("\n\n >> 퀘스트 완료 보상으로 10골드를 얻었습니다!\n");
		state[4] = 0;
	}

	printf("> 야호! (ENTER를 눌러 돌아가기.)\n");
	cin.ignore(10000, '\n');
	cin.get();
	goto HUNT;
}