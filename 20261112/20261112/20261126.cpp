#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;


class Person {
	char* name;
	char* phone;
	int age;
public:
	Person(const char* _name, const char* _phone, int _age) {
		// _name = "abced"
		name = new char[strlen(_name) + 1]; //name = [][][][][]
		
		strcpy(name, _name);
		//_name = [a] [b] [c] [d] [e]
		//         v   v   v   v   v
		// name = [a] [b] [ ] [ ] [ ]

		phone = new char[strlen(_phone) + 1];
		strcpy(phone, _phone);
		strcpy_s(phone, strlen(_name) + 1, _phone);

		age = _age;
	}

	~Person() {
		DelMemory();
	}

	void DelMemory(){
		delete[]name;
		delete[]phone;
	}
	void ShowData() {
		cout << "name : " << name << endl;
		cout << "phone : " << phone << endl;
		cout << "age : " << age << endl;
	}
};

int main() {
	Person p("adks", "010-2020-3030", 10);
	p.ShowData();
	
}

//class AAA {
//public:
//	AAA() {
//		cout << "AAA() Call" << endl;
//	}
//
//	AAA(int i) {
//		cout << "AAA(int i) Call" << endl;
//	}
//
//	AAA(const AAA& a) {
//		cout << "const AAA& a Call" << endl;
//	}
//};
//
//int main() {
//	AAA obj1;
//	AAA obj2(10);
//	AAA obj3(obj2);
//	return 0;
//}





//class AAA {
//	int m_i, m_j;
//public:
//	AAA(int _i, int _j) {
//		m_i = _i;
//		m_j = _j;
//	}
//
//	void ShowData() {
//		std::cout << m_i << ' ' << m_j << std::endl;
//	}
//};
//
//int main() {
//	AAA a(1, 2);
//	a.ShowData();
//}