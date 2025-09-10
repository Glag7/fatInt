#ifndef FATINT_HPP
# define FATINT_HPP

#include <vector>
#include <ostream>
#include <cstdint>
#include <string>

class	FatInt
{
	private:
		static uint64_t			wordmax;

		FatInt();
	
		std::vector<uint32_t>	words;
		bool					neg;

		static void	uadd(FatInt &dst, const FatInt &a, const FatInt &b);
		static void	usub(FatInt &dst, const FatInt &a, const FatInt &b);

	public:
		FatInt(int64_t n);
		FatInt(const std::string &s);//move copy etc

		//operator=(const FatInt &n);
		//unary -, ++, etc
		FatInt	operator-() const;
		FatInt	operator~() const;
		void	flip();
		//FatInt	operator~() const;
		FatInt	operator+(const FatInt &n) const;
		FatInt	operator-(const FatInt &n) const;
		//FatInt	operator-(const FatInt &n);//*/% + bool
		
		friend std::ostream	&operator<<(std::ostream &o, const FatInt &f);
};

#endif
