#include "FatInt.hpp"

uint64_t	FatInt::wordmax = (1ULL << 32) - 1;

void	FatInt::operator=(const FatInt &n)
{
	sign = n.sign;
	words = n.words;
}

void	FatInt::trim()
{
	auto end = words.end();
	auto start = end;

	while (start != ++words.begin() && *(start - 1) == 0)
		--start;
	words.erase(start, end);
}

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
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	while (i < big.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) + carry;

		dst.words.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) && 1;
		++i;
	}
	if (carry)
		dst.words.push_back(1);
}

void	FatInt::sub(FatInt &dst, const FatInt &a, const FatInt &b)
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
		dst.sign = true;
	}
	dst.sign ^= (&small == &a) ^ a.sign;
	dst.trim();
}

FatInt	FatInt::operator-() const
{
	FatInt	res = *this;

	res.sign = !sign;
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

	if (sign != n.sign)
	{
		sub(res, *this, n);
	}
	else
	{
		res.sign = sign;
		uadd(res, *this, n);
	}
	return res;
}

FatInt	FatInt::operator-(const FatInt &n) const
{
	FatInt			res;

	if (sign == n.sign)
	{
		sub(res, *this, n);
	}
	else
	{
		res.sign = sign;
		uadd(res, *this, n);
	}
	return res;
}

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

//TODO karatsuba, fft
FatInt	FatInt::operator*(const FatInt &n) const
{
	if ((words.size() == 1 && words[0] == 0) || (n.words.size() == 1 && n.words[0] == 0))
		return FatInt(0);

	FatInt	res;

	res.sign = sign ^ n.sign;
	umul_naive(res, *this, n);
	return res;
}

void	FatInt::udiv_word(FatInt &qt, FatInt &rm, const FatInt &num, uint32_t div)
{//XXX
	uint32_t	carry = 0;
	size_t		i = num.words.size();

	//FIXME reserve + put at the end ?
	while (i-- != 0)
	{
		uint64_t	tmp = (static_cast<uint64_t>(carry) << 32) | num.words[i];

	 	uint64_t	huh = tmp / div;

		qt.words.insert(qt.words.begin(), huh);
		carry = tmp - huh * div;
	}
	rm = carry;
	qt.trim();//weird
}

//TODO manage 0 (?), long div, newton
FatInt	FatInt::operator/(const FatInt &n) const
{
	FatInt	res, foo;
	
	res.sign = sign ^ n.sign;
	if (n.words.size() == 1 && n.words[0] != 0)
		udiv_word(res, foo, *this, n.words[0]);
	else
	{
		//yes
	}
	return res;
}

FatInt	FatInt::operator%(const FatInt &n) const
{
	FatInt	res, foo;
	
	res.sign = sign ^ n.sign;//modulo or remainder ? not the same
	if (n.words.size() == 1 && n.words[0] != 0)
		udiv_word(foo, res, *this, n.words[0]);
	else
	{
		//yes
	}
	return res;
}

#include <iostream>
std::string	FatInt::tostring() const
{
	FatInt	cpy = *this;
	std::string	s;

	if (cpy.words.size() == 1 && cpy.words[0] == 0)
		return "0";
	s.reserve(words.size() * 10 + 1);//9.63 is enough
	while (!(cpy.words.size() == 1 && cpy.words[0] == 0))
	{
		std::string	tmp = std::to_string((cpy % 1000000000).words[0]);
		cpy = cpy / 1000000000;
		if (!(cpy.words.size() == 1 && cpy.words[0] == 0))
			s = (std::string(9 - tmp.size(), '0') + tmp) + s;
		else
			s = tmp + s;
		//std::cout << "|||" << cpy << "|||";
	}
	if (cpy.sign)
		s.insert(s.begin(), '-');
	return s;
}

std::ostream	&operator<<(std::ostream &o, const FatInt &f)
{
	o << f.tostring();
	return o;
	o << '(' << f.words.size() << ')';//
	if (f.sign)
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

