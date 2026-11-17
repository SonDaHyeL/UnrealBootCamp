#include <stdio.h>

using namespace std;
void calc(int* a, int* min, int* max) {

	for (int i = 0; i < 5;i++) {
		if (*max < a[i]) *max = a[i];
		if (*min > a[i]) *min = a[i];
	}
	
}

int main() {
	int max = 0; 
	int min = 0;
	int a[5];
	for (int i = 0; i < 5;i++) {
		printf(">>");
		scanf_s("%d", &a[i]);
	}
	calc(a, &min, &max);
	printf(">> 최댓값은 %d입니다.\n", max);
	printf(">> 최솟값은 %d입니다.\n", min);

	return 0;
}


/*
void swap(int a, int b) {
	int temp = a;
	a = b;
	b = temp;
}

void rswap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

void pswap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main(void) {
	int a[5] = { 0,1,2,3,4 };
	printf_s("%d, %d\n", a[0], a[1]);
	printf_s("%d번지, %d번지\n", &a[0], &a[1]);
	printf_s("배열 이름 : %d", a);


	int val1 = 1;
	int val2 = 2;
	swap(val1, val2);
	printf_s("\nSwap : %d %d", val1, val2);
	rswap(val1, val2);
	printf_s("\nRSwap : %d %d", val1, val2);
	pswap(&val1, &val2);
	printf_s("\nPSwap : %d %d", val1, val2);
}
*/

/*
#include <iostream> //이거 있어야 system어쩌구

const char* PlayerName[] = { "무직", "판사","검사","법사" };
int iJob = 0;// 1.판사 2.검사 3.법사
int iLevel = 1;
int iExp = 100;//경험치
int iHp = 0;
int iMaxHp = 0;
int iAttack = 0;

int MAXEXP()
{
	return iLevel * 100;
}

//전방선언 
void SelectJob();
void Player_Render();
void Fight(int iMonsterIndex);
void Field();


int main(void)
{
	SelectJob();

	int iInput = 0;
	while (true)
	{
		//iostream에 있는 기능, 화면 초기화
		system("cls");
		Player_Render();
		printf_s("1.사냥터 2.게임 종료 : ");
		scanf_s("%d", &iInput);
		if (iInput == 1)
			Field();
		else
			break;
	}

}


void SelectJob()
{
	int iInput = 0;
	printf_s("직업을 선택 하세요(1.판사 2.검사 3.법사) : ");
	scanf_s("%d", &iInput);
	iJob = iInput; //직업 설정
	switch (iJob)
	{
	case 1: //판사
		iMaxHp = iHp = 10;
		iAttack = 4;
		break;
	case 2: //검사
		iMaxHp = iHp = 6;
		iAttack = 6;
		break;
	case 3: //법사
		iMaxHp = iHp = 5;
		iAttack = 7;
		break;
	default:
		break;
	}
}

void Player_Render()
{
	printf_s("---------------------------------------------------\n");
	printf_s("직업 이름 : %s\n", PlayerName[iJob]);
	printf_s("레벨 : %d   체력 : %d/%d   공격력 : %d \n", iLevel, iHp, iMaxHp, iAttack);
	printf_s("경험치: %d / %d\n", iExp, MAXEXP());
	printf_s("---------------------------------------------------\n");
}
void Fight(int iMonsterIndex)
{
	const char* MonsterName[] = { "버그", "토끼", "다람쥐","사슴","소" };
	int iMonsterHp = 0;
	int iMonsterMaxHp = 0;
	int iMonsterAttack = 0;
	//몬스터 정보 설정
	switch (iMonsterIndex)
	{
	case 1: //토끼
	{
		iMonsterMaxHp = iMonsterHp = 3;
		iMonsterAttack = 1;
	}
	case 2: //다람쥐
	{
		iMonsterMaxHp = iMonsterHp = 5;
		iMonsterAttack = 2;
	}
	case 3: //사슴
	{
		iMonsterMaxHp = iMonsterHp = 6;
		iMonsterAttack = 3;
	}
	}

	int iInput = 0;
	while (true)
	{
		system("cls");
		Player_Render();
		printf_s("---------------------------------------------------\n");
		printf_s("몬스터 이름 : %s\n", MonsterName[iMonsterIndex]);
		printf_s("체력 : %d/%d   공격력 : %d \n", iMonsterHp, iMonsterMaxHp, iMonsterAttack);
		printf_s("---------------------------------------------------\n");
		printf_s("1.공격 2.체력회복 3.도망\n");
		scanf_s("%d", &iInput);
		switch (iInput)
		{
		case 1:
		{
			iMonsterHp -= iAttack;
			printf_s("플레이어가 %s에게 %d의 데미지를 입혔습니다.\n",
				MonsterName[iMonsterIndex],
				iAttack);
			iHp -= iMonsterAttack;
			printf_s("%s가 플레이어에게 %d의 데미지를 입혔습니다.\n",
				MonsterName[iMonsterIndex],
				iMonsterAttack);
		}
		break;
		}
		if (iHp <= 0)
		{
			printf_s("플레이어가 사망했습니다.");
			iHp = iMaxHp;
			system("pause"); //유저가 키를 누를때까지 멈춤
			break;
		}
		//몬스터 죽음.
		if (iMonsterHp <= 0)
		{
			printf_s("%s가 죽었습니다!.", MonsterName[iMonsterIndex]);

			iExp += 50 * iMonsterIndex;
			if (iExp >= MAXEXP())
			{
				iExp = 0;
				++iLevel;

				iAttack += 3;
				iMaxHp += 10;
				iHp = iMaxHp;
				printf_s("레벨이 올랐습니다!!!!!!&&&&");
			}
			system("pause");//유저가 키를 누를때까지 멈춤
			break;
		}
		system("pause");//유저가 키를 누를때까지 멈춤
	}
}
void Field()
{
	int iInput = 0;
	while (true)
	{
		system("cls");
		Player_Render();
		printf_s("1.토끼 2.다람쥐 3.사슴 4.도망");
		scanf_s("%d", &iInput);
		if (iInput == 4)
			break;
		if (iInput <= 3)
			Fight(iInput);
	}
}

*/