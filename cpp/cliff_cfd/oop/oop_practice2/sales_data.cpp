#include <iostream>
#include "sales_data.h"
using namespace std;

double Sales_data::actual_price()const {
	//��Ա������ͷ�ļ�����,Դ�ļ�����
	return (double)price * discount;
}
Sales_data& Sales_data::combine(const Sales_data& rhs) {
	return *this;
}

int main1() {
	Sales_data data1, data2;
	data1.price = 1.5;
	data1.sku = 233453;
	data1.timedate = "2022-12-30";
	data1.discount = 0.86;
	data1.account = 1;
	int sku1 = data1.get_sku();
	cout << data1.account << endl;
	return 0;
}