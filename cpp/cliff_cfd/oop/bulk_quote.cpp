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
	//回避虚函数机制
	double rel = item.Quote::net_price(n);
	cout << "print_total_rel=" << rel << endl;
	return rel;
}

int main() {
	Quote item;
	Bulk_quote bulk;
	//Quote* item = bulk;错误的写法,不存在从基类到派生类的转换
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
	//所有实例对象的静态成员都指向同一地址
	cout << "address static=" << &bulk1.date << endl;
	cout << "address static=" << &bulk2.date << endl;
	double p_t_r = print_total(bulk1, 3);//69
	double p_t_r1 = print_total(item, 3);//0
	double p_t_r2 = print_total_1(bulk1, 3);//36

	//Disc_quote item2;//报错,抽象基类不能实例化






	return 0;


}
