#ifndef FATINT_HPP
# define FATINT_HPP

#include <cstdint>
#include <string>

class	FatInt
{
	private:
		union
		{
			struct
			{
				uint64_t	*start;
				uint64_t	*end;
				uint64_t	*memend;
			};
			struct
			{
				uint64_t	pad[2];
				uint64_t	num;
			}
		};

	public:
		FatInt();
		FatInt(uint64_t n);
		FatInt(int64_t n);
		FatInt(const std::string &s);//move copy etc

		//operator=(const FatInt &n);
		FatInt	operator+(const FatInt &n);
		FatInt	operator-(const FatInt &n);//*/% + bool
		
		~FatInt();
};

#endif
