#ifndef FATINTTEST_HPP
# define FATINTTEST_HPP

#include "FatInt.hpp"

struct	FatIntTest
{
	FatInt	a;
	FatInt	b;
	struct
	{
		bool	preop = true;
		bool	unary = true;
		bool	boolop = true;
		bool	rshift = false;
		bool	lshift = false;
		bool	bitop = true;
		bool	add = true;
		bool	sub = true;
		bool	mul = true;
		bool	div = true;
		bool	remain = true;
		bool	cmp = true;
		bool	string = true;
	}	flags;

	FatIntTest(const FatInt &a, const FatInt &b);

	void	run_preop();
	void	run_unary();
	void	run_boolop();
	void	run_rshift();
	void	run_lshift();
	void	run_bitop();
	void	run_add();
	void	run_sub();
	void	run_mul();
	void	run_div();
	void	run_remain();
	void	run_cmp();
	void	run_string();
	void	run();
};

#endif
