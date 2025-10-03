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
		bool					sign;

		static void	uadd_word(FatInt &a, uint32_t b);
		static void	usub_word(FatInt &a, uint32_t b);
		static void	umul_word(FatInt &a, uint32_t b);
		static void	udiv_word(FatInt &num, uint32_t div, uint32_t *mod);
		
		static void	uadd(FatInt &dst, const FatInt &a, const FatInt &b);
		static void	sub(FatInt &dst, const FatInt &a, const FatInt &b);
		static void	umul_naive(FatInt &dst, const FatInt &a, const FatInt &b);
		
		inline bool	is_zero() const {return words.size() == 1 && words[0] == 0;}
		
		void	trim();

	public:
		FatInt(int64_t n);
		FatInt(const std::string &s);
		FatInt(const FatInt &f);
		FatInt(FatInt &&f);

		void	operator=(const FatInt &n);

		FatInt	&operator++();
		FatInt	&operator--();
		//needs to make a copy -> garbage
		FatInt	operator++(int) = delete;
		FatInt	operator--(int) = delete;

		FatInt	operator+() const;//useless
		FatInt	operator-() const;
		void	invert();

		explicit	operator bool() const;
		bool		operator!();
		bool		operator&&(bool b) const;
		bool		operator||(bool b) const;
		
		FatInt	operator~() const;
		void	flip();
		//FatInt	operator>>(uint64_t n) const;
		//FatInt	operator<<(uint64_t n) const;
		//FatInt	operator&(const FatInt &f) const;
		//FatInt	operator^(const FatInt &f) const;
		//FatInt	operator|(const FatInt &f) const;
		
		//+= -= *= /= %= <<= >>= &= ^= |=
		//op to flip the sign ? abs ?
		FatInt	operator+(const FatInt &n) const;
		FatInt	operator-(const FatInt &n) const;
		FatInt	operator*(const FatInt &n) const;
		FatInt	operator/(const FatInt &n) const;
		FatInt	operator%(const FatInt &n) const;

		bool	operator==(const FatInt &n) const;
		bool	operator!=(const FatInt &n) const;
		bool	operator>=(const FatInt &n) const;
		bool	operator>(const FatInt &n) const;
		bool	operator<=(const FatInt &n) const;
		bool	operator<(const FatInt &n) const;
		//FatInt	operator-(const FatInt &n);//*/% + bool

		static std::string	to_string(const FatInt &f);
		
		friend std::ostream	&operator<<(std::ostream &o, const FatInt &f);
};

#endif
