import sys
from termcolor import colored

#TODO: make this tester less bad at some point
sys.set_int_max_str_digits(100000)

test = 0

def	fake_not(n : int):
	nwords = 1
	while (abs(n) > 2 ** (nwords * 32)):
		nwords += 1
	if (n < 0):
		return 2 ** (nwords * 32) * -1 + abs(n) + 1
	return 2 ** (nwords * 32) - abs(n) - 1

def	fake_and(a : int, b : int):
	if (a < 0 and b < 0):
		return -(abs(a) & abs(b))
	return abs(a) & abs(b)

def	fake_or(a : int, b : int):
	if (a < 0 or b < 0):
		return -(abs(a) | abs(b))
	return abs(a) | abs(b)

def	fake_xor(a : int, b : int):
	if ((a < 0) ^ (b < 0)):
		return -(abs(a) ^ abs(b))
	return abs(a) ^ abs(b)

def ntobase(n : int, base : str):
	if (n == 0):
		return str(base[0])
	neg = n < 0
	numstr = ""
	if (neg):
		n = -n
	lenbase = len(base)
	while (n):
		numstr = str(base[n % lenbase]) + numstr
		n //= lenbase
	if (neg):
		numstr = "-" + numstr
	return numstr

for line in sys.stdin:
	words = line.split()
	if len(words) == 0:
		continue
	if (len(sys.argv) > 1 and words[0] not in sys.argv and words[0] != "TEST"):
		continue
	match words[0]:
		case "TEST":
			if len(words) != 4:
				print(colored("TEST: wrong number of args", "red"))
				test = -1
				continue
			test = int(words[1])
			print("TEST", words[1] + ":", words[2], words[3])

		case "preop":
			print("preop: ", end='')
			if len(words) != 6:
				print(colored("wrong number of args", "red"))
				continue
			num = int(words[1])
			expected = [num + 1, num, num - 1, num]
			if ([int(x) for x in words[2::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[2::])
			else:
				print(colored("OK", "green"))
		
		case "unary":
			print("unary: ", end='')
			if len(words) != 5:
				print(colored("wrong number of args", "red"))
				continue
			num = int(words[1])
			expected = [num, -num, -num]
			if ([int(x) for x in words[2::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[2::])
			else:
				print(colored("OK", "green"))
		
		case "boolop":
			print("boolop: ", end='')
			if len(words) != 7:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			expected = [int(not num1), int(not num2), int(num1 and num2), int(num1 or num2)]
			if ([int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))
		
		case "bitop":
			print("bitop: ", end='')
			if len(words) != 11:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			expected = [fake_not(num1), fake_and(num1, num2), fake_or(num1, num2), fake_xor(num1, num2)] * 2
			if ([int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))

		case "lshift":
			print("lshift: ", end='')
			if len(words) != 5:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			if (num2 >= 0):
				expected = [abs(num1) << num2]
			else:
				expected = [abs(num1) >> -num2]
			if (num1 < 0):
				expected[0] *= -1
			expected *= 2
			if ([int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))

		case "rshift":
			print("rshift: ", end='')
			if len(words) != 5:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			if (num2 >= 0):
				expected = [abs(num1) >> num2]
			else:
				expected = [abs(num1) << -num2]
			if (num1 < 0):
				expected[0] *= -1
			expected *= 2
			if ([int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))

		case "cmp":
			print("cmp: ", end='')
			if len(words) != 9:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			expected = [int(num1 == num2), int(num1 != num2), int(num1 >= num2), int(num1 > num2), int(num1 <= num2), int(num1 < num2)]
			if ([int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))
		
		case "string":
			print("string: ", end='')
			if len(words) != 19:
				print(colored("wrong number of args", "red"))
				continue
			num = int(words[1])
			bases = "0123456789ABCDEF"
			expected = [ntobase(num, "".join([bases[y] for y in range(i)])) for i in range(2, 17)]
			expected += [ntobase(num, "skib|d1amonguS5IGMA")]
			expected += [ntobase(num, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")]
			if ([x for x in words[2::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[2::])
			else:
				print(colored("OK", "green"))
		
		case "add":
			print("add: ", end='')
			if len(words) != 5:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			expected = [num1 + num2] * 2
			if ([int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))
		
		case "sub":
			print("sub: ", end='')
			if len(words) != 5:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			expected = [num1 - num2] * 2
			if ([int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))
		
		case "mul":
			print("mul: ", end='')
			if len(words) != 5:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			expected = [num1 * num2] * 2
			if ([int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))
		
		case "div":
			print("div: ", end='')
			if len(words) != 5:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			if (num2):
				expected = [num1 // num2] * 2
			if (num2 != 0 and [int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))
		
		case "remain":
			print("remain: ", end='')
			if len(words) != 5:
				print(colored("wrong number of args", "red"))
				continue
			num1 = int(words[1])
			num2 = int(words[2])
			if (num2):
				expected = [num1 % num2] * 2
			if (num2 != 0 and [int(x) for x in words[3::]] != expected):
				print(colored("wrong:", "red"), "\nexpected", *expected, "\ngot\t", *words[3::])
			else:
				print(colored("OK", "green"))
		
		case _:
			print(colored("Unknown word: " + words[0], "red"))
