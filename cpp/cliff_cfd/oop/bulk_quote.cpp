#include "bulk_quote.h"
using namespace std;
#include <iostream>

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


	return 0;
}
