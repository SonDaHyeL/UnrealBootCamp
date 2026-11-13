#include <stdio.h>

int main() {
	/*int skill = 0;
	printf("할 수 있는 행동 : 1.기본 공격\t2.파이어볼\t3.힐\t4.방어\n");
	printf(">> 커맨드를 입력하세요. : ");
	scanf_s("%d", &skill);

	switch (skill)
	{
	case 1: printf(">>> 기본 공격! 적에게 10 데미지를 주었습니다."); break;
	case 2: printf(">>> 파이어볼! 적에게 30 데미지를 주었습니다."); break;
	case 3: printf(">>> 힐! 체력이 20 회복됩니다."); break;
	case 4: printf(">>> 방어! 방어력이 10 증가합니다."); break;
	default:
		printf("잘못된 선택입니다...");
		break;
	}
	*/


	int num = 1, sum = 0;
	while (sum < 100) {
		sum += num;
		printf("Sum of 1~%d is %d\n", num, sum);
		num++;
	}

	//위를 등차수열의 합 공식을 적용하면?
	int first = 0; //첫째항
	int last = 100; //마지막항
	int count = 101; //항의 개수 (0부터 100까지는 총 101개)

	sum = count * (first + last) / 2;
	printf("등차수열 Sum of 1~%d is %d\n", num, sum);

	return 0;
}	