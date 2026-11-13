#include <stdio.h>

int main() {
	double price = 24;
	printf("1627년 맨하튼 섬 가격 : %.2f달러\n", price);
	for (int year = 1627; year <= 2010; year++) 
	{
		price *= 1.08;
	}
	printf("2010년 맨하튼 섬 가격 : %.2f달러\n", price);
}
