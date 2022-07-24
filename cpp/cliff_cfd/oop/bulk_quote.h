#pragma once
#ifndef _BULK_QUOTE_H_
#define _BULK_QUOTE_H_
#include "quote.h"
class Bulk_quote : public Quote
{
public:
	Bulk_quote() = default;
	//Bulk_quote(const std::string&, double, std::size_t, double);
	//book和p去初始化基类Quote
	Bulk_quote(std::string& book, double p, std::size_t qty, double disc) :Quote(book, p), min_qty(qty), discount(disc) {};
	double net_price(std::size_t) const override;

private:
	std::size_t min_qty = 0;
	double discount = 0.0;

};



double Bulk_quote::net_price(std::size_t t) const {//有const和没有const也是重载
	return t * discount;
}
#endif // _BULK_QUOTE_H_

