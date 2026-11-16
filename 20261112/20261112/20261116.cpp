
// 함수와 배열, 포인터1 강의 예제

#include <iostream>
#define DF_KOREA 13
using namespace std;

int main() {
/*
#ifdef DF_KOREA;
	int Ver = DF_KOREA;
#endif

#if DF_KOREA
	printf("한국 %d 버전입니다.\n", Ver);
#else 
	printf("한국 버전이 아닙니다...\n");
#endif

	int IU = 3;
	int* 매니저 = &IU;
	printf_s("아이유 스케쥴: %d\n", IU);
	printf_s("아이유 전화번호: %d\n", &IU);

	printf_s("아이유 스케쥴: %d\n", *매니저);
	printf_s("아이유 전화번호: %d\n", 매니저);

	printf_s("매니저 전화번호: %d\n", &매니저);


	int 수지 = 105;
	매니저 = &수지;
	*매니저 = 123;
	printf_s("아이유 스케쥴: %d\n", IU);
	printf_s("수지 스케쥴: %d\n", 수지);
*/

	char str[17] = "KGA Game Academy";

	for (int i = 0; i < 16; ++i) {
		if (str[i] >= 65 && str[i] < 97 || str[i] == ' ') cout << str[i];
		else cout << " " << &str[i];
	}

	cout << endl;

	for (int i = 16; i >= 0; --i) {
		cout << str[i];
	}

	return 0;
}