#include "FatInt.hpp"

void	FatInt::uadd_word(FatInt &a, uint32_t b)
{
	size_t			i = 0;
	uint32_t		carry = b;

	while (carry && i < a.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(a.words[i]) + carry;

		a.words[i] = tmp & wordmax;
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	if (carry)
		a.words.push_back(1);
}

//TODO sub

void	FatInt::umul_word(FatInt &a, uint32_t b)
{
	size_t			i = 0;
	uint32_t		carry = 0;

	while (i < a.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(a.words[i]) * b + carry;

		a.words[i] = tmp & wordmax;
		carry = (tmp & ~wordmax) >> 32;
		++i;
	}
	if (carry)
		a.words.push_back(carry);
}

void	FatInt::udiv_word(FatInt &num, uint32_t div, uint32_t *mod)
{
	uint32_t	carry = 0, fac = 0;
	size_t		i = num.words.size();

	while (fac == 0 && i-- != 0)
	{
		uint64_t	tmp = (static_cast<uint64_t>(carry) << 32) | num.words[i];
	 	
		fac = tmp / div;
		num.words[i] = fac;
		carry = tmp - fac * div;
	}
	if (!fac)
		++i;
	num.words.erase(num.words.begin() + i + 1, num.words.end());
	while (i-- != 0)
	{
		uint64_t	tmp = (static_cast<uint64_t>(carry) << 32) | num.words[i];
	 	
		fac = tmp / div;
		num.words[i] = fac;
		carry = tmp - fac * div;
	}
	*mod = carry;
}
