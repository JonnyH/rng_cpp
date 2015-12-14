#include <random>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "xorshift.h"

#define SEED 1
#define RUN_LENGTH 100

using namespace xorshift_rng;
template <class T> int test()
{
	xorshift<T> rng{SEED};

	std::cout << std::hex << "[0]:\t0x" << rng() << "\n[1]:\t0x" << rng() << "\n";

	unsigned i = 1;
	for (; i < RUN_LENGTH; i++)
	{
		rng();
	}

	std::cout << "[" << std::dec << i << std::hex << ":]\t0x" << rng() << "\n";
	std::cout << "state:\n";
	// state must be dec | left and fill ' ' (space)
	std::cout << std::dec << std::left << std::setfill(' ');
	std::cout << rng << "\n";

	std::uniform_int_distribution<int> dist(0, 99);
	std::cout << "uniform int 0-99\n";
	std::cout << dist(rng) << "\t" << dist(rng) << "\t" << dist(rng) << "\n";

	std::stringstream ss;
	ss << rng;
	std::cout << "stringstream state:\n" << ss.str() << "\n";
	std::stringstream ss2(ss.str());

	T state[2];
	T state2[2];
	T state3[2];

	rng.get_state(state);

	xorshift<T> rng2{};

	ss2 >> rng2;

	rng.get_state(state2);

	xorshift<T> rng3{state2};
	rng3.get_state(state3);

	std::cout << "state1:\t" << state[0] << " " << state[1] << "\n";
	std::cout << "state2:\t" << state2[0] << " " << state2[1] << "\n";
	std::cout << "state3:\t" << state3[0] << " " << state3[1] << "\n";

	if (state[0] != state2[0] || state[0] != state3[0])
	{
		std::cerr << "Invalid state[0]\n";
		return 1;
	}
	if (state[1] != state2[1] || state[1] != state3[1])
	{
		std::cerr << "Invalid state[1]\n";
		return 1;
	}

	return 0;
}
int main(int argc, char **argv)
{
	int ret = test<uint32_t>();
	if (ret)
	{
		std::cerr << "uint32_t test failed\n";
		return ret;
	}
	ret = test<uint64_t>();
	if (ret)
	{
		std::cerr << "uint64_t test failed\n";
		return ret;
	}
	return 0;
}
