//拷贝构造函数调用时机
#include <iostream>
using namespace std;

class Person
{
public:
	Person()
	{
		cout << "无参构造函数" << endl;

	}
	Person(int a)
	{
		age = a;
		cout << "有参构造函数" << endl;
	}
	Person(const Person& p)
	{
		age = p.age;
		cout << "拷贝构造函数" << endl;

	}
	~Person()
	{
		cout << "析构函数" << endl;
	}
	int age;
};

void test1()
{
	//括号法
	//Person p1;//默认构造函数的调用
	//Person p2(10);//有参构造函数
	//Person p3(p2);//拷贝构造函数调用
	//调用默认构造函数的时候不要加小括号
	// 
	//cout << "p2的年龄" << p2.age << endl;
	//cout << "p3的年龄" << p3.age << endl;
	//显示法
	Person p1;
	Person p2 = Person(10);
	Person p3 = Person(p2);
	//不要用拷贝构造函数来初始化匿名对象 Person(p3)
	//隐式法
	Person p4 = 10; //=== Person p4 = Person(10)
	Person p5 = p4;
}


int main()
{
	test1();
}
