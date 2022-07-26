#pragma once
#ifndef _SALES_DATA_H_
#define _SALES_DATA_H_
#include <string>
class Sales_data
{
public:
	typedef std::string::size_type pos;
	double price;
	int sku;
	double discount;
	std::string timedate;
	double actual_price() const;
	pos account;
	//int get_sku() const { return sku; }
	int get_sku() const { return this->sku; }//this是隐式形参
	Sales_data& combine(const Sales_data&);
	Sales_data();
	~Sales_data();

private:

};
Sales_data::Sales_data()
{
}

Sales_data::~Sales_data()
{
}
#endif // !_SALES_DATA_H_
