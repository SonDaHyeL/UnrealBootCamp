#include <iostream>
#include <string>

using namespace std;

struct Player {
	string name = "";
	string jobName = "";
	int job = 0;
	int attack = 0;
	int defense = 0;
	int hp = 0;
	bool quest = false;

	void init() {
		switch (this->job) {
		case 1: //전사
			this->jobName = "전사";
			this->attack = 5;
			this->defense = 3;
			this->hp = 150;
			break;
		case 2: //궁수
			this->jobName = "궁수";
			this->attack = 3;
			this->defense = 2;
			this->hp = 100;
			break;
		case 3: //도적
			this->jobName = "도적";
			this->attack = 3;
			this->defense = 1;
			this->hp = 120;
			break;
		case 4: //마법사
			this->jobName = "마법사";
			this->attack = 4;
			this->defense = 2;
			this->hp = 100;
			break;
		}
	}
	void showInfo() {
		cout << " > 플레이어 이름 : " << name << endl;
		cout << " > 직업 : " << jobName << endl;
		cout << " > 현재 HP : " << hp << endl;
		cout << " > 공격력 : " << attack << endl;
		cout << " > 방어력 : " << defense << endl;
	}
};
struct Enemy {
	string eName = "";
	int attack = 0;
	int defense = 0;
	int hp = 0;

	void init(int input) {
		switch (input) {
		case 1:
			this->eName = "슬라임";
			this->attack = 4;
			this->defense = 1;
			this->hp = 20;
			break;
		case 2:
			this->eName = "다람쥐";
			this->attack = 3;
			this->defense = 2;
			this->hp = 15;
			break;
		}
	}
};

void Playerinit(Player& player);
int Select();
void Town(Player& player);
void Hunting(Player& player);
void Fight(Player& player, Enemy& enemy);



void Playerinit(Player& player) {
	cout << ">> 당신의 이름을 입력하세요.\n";
	cout << "> ";
	cin >> player.name;

	cout << ">> 당신의 직업을 선택하세요.\n";
	cout << " 1.전사\n";
	cout << " 2.궁수\n";
	cout << " 3.도적\n";
	cout << " 4.마법사\n";
	cout << "> ";
	cin >> player.job;
	player.init();
}

int Select() {
	int input = 0;
	cout << "\n어떻게 할까요?\n> ";
	cin >> input;
	return input;
}

void Town(Player& player) {
	int input = 0;
	cout << "\n------------------------\n";
	cout << "     :: 마을 ::     \n";
	cout << "------------------------\n";
	cout << "한적하고 평화로운 곳입니다.\n\n";
	cout << " 1.주변을 탐색한다.\n";
	cout << " 2.상점으로 간다.\n";
	cout << " 3.사냥터로 이동한다.\n";
	cout << " 4.모험을 종료한다.\n\n";
	input = Select();

	switch (input)
	{
	case 1:
		cout << "> 마을 촌장이 거리에서 서성이고 있습니다.\n문제라도 있는 걸까요?\n\n";
		cout << " 1.대화를 시도한다.\n";
		cout << " 2.무시한다.\n\n";
		input = Select();
		switch (input) {
		case 1:
			cout << "> 당신은 촌장에게 말을 걸었습니다...\n\n";
			cout << "'거기 모험가 양반, 혹시 괜찮다면 사냥터의 슬라임을 잡아다 주지 않겠나?'\n\n";
			cout << " 1.승낙한다.\n";
			cout << " 2.거절한다.\n\n";
			input = Select();

			if (input == 1) {
				cout << "\n------------------------\n";
				cout << "    :: 퀘스트 승낙! ::    \n";
				cout << "------------------------\n";
				cout << "사냥터를 어지럽히는 슬라임을 잡자!(0/1)\n\n";
				player.quest = true;
			}
			else cout << "'에잇, 요즘 젊은이들은 통 야박해...'\n";
			Town(player);
			break;
		case 2:
			cout << "> 당신은 촌장을 무시했습니다...\n\n";
			cout << "어쩐지 뒤통수가 따가운 기분이 듭니다.\n\n\n";
			Town(player);
			return;
		default:
			cout << "(!) 잘못된 입력입니다! \n\n";
			break;
		}
	case 2:
		cout << "\n------------------------\n";
		cout << "    :: 오늘은 영업 안함. ::    \n";
		cout << "------------------------\n";
		cout << "\n\n\n > 이런...(ENTER를 눌러 돌아가기.)";
		cin.ignore(10000, '\n');
		cin.get();
		Town(player);
		break;
	case 3:
		Hunting(player);
		break;
	default:
		break;
	}
}
	
