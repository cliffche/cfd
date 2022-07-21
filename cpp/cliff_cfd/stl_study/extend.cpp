using namespace std;
#include <string>
class Quote
{
public:

	Quote();
	~Quote();
	string isbn() const;
	virtual double net_price(size_t n) const;

};

Quote::Quote()
{
}

Quote::~Quote()
{
}
class Bulk_Quote : public Quote
{
public:
	double net_price(size_t) const override;
};

