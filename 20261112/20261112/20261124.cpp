#include <iostream>

int main(void) {
	int num = 0;
	char name[20] = {};

	std::cout << "이름과 숫자를 차례로 입력 하시오." << std::endl;
	std::cin >> name >> num;
	std::cout << "이름 : " << name << " 숫자 : " << num << std::endl;
}