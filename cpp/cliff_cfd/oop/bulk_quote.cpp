#include "bulk_quote.h"
using namespace std;
#include <iostream>

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


	return 0;
}
