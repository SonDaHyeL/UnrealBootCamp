#include <stdio.h>

int main() {
	float cm, weight;

	printf("키와 몸무게를 입력하세요. (키와 몸무게는 cm, kg 단위. 띄어쓰기로 구분) : ");
	scanf_s("%f %f", &cm, &weight);
	float m = cm / 100.0f, bmi = (weight / (m * m));
	printf("당신의 BMI는...! %0.2f로 ", bmi);

	if (bmi >= 30.0) printf("고도비만입니다.");
	else if (bmi >= 25.0 && bmi < 30.0) printf("비만입니다.");
	else if (bmi >= 25.0 && bmi < 30.0) printf("과체중입니다.");
	else if (bmi >= 23.0 && bmi < 25.0) printf("표준 체중입니다.");
	else if (bmi < 18.5) printf("체중 미달입니다.");
}