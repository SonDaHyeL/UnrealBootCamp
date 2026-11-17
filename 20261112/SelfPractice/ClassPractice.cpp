#include <iostream>
using namespace std;

//클래스 구조 기초 학습
class Circle {
private:
	int radius;
public:
	double getArea();	
	double showArea();	//private로 선언하며 맴버 함수 추가
	Circle();
	Circle(int r);
	~Circle();
};

double Circle::getArea() {
	return 3.14 * radius * radius;
}

double Circle::showArea() {
	return radius;
}

Circle::Circle() {
	radius = 3;
}
// Circle::Circle() : Circle(3) <-이렇게 하면 위임 생성하조 아래 매개변수 가진 생성자로 넘겨줌

Circle::Circle(int r) {
	radius = 5;
}

Circle::~Circle() {//소멸자
	cout << radius << "의 반지름을 가진 원이 소멸함!" << endl;
}


class Square {
public:
	int width;
	int height;
	int getArea();
};

int Square::getArea() {
	return width * height;
}

int main() {
	Circle donut;
	double area = donut.getArea();
	cout << ">> Donut's Area : " << area << " (반지름:" << donut.showArea() << ")" << endl;

	Circle pizza(30);
	area = pizza.getArea();
	cout << ">> Pizza's Area : " << area << " (반지름:" << pizza.showArea() << ")" << endl;

	Square pizzaBox;
	pizzaBox.width = 3;
	pizzaBox.height = 15;
	area = pizzaBox.getArea();
	cout << ">> PizzaBox's Area : " << area << endl;
}
