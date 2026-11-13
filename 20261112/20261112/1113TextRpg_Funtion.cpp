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
		cout << " > 최대 HP : " << hp << endl;
		cout << " > 공격력 : " << attack << endl;
		cout << " > 방어력 : " << defense << endl;
	}
};


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

void main() {
	Player player;
	int input;
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
	cout << "> ";
	cin >> input;
	switch (input)
	{
	case 1:
		player.showInfo();
		printf(">> 여행을 시작해 봅시다! \n");
		printf("> 좋아요! (ENTER를 눌러 돌아가기.) ");
		cin.ignore(10000, '\n');
		cin.get();
		goto START;
	default:
		break;
	} 
}