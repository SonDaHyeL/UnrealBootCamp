//벡터 실습 예제

#include <iostream>
#include <vector>
using namespace std;

template <typename T>//벡터 출력 용 함수
void print(const vector<T>& v) {
	for (const auto& x : v) {
		cout << x << " ";
	}
	cout << endl;
} 

void part1(); //PART 1 : 기본 생성 및 원소 접근
void part2(); //PART 2 : 원소 추가 및 삭제
void part3(); //PART 3 : 반복자와 순회
void part4(); //PART 4 : 크기(size)와 메모리(capacity) 관리
void part5(); //PART 5 : 


int main() {
	//part1();
	//part2();
	part3();


	return 0;
}


void part4() { 
	//PART 4 : 크기(size)와 메모리(capacity) 관리
	cout << "===== 4. 크기(size)와 메모리(capacity) =====" << endl;
}

void part3() {
	//PART 3 : 반복자와 순회
	cout << "===== 3. 반복자와 순회 =====" << endl;
	//반복자(iterator)는 벡터의 원소를 가리키는 역할을 함.

	vector<int> v = { 10, 20, 30, 40, 50 };

	//1.begin()과 end() => 정방향 출력
	//begin() : 첫 번째 원소 '위치'
	//end() : 마지막 원소 '다음 위치'
	cout << ">> 정방향 출력 : ";
	for (auto iter = v.begin(); iter != v.end(); iter++) {
		cout << *iter << " ";
	}
	cout << endl;
	//2.rbegin()과 rend() => 역방향 출력
	//rbegin() : 역방향 첫 번째 원소 '위치'
	//rend() : 역방향 마지막 원소 '다음 위치'
	cout << "<< 역방향 출력 : ";
	for (auto iter = v.begin(); iter != v.end(); iter++) {
		cout << *iter << " ";
	}
}

void part2() {
	// PART 2 : 원소 추가 및 삭제
	cout << "===== 2. 원소 추가 및 삭제 =====" << endl;
	vector<int> v = { 10, 20, 30 };
	cout << ">> 원본 벡터 : ";
	print(v);
	//1. push_back(n) : 벡터 마지막 원소 뒤에 원소(n) 추가.
	v.push_back(40);
	v.push_back(50);
	cout << "> push_back*2 (40, 50) : ";
	print(v);
	
	//2.pop_back() : 마지막 원소를 제거
	v.pop_back();
	cout << "> pop_back : ";
	print(v);

	//3.insert(a, b) : a 위치에 b 값의 원소 삽입
	v.insert(v.begin() + 1, 100);
	cout << "> insert(v.begin() + 1, 100) : ";
	print(v); // 10 100 20 30 40

	//4.erase(a) : a 위치의 원소 삭제
	v.erase(v.begin() + 2);
	cout << "> erase(v.begin() + 2) : ";
	print(v); // 10 100 30 40

	//5.assign(n,a) : a의 값으로 n개의 원소 할당 (기존 값은 전부 지움, capacity는 그대로.)
	v.assign(3, 7);
	cout << "> assign(3, 7) : ";
	print(v); //즉, 결과 값은 7 7 7로 size는 3이지만 capacity는 4칸 값이 그대로 있는 셈. 

	//6.clear : 내부 원소 값 삭제. (기존 값은 전부 지움, capacity는 그대로.)
	v.clear();
	cout << "> clear() : ";
	print(v);
	cout << "= size : " << v.size() << endl;
	cout << "= empty : " << v.empty() << endl;
}



void part1() {
	// PART 1 : 기본 생성 및 원소 접근
	//vector 생성
	vector<int> v = { 10, 20, 30, 40, 50 };
	cout << "===== 1. 기본 접근 =====" << endl;
	// 1. at(index) => index 위치의 원소에 접근 (범위검사), 더 안전한 편
	cout << "at(2) : " << v.at(2) << endl;

	// 2. operator[] => 인덱스로 원소 접근
	cout << "[2] : " << v[2] << endl;

	// 3. front() => 첫 번째 원서 반환
	cout << "front() : " << v.front() << endl;

	// 4. back() => 마지막 원소 반환
	cout << "back() : " << v.back() << endl;

	// 5. size() => 원소의 갯수 반환
	cout << "size() : " << v.size() << endl;

	// 6. empty() => 벡터가 비어있으면(size == 0) true(1)를 반환
	cout << "empty() : " << v.empty() << endl << endl;

	cout << "===== 1. 예제 풀이 =====" << endl;
	vector<int> v2 = { 5, 10, 15, 20 };
	cout << "v2의 첫 번째 원소 : " << v2.front() << endl;
	cout << "v2의 마지막 번째 원소 : " << v2.back() << endl;
	cout << ":: at(1) | [1] 출력 비교 ::" << endl;
	cout <<	">>> 'at(1)': " << v2.at(1) << "  '[1]' : " << v2[1] << endl;
	//출력 값은 동일. 하지만 at은 접근하기 전에 인덱스가 유효한 범위 내에 있는지 먼저 검사함!
	// [n]은 범위 검사 없이 바로 메모리 주소로 접근함. 위험할 수 있음.
	v2.clear(); //벡터 비우기
	cout << "> v2.clear() 이후 empty() : " << v2.empty() << endl;
	cout << "> v2.clear() 이후 size() : " << v2.size() << endl;
}