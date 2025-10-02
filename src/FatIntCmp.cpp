#include "FatInt.hpp"

bool	FatInt::operator==(const FatInt &n) const
{
	if (words.size() != n.words.size()
		|| (sign != n.sign && (!this->is_zero() || !n.is_zero())))
		return false;
	for (size_t	i = 0; i < words.size(); ++i)
	{
		if (words[i] != n.words[i])
			return false;
	}
	return true;
}

bool	FatInt::operator!=(const FatInt &n) const
{
	if (words.size() != n.words.size()
		|| (sign != n.sign && (!this->is_zero() || !n.is_zero())))
		return true;
	for (size_t	i = 0; i < words.size(); ++i)
	{
		if (words[i] != n.words[i])
			return true;
	}
	return false;
}

bool	FatInt::operator>=(const FatInt &n) const
{
	if (sign != n.sign && !this->is_zero() && !n.is_zero())
		return !sign;
	if (words.size() != n.words.size())
		return (words.size() > n.words.size()) ^ sign;
	for (size_t	i = words.size(); i-- >= 1;)
	{
		if (words[i] != n.words[i])
			return (words[i] > n.words[i]) ^ sign;
	}
	return true;
}

bool	FatInt::operator>(const FatInt &n) const
{
	if (sign != n.sign && !this->is_zero() && !n.is_zero())
		return !sign;
	if (words.size() != n.words.size())
		return (words.size() > n.words.size()) ^ sign;
	for (size_t	i = words.size(); i-- >= 1;)
	{
		if (words[i] != n.words[i])
			return (words[i] > n.words[i]) ^ sign;
	}
	return false;
}

bool	FatInt::operator<=(const FatInt &n) const
{
	if (sign != n.sign && !this->is_zero() && !n.is_zero())
		return sign;
	if (words.size() != n.words.size())
		return (words.size() < n.words.size()) ^ sign;
	for (size_t	i = words.size(); i-- >= 1;)
	{
		if (words[i] != n.words[i])
			return (words[i] < n.words[i]) ^ sign;
	}
	return true;
}

bool	FatInt::operator<(const FatInt &n) const
{
	if (sign != n.sign && !this->is_zero() && !n.is_zero())
		return sign;
	if (words.size() != n.words.size())
		return (words.size() < n.words.size()) ^ sign;
	for (size_t	i = words.size(); i-- >= 1;)
	{
		if (words[i] != n.words[i])
			return (words[i] < n.words[i]) ^ sign;
	}
	return false;
}
