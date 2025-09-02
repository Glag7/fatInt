#ifndef FATINT_HPP
# define FATINT_HPP

#include <vector>
#include <ostream>
#include <cstdint>
#include <string>

class	FatInt
{
	private:
		static uint32_t			wordmax;

		std::vector<uint32_t>	words;
		bool					neg;

	public:
		FatInt();
		FatInt(uint64_t n);
		FatInt(int64_t n);
		FatInt(const std::string &s);//move copy etc

		//operator=(const FatInt &n);
		FatInt	operator+(const FatInt &n) const;
		//FatInt	operator-(const FatInt &n);//*/% + bool
		
		friend std::ostream	&operator<<(std::ostream &o, const FatInt &f);
};

#endif
