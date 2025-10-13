#include <exception>
#include "FatInt.hpp"

std::string	FatInt::to_string(const FatInt &f)
{
	if (f.is_zero())
		return "0";
	
	FatInt		cpy = f;
	std::string	s;

	s.reserve(f.words.size() * 10 + 1);
	while (!cpy.is_zero())
	{
		uint32_t	mod;

		udiv_word(cpy, 1000000000, &mod);

		std::string	tmp = std::to_string(mod);
		if (!cpy.is_zero())
			s = (std::string(9 - tmp.size(), '0') + tmp) + s;
		else
			s = tmp + s;
	}
	if (cpy.sign)
		s.insert(s.begin(), '-');
	return s;
}

std::string	FatInt::to_string(const FatInt &f, unsigned base)
{
	static const uint8_t	logs[] = {0, 0, 32, 21, 16, 14, 13, 12, 11, 11, 10, 10, 9, 9, 9, 9, 8};
	static const uint32_t	bigpow[] = {0, 0, 2147483648, 3486784401, 1073741824, 1220703125, 2176782336, 1977326743, 1073741824, 3486784401, 1000000000, 2357947691, 429981696, 815730721, 1475789056, 2562890625, 268435456};
	static const char		hex[] = "0123456789ABCDEF";

	if (base < 2 || base > 16)
		throw std::invalid_argument("invalid base");
	if (base == 10)
		return FatInt::to_string(f);
	if (f.is_zero())
		return "0";
	
	FatInt		cpy = f;
	std::string	s;

	s.reserve(f.words.size() * logs[base] + 1);
	while (!cpy.is_zero())
	{
		uint32_t	mod;
		size_t		i = logs[base] - 1;

		udiv_word(cpy, bigpow[base], &mod);

		std::string	tmp(i, '0');
		while (mod)
		{
			tmp[--i] = hex[mod % base];
			mod /= base;
		}
		if (cpy.is_zero())
			tmp.erase(tmp.begin(), tmp.begin() + i);
		s = tmp + s;
	}
	if (cpy.sign)
		s.insert(s.begin(), '-');
	return s;
}

static void	check_base(const std::string &b)
{
	bool	seen[256] = {0};

	for (uint8_t c : b)
	{
		#ifndef FATINT_YES_MINUS
		if (c == '-')
			throw std::invalid_argument("base contains '-'");
		#endif
		if (seen[c])
			throw std::invalid_argument("base contains the same character twice");
		seen[c] = 1;
	}
}

std::string	FatInt::to_string(const FatInt &f, const std::string &base)
{
	static const uint8_t	logs[256] = {0, 0, 32, 21, 16, 14, 13, 12, 11, 11, 10, 10, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	const uint32_t			size = base.size();

	if (size < 2 || size > 255)
		throw std::invalid_argument("base length must be between 2 and 255");
	check_base(base);
	if (f.is_zero())
		return std::string(1, base[0]);
	
	FatInt		cpy = f;
	std::string	s;
	uint32_t	bigpow = size;
	uint32_t	ndigit = logs[size];

	s.reserve(f.words.size() * ndigit + 1);
	while (static_cast<uint64_t>(bigpow) * size < wordmax)
		bigpow *= size;
	while (!cpy.is_zero())
	{
		uint32_t	mod;
		size_t		i = ndigit - 1;

		udiv_word(cpy, bigpow, &mod);

		std::string	tmp(i, base[0]);
		while (mod)
		{
			tmp[--i] = base[mod % size];
			mod /= size;
		}
		if (cpy.is_zero())
			tmp.erase(tmp.begin(), tmp.begin() + i);
		s = tmp + s;
	}
	if (cpy.sign)
		s.insert(s.begin(), '-');
	return s;
}
