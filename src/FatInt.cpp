#include "FatInt.hpp"

//X-------------------...
//^signe

//erase le bit de signe, addition, check le bit de signe

FatInt::FatInt() :
	start(0ULL),
	end(0ULL)
{
}

FatInt::FatInt(uint64_t n)
{
}

FatInt::FatInt(int64_t n) :
{
}

FatInt::FatInt(const std::string &s)
{
}

FatInt::~FatInt()
{
}

//check le signe, si - 

FatInt	FatInt::operator+(const FatInt &n)
{
	FatInt		res;
	uint64_t	*ptr = start;
	uint64_t	*nptr = n.start;

	res.start = new uint64_t[bigptr.end - bigptr.start]
	while (ptr != end && nptr != n.end)
	{
		++ptr;
		++nptr
	}
}

FatInt	FatInt::operator-(const FatInt &n)
{
}
