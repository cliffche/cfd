#include <vector>
#include <algorithm>
#include <iostream>
#include <string.h>
using namespace std;

void my_print(int val) {
	cout << val << endl;
}

//基础用法
void vet() {
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	vector<int>::iterator itBegin = v.begin();//迭代器的写法
	vector<int>::iterator itEnd = v.end();
	while (itBegin != itEnd) {
		cout << *itBegin << endl;
		itBegin++;
	}
	//for循环遍历
	//...
	//foreach遍历
	for_each(v.begin(), v.end(), my_print);

}

class Person {
public:
	Person(string name, int age) {
		this->name = name;
		this->age = age;
	}
	string name;
	int age;
};

//vector 放对象
void vet1() {
	vector<Person>v;
	Person p1("zs", 10);
	Person p2("zs1", 11);
	Person p3("zs2", 12);
	Person p4("zs3", 13);
	Person p5("zs4", 14);
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);
	for (vector<Person>::iterator it = v.begin(); it != v.end(); it++) {
		//cout << "姓名:" << (*it).name << "年龄:" << (*it).age << endl;
		cout << "姓名:" <<it->name << "年龄:" << (*it).age << endl;
	}
}


void vet2() {
	vector<Person*>v;
	Person p1("zs", 10);
	Person p2("zs1", 11);
	Person p3("zs2", 12);
	Person p4("zs3", 13);
	Person p5("zs4", 14);
	v.push_back(&p1);
	v.push_back(&p2);
	v.push_back(&p3);
	v.push_back(&p4);
	v.push_back(&p5);
	for (vector<Person *>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "姓名:" << (*it)->name << "年龄:" << (*it)->age << endl;
	}
}

//嵌套vector
void vect3() {
	vector<vector<int>>v;
	vector<int>v1;
	vector<int>v2;
	vector<int>v3;
	vector<int>v4;
	for (int i = 0; i < 4; i++) {
		v1.push_back(i + 1);
		v2.push_back(i + 2);
		v3.push_back(i + 3);
		v4.push_back(i + 4);
	}
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	v.push_back(v4);
	for (vector<vector<int>>::iterator it = v.begin(); it != v.end(); it++) {
		//(*it == vector<int>)
		for (vector<int>::iterator ita = (*it).begin(); ita != (*it).end(); ita++) {
			cout << *ita << " ";
		}
		cout << endl;
	}}

//int main() {
//	vect3();
//	return 0;
//}
