#include <stdio.h>
#include <iostream>
#include <time.h>

void main() {
	srand(unsigned(time(NULL)));
	int iRand = rand() % 100; //0~9까지 중 난수 생성
	printf_s("%d\n", iRand);
}