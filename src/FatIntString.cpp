#include <exception>
#include "FatInt.hpp"

std::string	FatInt::to_string(const FatInt &f)
{
	if (f.is_zero())
		return "0";
	
	FatInt		cpy = f;
	std::string	s;

	s.reserve(f.words.size() * 10 + 1);//9.63 is enough
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
	static const size_t		logs[] = {0, 0, 32, 21, 16, 14, 13, 12, 11, 11, 10, 10, 9, 9, 9, 9, 8};
	static const uint32_t	bigpow[] = {0, 0, 2147483648, 3486784401, 1073741824, 1220703125, 2176782336, 1977326743, 1073741824, 3486784401, 1000000000, 2357947691, 429981696, 815730721, 1475789056, 2562890625, 268435456};
	static const char		hex[] = "0123456789ABCDEF";

	if (base < 2 || base > 16)
		throw std::invalid_argument("base must be between 2 and 16")
	if (f.is_zero())
		return "0";
	
	FatInt		cpy = f;
	std::string	s;

	s.reserve(f.words.size() * logs[base] + 1);
	while (!cpy.is_zero())
	{
		uint32_t	mod;

		udiv_word(cpy, bigpow[base], &mod);

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