void Hunting(Player& player) {
	Enemy enemy;
	int input = 0;
	cout << "\n------------------------\n";
	cout << "     :: 사냥터 ::     \n";
	cout << "------------------------\n";
	cout << "사나운 몬스터 주의! 팻말이 붙어있습니다.\n\n";
	cout << " 1.슬라임을 사냥한다.\n";
	cout << " 2.다람쥐를 사냥한다.\n";
	cout << " 3.마을로 돌아간다.\n";
	cout << " 4.모험을 종료한다.\n\n";

	input = Select();
	switch(input) {
	case 1:
	case 2:
		cout << "\n------------------------\n";
		cout << "      :: 전투! ::      \n";
		cout << "------------------------\n";
		enemy.init(input);
		cout << enemy.eName << "이/가 나타났습니다!\n";
		Fight(player, enemy);
		break;
	case 3:
		Town(player);
		break;
	case 4:
		cout << ">> 모험을 종료합니다!\n";
		return;
		break;
	default:
		cout << "잘못된 입력입니다.\n";
		break;
		
	}
}

void Fight(Player& player, Enemy& enemy) {
	int input = 0;
	while (player.hp > 0 && enemy.hp > 0) {
		cout << player.name << "의 HP :" << player.hp << endl;
		cout << enemy.eName << "의 HP :" << enemy.hp << endl;
		cout << " 1.공격한다.\n";
		cout << " 2.방어한다.\n";
		cout << " 3.도망간다.\n\n\n";

		input = Select();
		switch (input) {
		case 1:
			cout << player.name << "의 공격! " << enemy.eName << "에게 " << (player.attack * 3) - enemy.defense << "의 데미지를 주었습니다." << endl;
			enemy.hp -= ((player.attack * 3) - enemy.defense);
			cout << enemy.eName << "의 공격! " << player.name << "에게 " << enemy.attack * 3 << "의 데미지를 주었습니다." << endl;
			player.hp -= (enemy.attack * 3);
			break;
		case 2:
			cout << enemy.eName << "의 공격! " << endl;
			cout << "하지만 방어하여 " << (enemy.attack * 3) - (player.defense * 2) << "의 데미지만 입었습니다." << endl;
			if (((enemy.attack * 3) - (player.defense * 2)) > 0) {
				player.hp -= (enemy.attack * 3) - (player.defense * 2);
			}
			break;
		case 3:
			cout << "> 당신은 도망쳤습니다...\n";
			cout << "> 이런... (ENTER를 눌러 돌아가기.) ";
			cin.ignore(10000, '\n');
			cin.get();
			Hunting(player);
			return;
		}
	}
	
	if (player.hp <= 0) {
		cout << "\n>> 당신은 쓰러졌습니다...\n";
		return;
	}

	cout << "\n------------------------\n";
	cout << "      :: 전투 종료! ::      \n";
	cout << "------------------------\n";

	if (enemy.eName == "슬라임" && enemy.hp <= 0 && player.quest == true) {
		cout << "\n------------------------\n";
		cout << "    :: 퀘스트 성공! ::    \n";
		cout << "------------------------\n";
		cout << "사냥터를 어지럽히는 슬라임을 잡자!(1/1)\n\n";
		cout << "\n\n >> 퀘스트 완료 보상으로 10골드를 얻었습니다!\n";
		player.quest = false;
	}
	printf("> 야호! (ENTER를 눌러 돌아가기.)\n");
	cin.ignore(10000, '\n');
	cin.get();
	Hunting(player);
}

int main() {
	Player player;
	int input = 0;
	cout << "\n------------------------\n";
	cout << "  Text_Rpg에 어서오세요!\n";
	cout << "------------------------\n";
	cout << "\n\n\n\n\n";

	Playerinit(player);
	
START:
	cout << "\n------------------------\n";
	cout << ">> 어떤 행동을 할까요?\n";
	cout << " 1.플레이어 정보 확인\n";
	cout << " 2.여행 시작\n";
	cout << " 3.종료\n";

	input = Select();

	switch (input)
	{
	case 1:
		player.showInfo();
		printf(">> 여행을 시작해 봅시다! \n");
		printf("> 좋아요! (ENTER를 눌러 돌아가기.) ");
		cin.ignore(10000, '\n');
		cin.get();
		goto START;
	case 2:
		cout << "\n------------------------\n";
		cout << ":: 여행을 시작합니다! ::\n";
		cout << "------------------------\n";
		cout << " 1.마을로 간다.\n";
		cout << " 2.사냥터로 간다.\n";
		cout << " 3.모험을 종료한다.\n\n";

		while (input != 3) {
			input = Select();
			switch (input) {
			case 1:
				Town(player);
				break;
			case 2:
				Hunting(player);
				break;
			case 3:
				END:
				cout << ">> 모험을 종료합니다!\n";
				break;
			default:
				cout << " (!) 잘못된 입력입니다!\n";
				break;
			}
		}
	case 3:
		return 0;
	default:
		break;
	} 

}