/*
g++ -std=c++11 -Wextra -pedantic -O3 -lpthread -o walk walk.cpp
*/

#include "timer.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <array>
#include <algorithm>
#include <thread>
#include <chrono>
#include <random>
#include <type_traits>

constexpr unsigned long long UP = 1ULL;
constexpr unsigned long long DOWN = 11ULL;
constexpr unsigned long long K = 101ULL;
constexpr unsigned long long M = 1000000000ULL;
constexpr unsigned long long THREADS = 50ULL;

template <typename IntegerType>
void output(const std::vector<IntegerType>& vec, const char* filename)
{
	static_assert(std::is_integral<IntegerType>::value,
		"Requires integral type.");

	std::ofstream output(filename);
	unsigned int i = 0U;
	for (const auto& x : vec)
	{
		output << i << ',' << static_cast<double>(x) / M << '\n';
		++i;
	}
}

template <typename IntegerType>
void worker_function(std::vector<IntegerType>& vec, int walks, int start, int up, int down)
{
	static_assert(std::is_integral<IntegerType>::value,
		"Requires integral type.");

	auto e = std::default_random_engine(std::random_device()());
	auto d = std::uniform_int_distribution<unsigned int>(0,1);

	int x;
	unsigned int time;
	for (int m = 0; m < walks; ++m)
	{
		// simulate time taken
		time = 0U;
		for (x = start; x > 0; ++time)
		{
			if (d(e))
				x += up;
			else
				x -= down;
		}

		// make space if neccessary
		for (auto t = vec.size(); t <= time; ++t)
			vec.push_back(static_cast<IntegerType>(0));

		// increment at time taken
		++vec.at(time);
	}

	return;
}

template <typename IntegerType>
void walk(const char* filename)
{
	static_assert(std::is_integral<IntegerType>::value,
		"Requires integral type.");

	typedef std::pair<std::thread, std::vector<IntegerType>> worker;
	auto workers = std::array<worker, THREADS>();

	for (auto& w : workers)
	{
		w.first = std::thread(
			worker_function<IntegerType>,
			std::ref(w.second),
			M / THREADS,
			K,
			UP,
			DOWN);
	}

	for (auto& w : workers)
	{
		w.first.join();
	}

	unsigned int max_time = 0U;
	for (auto& w : workers)
	{
		if (w.second.size() > max_time)
			max_time = w.second.size();
	}

	std::vector<IntegerType> histogram(max_time);
	std::fill(histogram.begin(), histogram.end(), static_cast<IntegerType>(0));
	for (auto& w : workers)
	{
		auto h = histogram.begin();
		for (const auto& x : w.second)
		{
			*h += x;
			++h;
		}
	}

	output(histogram, filename);

	return;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Enter output filename as paramter.\n";
		return EXIT_FAILURE;
	}

	auto ns = elapsed_nanoseconds(walk<unsigned int>, argv[1]);

	std::cout << "Total nanoseconds: " << ns << '\n';
	std::cout << "Total seconds: " << (static_cast<double>(ns) / 1000000000) << '\n';
	std::cout << "Average nanoseconds per walk: " << (static_cast<double>(ns) / M) << '\n';

	return EXIT_SUCCESS;
}
