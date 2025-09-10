#include <iostream>
#include "FatInt.hpp"

int	main()
{
	FatInt	a(2);
	FatInt	b(9223372036854775807);
	FatInt	c(1);

	std::cout << a << " + " << c << " = " << a + c << "\n";
	std::cout << b << " + " << c << " = " << b + c << "\n";
	std::cout << b << " + " << b  + 2 << " = " << b + b + 2 << "\n";
	std::cout << b << " + " << b + 1 << " = " << b + b + 1 << "\n";
	std::cout << FatInt(3e9) << " + " << FatInt(2e9) << " = " << FatInt(3e9) + FatInt(2e9) << "\n";
}
