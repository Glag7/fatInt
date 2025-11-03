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

void	FatInt::umul(FatInt &a, const FatInt &b)
{
	FatInt			dst = 0;

	dst.words.reserve(((a.words.size() > b.words.size()) ? a.words.size() : b.words.size()) * 2);
	for (size_t j = 0; j < a.words.size(); ++j)
	{
		for (size_t i = 0; i < b.words.size(); ++i)
		{
			uint64_t	tmp = static_cast<uint64_t>(b.words[i]) * a.words[j];
		
			uadd(dst, FatInt(tmp) << ((i + j) * 32));//XXX kinda garbage but will do for now
		}
	}
	dst.sign = a.sign;
	a = std::move(dst);
}

FatInt	FatInt::operator*(const FatInt &n) const
{
	if (this->is_zero() || n.is_zero())
		return 0;

	FatInt	res = *this;

	res.sign ^= n.sign;
	umul(res, n);
	return res;
}

static inline size_t	msb(uint32_t n)
{
	size_t	shift = 31;

	n |= 1;
	while (!(n & (1U << shift)))
		--shift;
	return shift;
}

#include <iostream>
void	FatInt::udiv(FatInt &q, FatInt &r, const FatInt &num, const FatInt &div)
{
	std::cerr << "AMONG US================== " << num << " " << div << "\n";
	q = 0;
	r = num;
	if (div.is_zero())
	{
		#ifdef FATINT_ZERO_THROW
		throw std::logic_error("division by 0");
		#else
		return;
		#endif
	}
	r.sign = div.sign;
	while ((div.sign && r <= div) || (!div.sign && r >= div))//ucmp anyone
	{
		std::cerr <<  "wtf" << r << " " << div << " " << (r >= div) << "\n";
		size_t	shift = (r.words.size() - div.words.size()) * 32
						+ (msb(*r.words.rbegin()) - msb(*div.words.rbegin()));

		if (shift == 0)
		{
			shift = 1;
			//idk
		}
		--shift;
		q += FatInt(static_cast<int64_t>(1)) << shift;
		r -= div * FatInt(static_cast<int64_t>(1)) << shift;
		r.sign = div.sign;
		std::cerr << "s " << shift << "\n";
		std::cerr << "q " << q << "\n";
		std::cerr << "r " << r << "\n";
	}
}

FatInt	FatInt::operator/(const FatInt &n) const
{
	FatInt	res, foo;
	
	udiv(res, foo, *this, n);
	res.sign = sign ^ n.sign;
	return res;
}

FatInt	FatInt::operator%(const FatInt &n) const
{
	FatInt	res, foo;
	
	udiv(foo, res, *this, n);
	res.sign = sign;
	return res;
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
	if (this->is_zero() || n.is_zero())
	{
		*this = 0;
		return;
	}
	sign ^= n.sign;
	umul(*this, n);
}

void	FatInt::operator/=(const FatInt &n)
{
	*this = *this / n;
}

void	FatInt::operator%=(const FatInt &n)
{
	*this = *this % n;
}

std::ostream	&operator<<(std::ostream &o, const FatInt &f)
{
	#ifdef FATINT_DEBUG
		o << '(' << f.words.size() << ')';
	#endif
	o << FatInt::to_string(f);
	return o;
}

