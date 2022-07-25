#pragma once
#ifndef _QUOTE_H_
#define _QUOTE_H_
#include <string>
using namespace std;
class Quote
{
public:
	Quote() = default;
	Quote(string& book, double sale_price) : bookNo(book), price(sale_price) {};
	virtual double net_price(size_t n) const { return n * price; }

	virtual ~Quote() = default;
	static string date() {
		return "1453-1-1";
	};
private:
	string bookNo;
protected:
	double price = 0;
};

#endif // !_QUOTE_H_
