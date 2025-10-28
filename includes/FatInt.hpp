#ifndef FATINT_HPP
# define FATINT_HPP

#include <vector>
#include <ostream>
#include <cstdint>
#include <string>

//#define FATINT_DEBUG
//#define FATINT_YES_MINUS

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

		static void	uand(FatInt &a, const FatInt &b);
		static void	uor(FatInt &a, const FatInt &b);
		static void	uxor(FatInt &a, const FatInt &b);
		static void	ushift_right(FatInt &f, uint64_t n);
		static void	ushift_left(FatInt &f, uint64_t n);
		
		static void	uadd(FatInt &a, const FatInt &b);
		static void	sub(FatInt &a, const FatInt &b);
		static void	umul(FatInt &a, const FatInt &b);
		
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
		//would need to make a copy with current implementation -> bad
		FatInt	operator++(int) = delete;
		FatInt	operator--(int) = delete;

		FatInt	operator+() const;//useless but why not
		FatInt	operator-() const;
		void	invert();

		explicit	operator bool() const;
		bool		operator!();
		bool		operator&&(bool b) const;
		bool		operator||(bool b) const;
		
		FatInt	operator~() const;
		void	flip();
		FatInt	operator>>(const FatInt &f) const;
		FatInt	operator<<(const FatInt &f) const;
		FatInt	operator&(const FatInt &f) const;
		FatInt	operator|(const FatInt &f) const;
		FatInt	operator^(const FatInt &f) const;
		void	operator&=(const FatInt &f);
		void	operator|=(const FatInt &f);
		void	operator^=(const FatInt &f);
		void	operator>>=(const FatInt &f);
		void	operator<<=(const FatInt &f);
		
		FatInt	operator+(const FatInt &n) const;
		FatInt	operator-(const FatInt &n) const;
		FatInt	operator*(const FatInt &n) const;
		FatInt	operator/(const FatInt &n) const;
		FatInt	operator%(const FatInt &n) const;
		void	operator+=(const FatInt &n);
		void	operator-=(const FatInt &n);
		void	operator*=(const FatInt &n);
		void	operator/=(const FatInt &n);
		void	operator%=(const FatInt &n);

		bool	operator==(const FatInt &n) const;
		bool	operator!=(const FatInt &n) const;
		bool	operator>=(const FatInt &n) const;
		bool	operator>(const FatInt &n) const;
		bool	operator<=(const FatInt &n) const;
		bool	operator<(const FatInt &n) const;

		static std::string	to_string(const FatInt &f);
		static std::string	to_string(const FatInt &f, unsigned base);
		static std::string	to_string(const FatInt &f, const std::string &base);
		
		friend std::ostream	&operator<<(std::ostream &o, const FatInt &f);
};

#endif
