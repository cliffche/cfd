#pragma once
#include "quote.h"
class Disc_quote : public Quote
{
	//�������  ���д��麯������
	//������ӿ�
public:
	Disc_quote() = default;
	Disc_quote(std::string& book, double p, std::size_t qty, double disc) :Quote(book, p), quantity(qty), discount(disc) {};
	double net_price(size_t) const = 0;//���麯��
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