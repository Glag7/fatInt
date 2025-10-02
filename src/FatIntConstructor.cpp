#include "FatInt.hpp"

#include <iostream>
FatInt::FatInt() :
	words{},
	sign(false)
{
	std::cout << "empty\n";
}

FatInt::FatInt(int64_t n)
{
	std::cout << "int\n";
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
	static uint32_t	pow10[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	size_t			i = s[0] == '+' || s[0] == '-';
	size_t			count = 0;
	uint32_t		num = 0;

	while (i < s.size())
	{
		if (count == 9)
		{
			umul_word(*this, 1000000000);
			uadd_word(*this, num);
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
	{
		umul_word(*this, pow10[count - 1]);
		uadd_word(*this, num);
	}
	sign = s[0] == '-';
}

FatInt::FatInt(const FatInt &f) :
	words(f.words),
	sign(f.sign)
{
	std::cout << "cpy\n";
}

FatInt::FatInt(FatInt &&f) : sign(f.sign)
{
	std::cout << "mv\n";
	words = std::move(f.words);
	f.sign = false;
}
