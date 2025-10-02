#ifndef FATINT_HPP
# define FATINT_HPP

#include <vector>
#include <ostream>
#include <cstdint>
#include <string>


//todo macro iszero
class	FatInt
{
	private:
		static uint64_t			wordmax;

		FatInt();
	
		std::vector<uint32_t>	words;
		bool					sign;

		static void	uadd_word(FatInt &a, uint32_t b);
		static void	usub_word(FatInt &a, uint32_t b);
		static void	umul_word(FatInt &a, uint32_t b);
		static void	udiv_word(FatInt &num, uint32_t div, uint32_t *mod);

		static void	uadd(FatInt &dst, const FatInt &a, const FatInt &b);
		static void	sub(FatInt &dst, const FatInt &a, const FatInt &b);
		static void	umul_naive(FatInt &dst, const FatInt &a, const FatInt &b);
		
		void		trim();
		inline bool	is_zero() const;

	public:
		FatInt(int64_t n);
		FatInt(const std::string &s);
		FatInt(const FatInt &f);
		FatInt(FatInt &&f);

		void	operator=(const FatInt &n);
		//unary -, ++, etc
		FatInt	operator-() const;
		FatInt	operator~() const;
		void	flip();
		//FatInt	operator~() const;
		FatInt	operator+(const FatInt &n) const;
		FatInt	operator-(const FatInt &n) const;
		FatInt	operator*(const FatInt &n) const;
		FatInt	operator/(const FatInt &n) const;
		FatInt	operator%(const FatInt &n) const;
		//FatInt	operator-(const FatInt &n);//*/% + bool

		static std::string	to_string(const FatInt &f);
		
		friend std::ostream	&operator<<(std::ostream &o, const FatInt &f);
};

#endif
