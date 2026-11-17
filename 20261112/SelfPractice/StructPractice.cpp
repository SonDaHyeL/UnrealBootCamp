#include <iostream>
using namespace std;

struct Circle {
	int radius;
public:
	Circle() {
		this->radius = 1;
	};
	Circle(int r) {
		this->radius = r;
	};
	double getArea();
	double showRadius();
};

double Circle::getArea() {
	return 3.14 * radius * radius;
}

double Circle::showRadius() {
	return radius;
}


int main() {
	Circle donut;
	double area = donut.getArea();
	cout << ">> Donut's Area : " << area << " (반지름:" << donut.showRadius() << ")" << endl;

	Circle pizza(3);
	area = pizza.getArea();
	cout << ">> Pizza's Area : " << area << " (반지름:" << pizza.showRadius() << ")" << endl;
}