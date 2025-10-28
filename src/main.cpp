#include <iostream>
#include <vector>
#include "FatInt.hpp"
#include "FatIntTest.hpp"

std::vector<FatIntTest>	tests;

void	runTest(int n = -1)
{
	if (n == -1)
	{
		for (size_t i = 0; i < tests.size(); ++i)
		{
			std::cout << "TEST " << i << " " << tests[i].a << " " << tests[i].b << "\n";
			tests[i].run();
			std::cout << "\n";
		}
	}
	else
	{
		std::cout << "TEST " << n << " " << tests[n].a << " " << tests[n].b << "\n";
		tests[n].run();
		std::cout << "\n";
	}
}

void	addTest(const FatInt &a, const FatInt &b)
{
	tests.push_back(FatIntTest(a, b));
}

void	mmmm_tests()
{
	FatInt	maxint32(9223372036854775807);//this is the maxint64
	FatInt	maxint322(static_cast<uint64_t>(9223372036854775809ULL));
	FatInt	big("12345678912345678998745632198765432155544462215");
	
	addTest(0, 0);
	addTest(FatInt("-0"), 0);
	addTest(0, FatInt("-0"));
	addTest(FatInt("-0"), FatInt("-0"));

	addTest(0, 1);
	addTest(1, 0);
	addTest(1, 1);
	addTest(-1, 1);
	addTest(1, -1);
	addTest(-1, -1);

	addTest(1, 34);
	tests.rbegin()->flags.rshift  = true;
	tests.rbegin()->flags.lshift  = true;
	addTest(FatInt("111111111111111111111111"), -34);
	tests.rbegin()->flags.rshift  = true;
	tests.rbegin()->flags.lshift  = true;
	addTest(-1, -3);
	tests.rbegin()->flags.rshift  = true;
	tests.rbegin()->flags.lshift  = true;
	addTest(FatInt("-111111111111111111111111"), 3);
	tests.rbegin()->flags.rshift  = true;
	tests.rbegin()->flags.lshift  = true;
	addTest(FatInt("123456789123456789123456789"), 12345);
	tests.rbegin()->flags.rshift  = true;
	tests.rbegin()->flags.lshift  = true;
	addTest(FatInt("123456789123456789123456789"), -123456);
	tests.rbegin()->flags.rshift  = true;
	tests.rbegin()->flags.lshift  = true;

	addTest(2 | 64 | (2ULL << 62), 1 | 2 | (2ULL << 62));
	addTest(FatInt("295811224608098629060044695716103590786339687135372992239556207050657350796238924261053837248378050186443647759070955993120820899330381760937027212482840944941362110665443775183495726811929203861182015218323892077355983393191208928867652655993602487903113708549402668624521100611794270340232766099317098048887493809023127398253860618783252858976162599942806745196064145410"), 0);
	addTest(FatInt("295811224608098629060044695716103590786339687135372992239556207050657350796238924261053837248378050186443647759070955993120820899330381760937027212482840944941362110665443775183495726811929203861182015218323892077355983393191208928867652655993602487903113708549402668624521100611794270340232766099317098048887493809023127398253860618783252858976162599942806745196064145410"), FatInt("295811224608098629060044695716103590786339687135372992239556207050657350796238924261053837248378050186443647759070955993120820899330381760937027212482840944941362110665443775183495726811929203861182015218323892077355983393191208928867652655993602487903113708549402668624521100611794270340232766099317098048887493809023127398253860618772619035009883272959576288713821388801"));

	addTest(3, 1);
	addTest(-3, 1);
	addTest(3, -1);
	addTest(-3, -1);
	addTest(maxint32, maxint32);
	addTest(maxint32, -maxint32);
	addTest(-maxint32, maxint32);
	addTest(-maxint32, -maxint32);
	addTest(maxint322, maxint32);
	addTest(maxint322, -maxint32);
	addTest(-maxint322, maxint32);
	addTest(-maxint322, -maxint32);
	addTest(maxint32, maxint322);
	addTest(maxint32, -maxint322);
	addTest(-maxint32, maxint322);
	addTest(-maxint32, -maxint322);
	addTest(3e9, 2e9);
	addTest(big, 2e9);
	addTest(big, -2e9);
	addTest(-big, 2e9);
	addTest(-big, -2e9);
	addTest(2e9, big);
	addTest(-2e9, big);
	addTest(2e9, -big);
	addTest(-2e9, -big);
	addTest(FatInt("18446744073709551615"), 1);
	addTest(FatInt("-18446744073709551615"), 1);
	addTest(FatInt("18446744073709551615"), -1);
	addTest(FatInt("-18446744073709551615"), -1);
	addTest(1, FatInt("18446744073709551615"));
	addTest(-1, FatInt("18446744073709551615"));
	addTest(1, -FatInt("18446744073709551615"));
	addTest(-1, -FatInt("18446744073709551615"));
	addTest(big, FatInt("18446744073709551615"));
	addTest(-big, FatInt("18446744073709551615"));
	addTest(big, -FatInt("18446744073709551615"));
	addTest(-big, -FatInt("18446744073709551615"));
	addTest(FatInt("18446744073709551615"), big);
	addTest(FatInt("-18446744073709551615"), big);
	addTest(FatInt("18446744073709551615"), -big);
	addTest(FatInt("-18446744073709551615"), -big);
}

//TODO add basic mul div remain
//TODO hex constructor ?
//TODO opt for large numbers
//TODO better string converter (no precompute + better string management)
//TODO better tester
int	main(int argc, char **argv)
{
	mmmm_tests();
	if (argc == 1)
		runTest();
	for (int i = 1; i < argc; ++i)
		runTest(std::strtol(argv[i], NULL, 0));	
}
