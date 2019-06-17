#pragma once
#include "cinary.h"

//  sample class
class Hoge {
private:
	int count;
	double rate;
	char symbol;

public:
	Hoge() = default;

	Hoge(int count, double rate, char symbol)
		: count(count), rate(rate), symbol(symbol)
	{}

	Hoge(cinary::IBStream& ibs)
		: count(ibs["count"]), rate(ibs["rate"]), symbol(ibs["symbol"])
	{}

	void __output(cinary::OBStream& obs) const
	{
		obs.write("count" , count );
		obs.write("rate"  , rate  );
		obs.write("symbol", symbol);
	}


	friend std::ostream& operator <<(std::ostream& os, const Hoge& hoge);
	friend std::istream& operator >>(std::istream& is, Hoge& hoge);
};

std::ostream& operator <<(std::ostream& os, const Hoge& hoge)
{
	return os << "Hoge(count: " << hoge.count << ", rate:" << hoge.rate << ", symbol:" << hoge.symbol << ")";
}

std::istream& operator >>(std::istream& is, Hoge& hoge)
{
	return is >> hoge.count >> hoge.rate >> hoge.symbol;
}