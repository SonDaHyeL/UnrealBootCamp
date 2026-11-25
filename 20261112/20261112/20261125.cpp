#include <iostream>
#include <String>
using namespace std;



class Door {
private:
	int state;
	int OPEN = 1;
	int CLOSE = 2;
public:
	void Open();
	void Close();
	void ShowState();
};

void Door::Open() {
	state = OPEN;
	cout << "-- Door OPEN! -- " << endl;
}

void Door::Close() {
	state = CLOSE;
	cout << "-- Door CLOSE! -- " << endl;
}

void Door::ShowState() {
	string show = "";
	if (state == 1) { show = "OPEN"; }
	if (state == 2) { show = "CLOSE"; }
	cout << "Door State : " << show << endl;
}

int main() {
	Door d;

	d.Open();
	d.ShowState();
	d.Close();
	d.ShowState();
}

//struct  Account
//{
//	char accID[20];
//	char secID[20];
//	char name[20];
//	int balance;
//
//	void Deposit(int money) {
//		balance += money;
//	}
//	void Withdraw(int money) {
//		balance -= money;
//	}
//};
//
//int main() {
//	Account yoon = { "1234","2321","yoon",1000 };
//	yoon.Deposit(100);
//	cout << "잔  액 : " << yoon.balance << endl;
//	return 0;
//}

//int& funtion() {
//	int val = 10;
//	cout << &val << endl;
//	return val;
//}
//
//int& funtion2() {
//	int val2 = 20;
//	cout << &val2 << endl;
//	return val2;
//}
//
//
//void main() {
//	int& ref = funtion();
//	int& ref2 = funtion2();
//	cout << ref << endl;
//	cout << ref2 << endl;
//}


//int& increment(int& val) {
//	val++;
//	return val;
//}
//
//int main(void) {
//	int n = 10;
//	int& ref = increment(n);
//	cout << "n: " << n << endl;
//	cout << "&n: " << &n << endl;
//	cout << "ref: " << ref << endl;
//	cout << "&ref: " << &ref << endl;
//}