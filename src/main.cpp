#include <iostream>
#include "FatInt.hpp"

int	main()
{
	FatInt	a(2);
	FatInt	b(9223372036854775807);
	FatInt	c(1);

	std::cout << a << " + " << c << " = " << a + c << "\n";
}
