//�������캯������ʱ��
#include <iostream>
using namespace std;

class Person
{
public:
	Person()
	{
		cout << "�޲ι��캯��" << endl;

	}
	Person(int a)
	{
		age = a;
		cout << "�вι��캯��" << endl;
	}
	Person(const Person& p)
	{
		age = p.age;
		cout << "�������캯��" << endl;

	}
	~Person()
	{
		cout << "��������" << endl;
	}
	int age;
};

void test1()
{
	//���ŷ�
	//Person p1;//Ĭ�Ϲ��캯���ĵ���
	//Person p2(10);//�вι��캯��
	//Person p3(p2);//�������캯������
	//����Ĭ�Ϲ��캯����ʱ��Ҫ��С����
	// 
	//cout << "p2������" << p2.age << endl;
	//cout << "p3������" << p3.age << endl;
	//��ʾ��
	Person p1;
	Person p2 = Person(10);
	Person p3 = Person(p2);
	//��Ҫ�ÿ������캯������ʼ���������� Person(p3)
	//��ʽ��
	Person p4 = 10; //=== Person p4 = Person(10)
	Person p5 = p4;
}


int main()
{
	test1();
}
