#include "FatInt.hpp"

FatInt::FatInt() :
	words{},
	sign(false)
{
}

FatInt::FatInt(int64_t n)
{
	uint64_t	un;

	sign = n < 0;
	un = sign ? -n : n;
	words.push_back(un & wordmax);
	if (un & ~wordmax)
		words.push_back(un >> 32);
}

FatInt::FatInt(const std::string &s) :
	words{0},
	sign(false)
{
	static uint32_t	p10[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	size_t			i = s[0] == '+' || s[0] == '-';
	size_t			count = 0;
	uint32_t		num = 0;
	bool			minus = s[0] == '-';

	while (i < s.size())
	{
		if (count == 9)
		{
			*this = *this * 1000000000 + num;//TODO, *=, +=, * for uint32_t ?
			count = 0;
			num = 0;
		}
		else
		{
			num = 10 * num + s[i] - '0';
			++count;
			++i;
		}
	}
	if (count)
		*this = *this * p10[count - 1] + num;
	sign = minus;
}

FatInt::FatInt(const FatInt &f) :
	words(f.words),
	sign(f.sign)
{
}

FatInt::FatInt(FatInt &&f) : sign(f.sign)
{
	words = std::move(f.words);
	f.sign = false;
}
