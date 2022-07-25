#pragma once
#include "quote.h"
class Disc_quote : public Quote
{
	//抽象基类  含有纯虚函数的类
	//负责定义接口
public:
	Disc_quote() = default;
	Disc_quote(std::string& book, double p, std::size_t qty, double disc) :Quote(book, p), quantity(qty), discount(disc) {};
	double net_price(size_t) const = 0;//纯虚函数
	~Disc_quote();
protected:
	size_t quantity = 0;
	double discount = 0.0;
private:

};

Disc_quote::Disc_quote()
{
}

Disc_quote::~Disc_quote()
{
}