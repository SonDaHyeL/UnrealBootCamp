#include <stdio.h>

int main() {
	int level = 0;
	int select = 0;
	while (select != 8) {
		printf("-------------------------\n");
		printf("> 원하는 작업을 선택하세요.\n");
		printf("(1) 오른쪽 삼각형\n");
		printf("(2) 오른쪽 거꾸로 삼각형\n");
		printf("(3) 왼쪽 삼각형\n");
		printf("(4) 왼쪽 거꾸로 삼각형\n");
		printf("(5) 좌측 피라미드 \n");
		printf("(6) 우측 피라미드 \n");
		printf("(7) 마름모 \n");
		printf("(8) 종료 \n");
		printf(">> 작업 선택 : ");
		scanf_s("%d", &select);
		switch (select)
		{
		case 1:
			printf(">> 오른쪽 삼각형! \n> 몇 층으로 만들까요? (0보다 큰 정수 입력) :");
			scanf_s("%d", &level);
			while (level <= 0) {
				printf("0보다 큰 수를 입력해주세요! :");
				scanf_s("%d", &level);
			}
			for (int i = 1;i <= level;i++) {
				for (int j = 0;j < i;j++) {
					printf("*");
				}
				printf("\n");
			}
			printf(">> 오른쪽 삼각형 만들기 종료!\n");
			break;

		case 2:
			printf(">> 오른쪽 거꾸로 삼각형! \n> 몇 층으로 만들까요? (0보다 큰 정수 입력) :");
			scanf_s("%d", &level);
			while (level <= 0) {
				printf("0보다 큰 수를 입력해주세요! :");
				scanf_s("%d", &level);
			}
			for (int i = 0;i < level;i++) {
				for (int j = 0;j <= i;j++) {
					printf(" ");
				}
				for (int j = level;j > i;j--) {
					printf("*");
				}
				printf("\n");
			}
			printf(">> 오른쪽 거꾸로 삼각형 만들기 종료!\n");
			break;

		case 3:
			printf(">> 왼쪽 삼각형! \n> 몇 층으로 만들까요? (0보다 큰 정수 입력) :");
			scanf_s("%d", &level);
			while (level <= 0) {
				printf("0보다 큰 수를 입력해주세요! :");
				scanf_s("%d", &level);
			}
			for (int i = 0;i < level;i++) {
				for (int j = level;j > i;j--) {
					printf(" ");
				}
				for (int j = 0;j <= i;j++) {
					printf("*");
				}
				printf("\n");
			}
			printf(">> 왼쪽 삼각형 만들기 종료!\n");
			break;

		case 4:
			printf(">> 왼쪽 거꾸로 삼각형! \n> 몇 층으로 만들까요? (0보다 큰 정수 입력) :");
			scanf_s("%d", &level);
			while (level <= 0) {
				printf("0보다 큰 수를 입력해주세요! :");
				scanf_s("%d", &level);
			}
			for (int i = 1;i <= level;i++) {
				for (int j = level;j >= i;j--) {
					printf("*");
				}
				printf("\n");
			}
			printf(">> 왼쪽 거꾸로 삼각형 만들기 종료!\n");
			break;

		case 5:
			printf(">> 좌측 피라미드! \n> 몇 층으로 만들까요? (0보다 큰 정수 입력) :");
			scanf_s("%d", &level);
			while (level <= 0) {
				printf("0보다 큰 수를 입력해주세요! :");
				scanf_s("%d", &level);
			}
			

			for (int i = 0; i <= level;i++) {
				for (int j = 0; j <= i;j++) {
					printf("*");
				}
				printf("\n");
			}
			for (int i = 0; i <= level;i++) {
				for (int j = level; j >= i;j--) {
					printf("*");
				}
				printf("\n");
			}

			printf(">> 좌측 피라미드 만들기 종료!\n");
			break;

		case 6:
			printf(">> 우측 피라미드! \n> 몇 층으로 만들까요? (0보다 큰 정수 입력) :");
			scanf_s("%d", &level);
			while (level <= 0) {
				printf("0보다 큰 수를 입력해주세요! :");
				scanf_s("%d", &level);
			}
			
			for (int i = 0;i < level;i++) {
				for (int j = level;j > i;j--) {
					printf(" ");
				}
				for (int j = 0;j <= i;j++) {
					printf("*");
				}
				printf("\n");
			}

			for (int i = 0;i < level;i++) {
				for (int j = 0;j <= i;j++) {
					printf(" ");
				}
				for (int j = level;j > i;j--) {
					printf("*");
				}
				printf("\n");
			}
		
			printf(">> 우측 피라미드 만들기 종료!\n");
			break;

		case 7:
			printf(">> 마름모! \n> 몇 층으로 만들까요? (0보다 큰 정수 입력) :");
			scanf_s("%d", &level);
			while (level <= 0) {
				printf("0보다 큰 수를 입력해주세요! :");
				scanf_s("%d", &level);
			}

			for (int i = 0; i < level; i++) {
					for (int j = level;j > i;j--) {
						printf(" ");
					}
					for (int j = 0;j <= i*2;j++) {
						printf("*");
					}
					printf("\n");
			}

			for (int i = 0; i < level; i++) {
				for (int j = 0;j <= i;j++) {
					printf(" ");
				}
				for (int j = level*2-1;j > i*2;j--) {
					printf("*");
				}
				printf("\n");
			}

			break;
		case 8:
			printf("삼각형 만들기를 종료합니다!\n");
			break;
		default:
			printf("잘못된 입력입니다.\n");
			break;
		}
	}
}