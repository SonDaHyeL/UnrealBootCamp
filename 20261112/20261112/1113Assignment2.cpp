#include <stdio.h>
#include <iostream>
#include <time.h>

void main() {
	srand(unsigned(time(NULL)));
	int qusetion[3];
	int answer[3];
	bool success = false;
	int ball = 0;
	int strike = 0;

	for (int i = 0; i < 3; i++) {
		qusetion[i] = rand() % 9 + 1;

		for (int j = 0; j < i; j++) {
			if (qusetion[i] == qusetion[j]) {
				i--;
				break;
			}
		}
	}

	printf("************야구 게임 시작!************\n");

	while (success < 1) {
		int ball = 0;
		int strike = 0;
		printf(">> 예상되는 숫자 3개를 입력하세요. (1~9, 띄어쓰기로 구분) : ");
		scanf_s("%d %d %d", &answer[0], &answer[1], &answer[2]);

		// 숫자 범위 검사
		bool valid = true;

		for (int i = 0; i < 3; i++) {
			if (answer[i] < 1 || answer[i] > 9) {
				valid = false;
			}

			for (int j = 0; j < i; j++) {
				if (answer[i] == answer[j]) {
					valid = false;
				}
			}
		}

		if (!valid) {
			printf("1~9 사이의 중복되지 않는 숫자 3개를 입력하세요.\n");
			continue;
		}

		//스트라이크&볼 판정
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (answer[i] == qusetion[j]) {
					if (i == j)	strike++;
					else ball++;
				}
			}
		}
		printf("> %d 스트라이크, %d 볼입니다.\n", strike, ball);
		if (strike >= 3) {
			printf(">> 정답을 맞췄습니다!\n");
			success = 1;
		}
	}
	printf("> 게임이 종료되었습니다.");
}