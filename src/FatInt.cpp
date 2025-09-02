#include "FatInt.hpp"

uint32_t	FatInt::wordmax = (1ULL << 32) - 1;

FatInt::FatInt() :
	words{0},
	neg(0)
{
	std::cout << wordmax << "\n";
}

FatInt::FatInt(uint64_t n) : neg(0)
{
	words.push_back(n & wordmax);
	if (n > wordmax)
		words.push_back(n >> 32);
}

FatInt::FatInt(int64_t n)
{
	uint64_t	un;

	neg = n < 0;
	un = neg ? -n : n;
	words.push_back(un & wordmax);
	if (un > wordmax)
		words.push_back(un >> 32);
}

FatInt::FatInt(const std::string &s)
{
}

FatInt	FatInt::operator+(const FatInt &n) const//pour l'instant que + +, plus tard wrapper vers + -
{
	FatInt			res;
	const FatInt	&small = (words.size() > n.words.size()) ? n : *this;
	const FatInt	&big = (&small == this) ? n : *this;
	size_t			i = 0;
	uint32_t		carry = 0;

	res.words.reserve(big.size() + 1);
	while (i < small.words.size())
	{
		uint64_t	tmp = small[i] + big[i] + carry;

		res.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) >> 32;
	}
	while (i < big.words.size())
	{
		uint64_t	tmp = big[i] + carry;

		res.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) >> 32;
	}
	if (carry)
		res.push_back(1);
	return res;
}


std::ostream	&operator<<(std::ostream &o, const FatInt &f)
{
	if (neg)
		o << '-';
	//garbage
	if (f.words.size() == 1)
		o << words[0];
	else if (f.words.size() == 2)
		o << ((static_cast<uint64_t>words[1] << 32) | words[0]);
	else
		o << "too big";
	//
	return o;
}

