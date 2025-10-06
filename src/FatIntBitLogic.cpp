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

void	FatInt::ushift(FatInt &f, int64_t n)
{
	bool		left = (n >= 0);
	uint64_t	shift = !left ? -n : n;
	uint64_t	word_shift = shift / 32;
	uint64_t	digit_shift = shift % 32;
	uint32_t	carry = 0;

//if rifgt (it removes)
//stuff
//if left (it adds)

	if (left)
	{
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
	else
	{
		if (word_shift > f.words.size())
			f.words = {0};
		else if (word_shift)
			f.words.erase(f.words.begin(), f.words.begin() + word_shift);
		for (auto it = f.words.rbegin(); it != f.words.rend(); ++it)
		{
			uint64_t	tmp = (static_cast<uint64_t>(*it) << 32 >> digit_shift) | carry;

			carry = tmp & wordmax;
			*it = tmp >> 32;
		}
	}
}

FatInt	FatInt::operator~() const
{
	FatInt	res(*this);

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
	//check size
	uint64_t	shift = static_cast<uint64_t>(f.words[1]) | f.words[0];

	
	FatInt	res = *this;
	ushift(res, -shift);
	return res;
}

FatInt	FatInt::operator<<(const FatInt &f) const
{
	//check size
	//uint64_t	shift = static_cast<uint64_t>(f.words[1]) | f.words[0];
	uint64_t	shift = f.words[0];

	
	FatInt	res = *this;
	if (f.sign)
		ushift(res, -shift);
	else
		ushift(res, shift);
	return res;
}

void	FatInt::flip()
{
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
