#include <stdio.h> //표준 입풀력 라이브러리 헤더추가 


int Add(int x) {
	return x + 3;
}

int main() {
	printf("Hello World!\n");

	printf("**************************\n");
	printf("Let's Study C/C++ Programing!!! \n");
	printf("**************************\n\n");

	int x = 3;
	printf("Add : %d", Add(x));

	int radius = 10;
	double pi = 3.141592;
	printf("Radius = %d\t", radius);
	printf("PI = %0.2f\n", pi);
	printf("Circle = %f\n\n", radius * pi);


	int a = 15; //00000000 00000000 00000000 00001111
	int	b = 20; //00000000 00000000 00000000 00010100 
	int c = a & b; //00000000 00000000 00000000 00000100 
	printf("& 연산 결과 : %d\n\n", c);

	a = 3;
	b = 2;
	x = (a != b && (a || b) > 2 ? 1 : 0);
	printf("x = %d\n", x);
	printf("%d\n\n", sizeof x);
	
	char ch1 = 'A';
	printf("%%c : %c\t %%d : %d\n", ch1, ch1);

	int sa = 0;
	while (sa != 3) {
		printf("1 혹은 2를 입력하세요 : ");
		scanf_s("%d", &sa);
		printf("숫자는 %d\n", sa);
		if (sa == 1) printf("******몸통박치기!!!******\n");
		else printf("효과는 미미했다...\n");
	}

	
	

	return 0;	
}