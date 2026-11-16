// 로또 번호 당첨기

#include <iostream>
#include <time.h>

using namespace std;

void Rand(int answer[]);
void Check(int answer[], int input[]);

int main() {
	srand(time(NULL));
	int answer[7];
	int input[6];
	int select = 0;

	while (1) {
		Rand(answer);
		for (int i = 0; i < 7; ++i) {
			cout << answer[i] << " ";
		}

		cout << "\n>> 중복 없이 1~45 중 숫자 6개를 입력하시오. : ";
		for (int i = 0; i < 6; ++i) {
			cin >> input[i];
		}
		
		Check(answer, input);

		
		cout << "\n>> 새로운 게임을 시작할까요?\n1.새 게임\n2.끝내기\n> ";
		cin >> select;

		switch (select)
		{
		case 1:
			break;
		case 2:
			return 0;
		default:
			break;
		}
	}
		

}

void Rand(int answer[]) {
	for (int i = 0;i < 7;++i) {
		answer[i] = rand() % 45 + 1;
		for (int j = 0; j < i; ++j) {
			if (answer[i] == answer[j]) {
				--i;
				break;
			}
		}
	}
}

void Check(int answer[], int input[]) {
	int count = 0;
	bool bonus = false;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			if (answer[i] == input[j]) {
				++count;
			}
		}
		if (input[i] == answer[6])
			bonus = true;
	}

	switch (count)
	{
	case 3:
		cout << "\n>> 축하합니다! 로또 5등에 당첨되셨습니다.";
		break;
	case 4:
		cout << "\n>> 축하합니다! 로또 4등에 당첨되셨습니다!";
		break;
	case 5:
		if (bonus) {
			cout << "\n>> 축하합니다! 로또 2등에 당첨되셨습니다!!";
			break;
		}
		cout << "\n>> 축하합니다! 로또 3등에 당첨되셨습니다!";
		break;
	case 6:
		cout << "\n>> 축하합니다! 로또 1등에 당첨되셨습니다!!!";
		break;
	default:
		cout << "\n>> 낙첨입니다...";
		break;
	}

	cout << "\n정답 번호는 [ ";
		for (int i = 0; i < 6; ++i) {
			cout << answer[i] << " ";
		}
		cout << "], 보너스 번호는 " << answer[6] << "입니다.";
}