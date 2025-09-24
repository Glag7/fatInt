#include "FatInt.hpp"

uint64_t	FatInt::wordmax = (1ULL << 32) - 1;

FatInt::FatInt() :
	words{},
	neg(0)
{
}

FatInt::FatInt(int64_t n)
{
	uint64_t	un;

	neg = n < 0;
	un = neg ? -n : n;
	words.push_back(un & wordmax);
	if (un & ~wordmax)
		words.push_back(un >> 32);
}

FatInt::FatInt(const std::string &s)
{
	//TODO
}

void	FatInt::trim()
{
	auto end = words.end();
	auto start = end;

	while (start != ++words.begin() && *(start - 1) == 0)
		--start;
	words.erase(start, end);
}

void	FatInt::uadd(FatInt &dst, const FatInt &a, const FatInt &b)
{
	const FatInt	&small = (a.words.size() > b.words.size()) ? b : a;
	const FatInt	&big = (&small == &a) ? b : a;
	size_t			i = 0;
	uint32_t		carry = 0;

	dst.words.reserve(big.words.size() + 1);
	while (i < small.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(small.words[i]) + big.words[i] + carry;

		dst.words.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) >> 32;
		++i;
	}
	while (i < big.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) + carry;

		dst.words.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) >> 32;
		++i;
	}
	if (carry)
		dst.words.push_back(1);
}

void	FatInt::usub(FatInt &dst, const FatInt &a, const FatInt &b)
{
	const FatInt	&small = (a.words.size() > b.words.size()) ? b : a;
	const FatInt	&big = (&small == &a) ? b : a;
	size_t			i = 0;
	uint32_t		carry = 0;

	dst.words.reserve(big.words.size());
	while (i < small.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) - small.words[i] - carry;

		dst.words.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	while (i < big.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) - carry;

		dst.words.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	if (carry)
	{
		size_t	i = 0;

		while (i < dst.words.size() && carry)
		{
			uint64_t	tmp = static_cast<uint64_t>(~dst.words[i]) + 1;
			
			dst.words[i] = static_cast<uint32_t>(tmp & wordmax);
			carry = (tmp & ~wordmax) && 1;
			++i;
		}
		while (i < dst.words.size())
		{
			dst.words[i] = ~dst.words[i];
			++i;
		}
		if (carry)
			dst.words.push_back(carry);
		dst.neg = true;
	}
	dst.neg ^= (&small == &a) ^ a.neg;
	dst.trim();
}

FatInt	FatInt::operator-() const
{
	FatInt	res = *this;

	res.neg = !neg;
	return res;
}

void	FatInt::flip()
{
	for (auto &&word : words)
		word = ~word;
}

FatInt	FatInt::operator+(const FatInt &n) const
{
	FatInt			res;

	if (neg != n.neg)
	{
		usub(res, *this, n);
	}
	else
	{
		res.neg = neg;
		uadd(res, *this, n);
	}
	return res;
}

FatInt	FatInt::operator-(const FatInt &n) const
{
	FatInt			res;

	if (neg == n.neg)
	{
		usub(res, *this, n);
	}
	else
	{
		res.neg = neg;
		uadd(res, *this, n);
	}
	return res;
}

std::ostream	&operator<<(std::ostream &o, const FatInt &f)
{
	o << '(' << f.words.size() << ')';//
	if (f.neg)
		o << '-';
	//garbage
	if (f.words.size() == 1)
		o << f.words[0];
	else if (f.words.size() == 2)
		o << ((static_cast<uint64_t>(f.words[1]) << 32) | f.words[0]);
	else
		o << "too big";
	//
	return o;
}

