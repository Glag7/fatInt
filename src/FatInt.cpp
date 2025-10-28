#include "FatInt.hpp"

uint64_t	FatInt::wordmax = (1ULL << 32) - 1;

void	FatInt::operator=(const FatInt &n)
{
	sign = n.sign;
	words = n.words;
}

FatInt	&FatInt::operator++()
{
	if (sign)
	{
		if (this->is_zero())
			sign = false;
		usub_word(*this, 1);
	}
	else
		uadd_word(*this, 1);
	return *this;
}

FatInt	&FatInt::operator--()
{
	if (!sign)
	{
		if (this->is_zero())
			sign = true;
		usub_word(*this, 1);
	}
	else
		uadd_word(*this, 1);
	return *this;
}

void	FatInt::trim()
{
	auto end = words.end();
	auto start = end;

	while (start != ++words.begin() && *(start - 1) == 0)
		--start;
	words.erase(start, end);
}

void	FatInt::uadd(FatInt &a, const FatInt &b)
{
	const FatInt	&small = (a.words.size() > b.words.size()) ? b : a;
	const FatInt	&big = (&small == &a) ? b : a;
	size_t			i = 0;
	uint32_t		carry = 0;
	bool			asmall = &a == &small;

	a.words.reserve(big.words.size() + 1);
	while (i < small.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(small.words[i]) + big.words[i] + carry;

		a.words[i] = (tmp & wordmax);
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	if (asmall)
		a.words.resize(big.words.size());
	while (i < big.words.size() && (carry | asmall))
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) + carry;

		a.words[i] = (tmp & wordmax);
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	if (carry)
		a.words.push_back(1);
}

void	FatInt::sub(FatInt &a, const FatInt &b)
{
	const FatInt	&small = (a.words.size() > b.words.size()) ? b : a;
	const FatInt	&big = (&small == &a) ? b : a;
	size_t			i = 0;
	uint32_t		carry = 0;
	bool			asmall = &a == &small;

	a.words.reserve(big.words.size());
	while (i < small.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) - small.words[i] - carry;

		a.words[i] = (tmp & wordmax);
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	if (asmall)
		a.words.resize(big.words.size());
	while (i < big.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) - carry;

		a.words[i] = (tmp & wordmax);
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	if (carry)
	{
		size_t	i = 0;

		while (i < a.words.size() && carry)
		{
			uint64_t	tmp = static_cast<uint64_t>(~a.words[i]) + 1;
			
			a.words[i] = static_cast<uint32_t>(tmp & wordmax);
			carry = (tmp & ~wordmax) && 1;
			++i;
		}
		while (i < a.words.size())
		{
			a.words[i] = ~a.words[i];
			++i;
		}
		if (carry)
			a.words.push_back(carry);
		a.sign ^= true;
	}
	a.sign ^= (&small == &a);
	a.trim();
}

FatInt	FatInt::operator+() const
{
	return *this;
}

FatInt	FatInt::operator-() const
{
	FatInt	res(*this);

	res.sign = !sign;
	return res;
}

void	FatInt::invert()
{
	sign = !sign;
}

FatInt	FatInt::operator+(const FatInt &n) const
{
	FatInt			res(*this);

	if (sign != n.sign)
		sub(res, n);
	else
		uadd(res, n);
	return res;
}

FatInt	FatInt::operator-(const FatInt &n) const
{
	FatInt			res(*this);

	if (sign == n.sign)
		sub(res, n);
	else
		uadd(res, n);
	return res;
}

void	FatInt::umul_naive(FatInt &dst, const FatInt &a, const FatInt &b)
{
	const FatInt	&small = (a.words.size() > b.words.size()) ? b : a;
	const FatInt	&big = (&small == &a) ? b : a;
	size_t			i = 0;
	uint32_t		carry = 0;

	dst.words.reserve(big.words.size() * 2);
	while (i < big.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) * small.words[0] + carry;

		dst.words.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) >> 32;
		++i;
	}
	if (carry)
		dst.words.push_back(carry);
	for (size_t j = 1; j < small.words.size(); ++j)
	{
		i = 0;
		while (i < big.words.size())
		{
			uint64_t	tmp = static_cast<uint64_t>(big.words[i]) * small.words[0] + carry;
			
			if (j < dst.words.size())
			{
				tmp += dst.words[i];
				dst.words[j] = tmp & wordmax;
			}
			else
				dst.words.push_back(tmp & wordmax);
			carry = (tmp & ~wordmax) >> 32;
			++i;
		}
		if (carry)
			dst.words.push_back(carry);
	}
}

FatInt	FatInt::operator*(const FatInt &n) const
{
	if ((words.size() == 1 && words[0] == 0) || (n.words.size() == 1 && n.words[0] == 0))
		return FatInt(0);

	FatInt	res;

	res.sign = sign ^ n.sign;
	umul_naive(res, *this, n);
	return res;
}


//TODO manage 0 (?), long div, newton
FatInt	FatInt::operator/(const FatInt &n) const
{
	FatInt	res, foo;
	
//	res.sign = sign ^ n.sign;
//	if (n.words.size() == 1 && n.words[0] != 0)
//		udiv_word(res, foo, *this, n.words[0]);
//	else
//	{
//		//yes
//	}
	return 0;//XXX
}

FatInt	FatInt::operator%(const FatInt &n) const
{
	FatInt	res, foo;
	
//	res.sign = sign ^ n.sign;//modulo or remainder ? not the same
//	if (n.words.size() == 1 && n.words[0] != 0)
//		udiv_word(foo, res, *this, n.words[0]);
//	else
//	{
//		//yes
//	}
	return 0;//XXX
}
void	FatInt::operator+=(const FatInt &n)
{
	if (sign != n.sign)
		sub(*this, n);
	else
		uadd(*this, n);
}

void	FatInt::operator-=(const FatInt &n)
{
	if (sign == n.sign)
		sub(*this, n);
	else
		uadd(*this, n);
}

void	FatInt::operator*=(const FatInt &n)
{
}

void	FatInt::operator/=(const FatInt &n)
{
}

void	FatInt::operator%=(const FatInt &n)
{
}

std::ostream	&operator<<(std::ostream &o, const FatInt &f)
{
	#ifdef FATINT_DEBUG
		o << '(' << f.words.size() << ')';
	#endif
	o << FatInt::to_string(f);
	return o;
}

