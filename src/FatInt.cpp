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
	if (neg != n.neg)
		return (neg) ? n - -*this : *this - -n;//un peu a chier, passer par un wrapper
		//FIXME uadd usub pour eviter de totu comparer 2 fois

	FatInt			res;
	const FatInt	&small = (words.size() > n.words.size()) ? n : *this;
	const FatInt	&big = (&small == this) ? n : *this;
	size_t			i = 0;
	uint32_t		carry = 0;

	res.words.reserve(big.words.size() + 1);
	res.neg = neg;
	while (i < small.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(small.words[i]) + big.words[i] + carry;

		res.words.push_back(static_cast<uint32_t>(tmp & wordmax));
		carry = (tmp & ~wordmax) >> 32;
		++i;
	}
	while (i < big.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) + carry;

		res.words.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) >> 32;
		++i;
	}
	if (carry)
		res.words.push_back(1);
	return res;
}

//envoer dans le usub (qui peut renvoyer des negatifs) pusi xor

#include <iostream>
FatInt	FatInt::operator-(const FatInt &n) const//considere les 2 positifs
{
	FatInt			res;
	const FatInt	&small = (words.size() > n.words.size()) ? n : *this;
	const FatInt	&big = (&small == this) ? n : *this;
	size_t			i = 0;
	uint32_t		carry = 0;

	res.words.reserve(big.words.size() + 1);
	while (i < small.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) - small.words[i] - carry;

		res.words.push_back(static_cast<uint32_t>(tmp & wordmax));
		carry = (tmp & ~wordmax) >> 32;
		++i;
	}
	while (i < big.words.size())
	{
		uint64_t	tmp = static_cast<uint64_t>(big.words[i]) - carry;

		res.words.push_back(tmp & wordmax);
		carry = (tmp & ~wordmax) >> 32;
		++i;
	}
	if (carry)
	{
		//std::cout << "wooooooo\n";
		res.words.push_back(carry);
		res.flip();
		res = res + 1;//++
		res.neg = true;
		//tout xor, + 1, signe a neg
	}
	res.neg ^= &small == this;
	//check si carry plus grand que le reste du nombre, si c'est le cas les nombres etaient flip
	//prendre en compte signes, n et this et big et small
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

