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

FatInt	FatInt::operator~() const
{
	FatInt	res(*this);

	for (auto &&word : res.words)
		word = ~word;
	return res;
}

void	FatInt::flip()
{
	for (auto &&word : words)
		word = ~word;
}
