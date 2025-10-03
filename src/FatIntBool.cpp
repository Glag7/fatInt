#include "FatInt.hpp"

FatInt::operator bool() const
{
	return !this->is_zero();
}

bool	FatInt::operator!()
{
	return this->is_zero();
}

bool	FatInt::operator&&(bool b) const
{
	return !this->is_zero() && b;
}

bool	FatInt::operator||(bool b) const
{
	return !this->is_zero() || b;
}
