#include <iostream>
#include "FatIntTest.hpp"

FatIntTest::FatIntTest(const FatInt &a, const FatInt &b) :
	a(a),
	b(b)
{
}

void	FatIntTest::run_preop()
{
	FatInt	cpy = a;
	std::cout << "preop " << cpy << " " << ++cpy << " " << --cpy << " " << --cpy << " " << ++cpy << "\n";
}

void	FatIntTest::FatIntTest::run_unary()
{
	FatInt	cpy = a;
	std::cout << "unary " << a << " " << +a << " " << -a << " ";
	cpy.invert();
	std::cout << cpy << "\n";

}

void	FatIntTest::run_boolop()
{
	std::cout << "boolop " << static_cast<bool>(a) << " " << static_cast<bool>(b)
		<< " " << !a << " " << !b << " " << (a && b) << " " << (a || b) << "\n";
}

void	FatIntTest::run_rshift()
{
	FatInt	cpy = a;
	std::cout << "rshift " << a << " " << b << " " << (a >> b) << " ";
	cpy >>= b;
	std::cout << cpy << "\n";
}

void	FatIntTest::run_lshift()
{
	FatInt	cpy = a;
	std::cout << "lshift " << a << " " << b << " " << (a << b) << " ";
	cpy <<= b;
	std::cout << cpy << "\n";
}

void	FatIntTest::run_bitop()
{
	FatInt	cpy = a;
	std::cout << "bitop " << a << " " << b << " " << ~a << " "
		<< (a & b) << " " << (a | b) << " " << (a ^ b) << " ";
	cpy.flip();
	std::cout << cpy << " ";
	cpy = a;
	cpy &= b;
	std::cout << cpy << " ";
	cpy = a;
	cpy |= b;
	std::cout << cpy << " ";
	cpy = a;
	cpy ^= b;
	std::cout << cpy << "\n";
}

void	FatIntTest::run_add()
{
	FatInt	cpy = a;
	std::cout << "add " << a << " " << b << " " << (a + b) << " ";
	cpy += b;
	std::cout << cpy << "\n";
}

void	FatIntTest::run_sub()
{
	FatInt	cpy = a;
	std::cout << "sub " << a << " " << b << " " << (a - b) << " ";
	cpy -= b;
	std::cout << cpy << "\n";
}

void	FatIntTest::run_mul()
{
	FatInt	cpy = a;
	std::cout << "mul " << a << " " << b << " " << (a * b) << " ";
	cpy *= b;
	std::cout << cpy << "\n";
}

void	FatIntTest::run_div()
{
	FatInt	cpy = a;
	std::cout << "div " << a << " " << b << " " << (a / b) << " ";
	cpy /= b;
	std::cout << cpy << "\n";
}

void	FatIntTest::run_remain()
{
	FatInt	cpy = a;
	std::cout << "remain " << a << " " << b << " " << (a % b) << " ";
	cpy %= b;
	std::cout << cpy << "\n";
}

void	FatIntTest::run_cmp()
{
	std::cout << "cmp " << a << " " << b << " " << (a == b) << " " << (a != b) << " "
		<< (a >= b) << " " << (a > b) << " " << (a <= b) << " " << (a < b) << "\n";
}

void	FatIntTest::run_string()
{
	std::cout << "string " << a
		<< " " << FatInt::to_string(a, 2)
		<< " " << FatInt::to_string(a, 3)
		<< " " << FatInt::to_string(a, 4)
		<< " " << FatInt::to_string(a, 5)
		<< " " << FatInt::to_string(a, 6)
		<< " " << FatInt::to_string(a, 7)
		<< " " << FatInt::to_string(a, 8)
		<< " " << FatInt::to_string(a, 9)
		<< " " << FatInt::to_string(a, 10)
		<< " " << FatInt::to_string(a, 11)
		<< " " << FatInt::to_string(a, 12)
		<< " " << FatInt::to_string(a, 13)
		<< " " << FatInt::to_string(a, 14)
		<< " " << FatInt::to_string(a, 15)
		<< " " << FatInt::to_string(a, 16) << "\n";
	//	<< " " << FatInt::to_string(a, "skib|d1amonguS5IGMA")
	//	<< " " << FatInt::to_string(a, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
}

void	FatIntTest::run()
{
	if (flags.preop)
		run_preop();
	if (flags.unary)
		run_unary();
	if (flags.boolop)
		run_boolop();
	if (flags.lshift)
		run_lshift();
	if (flags.rshift)
		run_rshift();
	if (flags.bitop)
		run_bitop();
	if (flags.add)
		run_add();
	if (flags.sub)
		run_sub();
	if (flags.mul)
		run_mul();
	if (flags.div)
		run_div();
	if (flags.remain)
		run_remain();
	if (flags.cmp)
		run_cmp();
	if (flags.string)
		run_string();	
}
