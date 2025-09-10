#include <iostream>
#include <vector>
#include "FatInt.hpp"

std::vector<std::pair<FatInt, FatInt>>	tests;

void	testAddSub(const FatInt &a, const FatInt &b)
{
	std::cout << "add\n";
	std::cout << a << " + " << b << " = " << a + b << "\n";
	std::cout << a << " + " << -b << " = " << a + -b << "\n";
	std::cout << -a << " + " << b << " = " << -a + b << "\n";
	std::cout << -a << " + " << -b << " = " << -a + -b << "\n";
	std::cout << "sub\n";
	std::cout << a << " - " << b << " = " << a - b << "\n";
	std::cout << a << " - " << -b << " = " << a - -b << "\n";
	std::cout << -a << " - " << b << " = " << -a - b << "\n";
	std::cout << -a << " - " << -b << " = " << -a - -b << "\n";
	std::cout << "sub rev\n";
	std::cout << b << " - " << a << " = " << b - a << "\n";
	std::cout << b << " - " << -a << " = " << b - -a << "\n";
	std::cout << -b << " - " << a << " = " << -b - a << "\n";
	std::cout << -b << " - " << -a << " = " << -b - -a << "\n";
	std::cout << "\n";
}

void	addTest(const FatInt &a, const FatInt &b)
{
	tests.push_back(std::make_pair(a, b));
}

void	runTest(int n = -1)
{
	if (n == -1)
	{
		for (size_t i = 0; i < tests.size(); ++i)
		{
			std::cout << "TEST " << i << "\n";
			testAddSub(tests[i].first, tests[i].second);
		}
	}
	else
	{
		std::cout << "TEST " << n << "\n";
		testAddSub(tests[n].first, tests[n].second);
	}
}

int	main(int argc, char **argv)
{
	FatInt	a(3);
	FatInt	b(9223372036854775807);
	FatInt	c(1);

	addTest(a, c);
	addTest(b, c);
	addTest(b, b);
	addTest(b, b + 2);
	addTest(b, b + 1);
	addTest(3e9, 2e9);
	if (argc == 1)
		runTest();
	for (int i = 1; i < argc; ++i)
		runTest(std::strtol(argv[i], NULL, 0));
		
}
