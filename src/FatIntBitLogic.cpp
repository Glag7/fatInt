#include "FatInt.hpp"

void	FatInt::uand(FatInt &a, const FatInt &b)
{
	const FatInt	&small = (a.words.size() > b.words.size()) ? b : a;
	
	for (size_t i = 0; i < small.words.size(); ++i)
		a.words[i] &= b.words[i];
	if (&small != &a)
		a.words.erase(a.words.begin() + small.words.size(), a.words.end());
}

void	FatInt::uor(FatInt &a, const FatInt &b)
{
	const FatInt	&small = (a.words.size() > b.words.size()) ? b : a;
	
	for (size_t i = 0; i < small.words.size(); ++i)
		a.words[i] |= b.words[i];
	if (&small == &a)
		a.words.insert(a.words.end(), b.words.begin() + a.words.size(), b.words.end());
}

void	FatInt::uxor(FatInt &a, const FatInt &b)
{
	const FatInt	&small = (a.words.size() > b.words.size()) ? b : a;
	
	for (size_t i = 0; i < small.words.size(); ++i)
		a.words[i] ^= b.words[i];
	if (&small == &a)
		a.words.insert(a.words.end(), b.words.begin() + a.words.size(), b.words.end());
}

void	FatInt::ushift_left(FatInt &f, uint64_t n)
{
	uint64_t	word_shift = n / 32;
	uint64_t	digit_shift = n % 32;
	uint32_t	carry = 0;

	for (auto it = f.words.begin(); it != f.words.end(); ++it)
	{
		uint64_t	tmp = (static_cast<uint64_t>(*it) << digit_shift) | carry;

		*it = tmp & wordmax;
		carry = tmp >> 32;
	}
	if (carry)
		f.words.push_back(carry);
	if (word_shift && word_shift - !!carry)
		f.words.insert(f.words.begin(), word_shift - !!carry, 0);
}

void	FatInt::ushift_right(FatInt &f, uint64_t n)
{
	uint64_t	word_shift = n / 32;
	uint64_t	digit_shift = 32 - n % 32;
	uint32_t	carry = 0;

	if (word_shift >= f.words.size())
	{
		f.words = {0};
		return;
	}
	if (word_shift)
		f.words.erase(f.words.begin(), f.words.begin() + word_shift);
	for (auto it = f.words.rbegin(); it != f.words.rend(); ++it)
	{
		uint64_t	tmp = (static_cast<uint64_t>(*it) <<  digit_shift) | carry;

		carry = tmp & wordmax;
		*it = tmp >> 32;
	}
}

FatInt	FatInt::operator~() const
{
	FatInt	res(*this);

	if (res.is_zero())
		res.sign = false;
	for (auto &&word : res.words)
		word = ~word;
	return res;
}

FatInt	FatInt::operator&(const FatInt &f) const
{
	FatInt	res(*this);

	uand(res, f);
	return res;
}

FatInt	FatInt::operator|(const FatInt &f) const
{
	FatInt	res(*this);

	uor(res, f);
	return res;
}

FatInt	FatInt::operator^(const FatInt &f) const
{
	FatInt	res(*this);

	uxor(res, f);
	return res;
}

FatInt	FatInt::operator>>(const FatInt &f) const
{
	uint64_t	shift = (f.words.size() == 1) ? f.words[0]
		: (static_cast<uint64_t>(f.words[1]) << 32) | f.words[0];

	if (f.words.size() > 2 || shift > (1ULL << 63) - 1)
		throw std::invalid_argument("shift too large");

	FatInt	res = *this;

	if (f.sign)
		ushift_left(res, shift);
	else
		ushift_right(res, shift);
	return res;
}

FatInt	FatInt::operator<<(const FatInt &f) const
{
	uint64_t	shift = (f.words.size() == 1) ? f.words[0]
		: (static_cast<uint64_t>(f.words[1]) << 32) | f.words[0];

	if (f.words.size() > 2 || shift > (1ULL << 63) - 1)
		throw std::invalid_argument("shift too large");

	FatInt	res = *this;

	if (f.sign)
		ushift_right(res, shift);
	else
		ushift_left(res, shift);
	return res;
}

void	FatInt::flip()
{
	if (this->is_zero())
		sign = false;
	for (auto &&word : words)
		word = ~word;
}
		
void	FatInt::operator&=(const FatInt &f)
{
	uand(*this, f);
}

void	FatInt::operator|=(const FatInt &f)
{
	uor(*this, f);
}

void	FatInt::operator^=(const FatInt &f)
{
	uxor(*this, f);
}

void	FatInt::operator>>=(const FatInt &f)
{
	uint64_t	shift = (f.words.size() == 1) ? f.words[0]
		: (static_cast<uint64_t>(f.words[1]) << 32) | f.words[0];

	if (f.words.size() > 2 || shift > (1ULL << 63) - 1)
		throw std::invalid_argument("shift too large");
	if (f.sign)
		ushift_left(*this, shift);
	else
		ushift_right(*this, shift);
}

void	FatInt::operator<<=(const FatInt &f)
{
	uint64_t	shift = (f.words.size() == 1) ? f.words[0]
		: (static_cast<uint64_t>(f.words[1]) << 32) | f.words[0];

	if (f.words.size() > 2 || shift > (1ULL << 63) - 1)
		throw std::invalid_argument("shift too large");
	if (f.sign)
		ushift_right(*this, shift);
	else
		ushift_left(*this, shift);
}
