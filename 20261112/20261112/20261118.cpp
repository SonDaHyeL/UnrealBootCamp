#include <iostream>
using namespace std;

struct simple {
	int data1;
	int data2;
};

int main() {
	struct simple s = { 1, 2 };

	cout << "address1 : " << (int)&s << endl;
	cout << "address2 : " << (int)&s.data1 << endl;
	cout << "address2 : " << (int)&s.data2 << endl;
}



/*
* //문자열 조작과 함수 포인터(델리게이트, 콜백 함수)
int add(int a, int b) {
	return a + b;
}

int sub(int a, int b) {
	return a - b;
}

int main(void) {
	char str[10] = "asfghdasd";
	cout << "문자열의 크기 : " << sizeof(str) << endl;
	cout << "문자열의 길이 : " << strlen(str) << endl;


	//함수 포인터. => 함수를 매개변수로 받아야 할 때 쓴다...
	// == 콜백 함수(매개변수로 함수 전달), 델리게이트(delegate)라고도 한다. 
	//int Cal(int (*calc)(int, int), int a, int b){
	//	return calc(a,b); 
	//}
	//이런 경우 쯤?
	int result = 0;
	int (*calc)(int, int); //함수의 포인터 선언 (함수 포인터 자료형)
	calc = add; // add 함수의 포인터 얻기
	result = calc(20, 10); // 얻은 포인트로 함수 호출
	cout << "calc = add; : " << result << endl;
	calc = sub; // sub 함수의 포인터 얻기
	result = calc(20, 10); // 얻은 포인트로 함수 호출
	cout << "calc = sub; : " << result << endl;
}
*/


