#include "Input.h"
#include <iostream>
#include <limits>
using namespace std;

int Select() {
	int input = 0;
	cout << "\n어떻게 할까요?\n> ";
	if (!(cin >> input)) { //입력값 오류 방지(숫자 외 입력값 거름)
		cin.clear();                                          // 실패 상태 해제
		cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 남은 입력 버리기 (<limits> 필요)
		return -1;                                            // 각 메뉴의 default로 감
	}
	return input;
}