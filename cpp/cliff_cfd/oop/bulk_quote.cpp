#include "bulk_quote.h"
#include "disc_quote.h"
using namespace std;
#include <iostream>
#include <string>

double print_total(const Quote& item, size_t n) {
	double rel = item.net_price(n);
	cout << "print_total_rel=" << rel << endl;
	return rel;
}

double print_total_1(const Quote& item, size_t n) {
	//�ر��麯������
	double rel = item.Quote::net_price(n);
	cout << "print_total_rel=" << rel << endl;
	return rel;
}

int main() {
	Quote item;
	Bulk_quote bulk;
	//Quote* item = bulk;�����д��,�����ڴӻ��ൽ�������ת��
	cout << "&item:" << &item << endl;
	cout << "&bulk:" << &bulk << endl;

	Quote* p = &item;
	p = &bulk;
	Quote& r = bulk;
	cout << "&p   :" << &p << endl;
	cout << "&r   :" << &r << endl;
	string book_name = "math";
	Bulk_quote bulk1(book_name, 12, 1, 23);
	Bulk_quote bulk2(book_name, 11, 12, 3);
	double net_price = bulk1.net_price(2);
	cout << "net_price=" << net_price << endl;
	string child_date = bulk1.date();
	cout << "child_date=" << child_date << endl;
	//����ʵ������ľ�̬��Ա��ָ��ͬһ��ַ
	cout << "address static=" << &bulk1.date << endl;
	cout << "address static=" << &bulk2.date << endl;
	double p_t_r = print_total(bulk1, 3);//69
	double p_t_r1 = print_total(item, 3);//0
	double p_t_r2 = print_total_1(bulk1, 3);//36

	//Disc_quote item2;//����,������಻��ʵ����






	return 0;


}
