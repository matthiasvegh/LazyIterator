#include <cassert>
#include <iterator>
#include <chrono>
#include <random>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include "ph.hpp"
#include "algorithm.hpp"

int main() {

	std::random_device rnd;

	std::mt19937 gen(rnd());

	std::uniform_int_distribution<> dis('A', 'z');

	char* str = new char[1024];
	std::memset(str, '\0', 1024);

	for(auto it=str; it<str+1023; ++it) {
		*it = dis(gen);
	}

	const int Size=6400000;
	std::vector<int> v(Size);
	for(auto& i: v) {
		std::uniform_int_distribution<int> intDis(0, Size/2);
		i = intDis(gen);
	}

	const int runs = 10000;

	// test vars complete. now for testing.

	for(int run=1; run <= runs; ++run) {

		{
			auto start = std::chrono::high_resolution_clock::now();

			auto endIterator = str+strlen(str); // O(n)

			auto pos = std::find(str, endIterator, 'a');

			auto end = std::chrono::high_resolution_clock::now();

			//std::cout<< "std::find: " << (end - start).count() << std::endl;
		}

		{
			auto start = std::chrono::high_resolution_clock::now();

			auto endIterator = ph::LazyStrIterator{};

			auto pos = ph::find(str, endIterator, 'a');

			auto end = std::chrono::high_resolution_clock::now();

			//std::cout<< "ph::find: " << (end - start).count() << std::endl;
		}

		{
			auto start = std::chrono::high_resolution_clock::now();

			auto endIterator = ph::until([](const char& c) { return c=='A'; });

			auto pos = ph::find(str, endIterator, 'a');

			auto end = std::chrono::high_resolution_clock::now();

			//std::cout<< "ph::find with until: " << (end-start).count() << std::endl;
		}

		{
			auto start = std::chrono::high_resolution_clock::now();

			auto endIterator1 = ph::until([](const char& c) { return c=='A'; });
			auto endIterator2 = ph::until([](const char& c) { return c=='B'; });

			auto pos = ph::find(str, endIterator1 || endIterator2, 'a');

			auto end = std::chrono::high_resolution_clock::now();

			//std::cout<< "ph::find with two untils: " << (end-start).count() << std::endl;
		}


		const int until1 = 100;
		const int until2 = 200;

		{
			auto start = std::chrono::high_resolution_clock::now();

			auto it = v.begin();
			for(; it != v.end(); ++it) {
				if(*it == until1)
					break;
				if(*it == until2)
					break;
				if(*it == 300)
					break;
			}

			assert(it != v.end());

			const auto value = *it;

			assert(value == until1 || value == until2 || value == 300);

			auto end = std::chrono::high_resolution_clock::now();
			//std::cout << "vectorFind: " << (end - start).count() << std::endl;

		}

		{
			auto start = std::chrono::high_resolution_clock::now();

			auto endIterator1 = ph::until([until1](const int& i) { return i==until1; });
			auto endIterator2 = ph::until([until2](const int& i) { return i==until2; });

			auto endIterator = endIterator1 || endIterator2;

			auto it = ph::find(v.begin(), endIterator || v.end(), 300);

			assert(it != v.end());

			const auto value = *it;
			assert(value == until1 || value == until2 || value == 300);

			auto end = std::chrono::high_resolution_clock::now();
			//std::cout << "phFind: " << (end - start).count() << std::endl;

		}

		{
			auto start = std::chrono::high_resolution_clock::now();


			auto endIterator = ph::untilValue(100, 200);

			auto it = ph::find(v.begin(), endIterator || v.end(), 300);

			assert(it != v.end());

			const auto value = *it;
			assert(value == until1 || value == until2 || value == 300);

			auto end = std::chrono::high_resolution_clock::now();
			//std::cout << "multiVal phFind: " << (end - start).count() << std::endl;

		}

		{
			auto start = std::chrono::high_resolution_clock::now();

			auto endIterator = std::find(v.begin(), v.end(), until1);

			auto count = std::count(v.begin(), endIterator, until2);


			auto end = std::chrono::high_resolution_clock::now();
			//std::cout << "std::count until until1: " << (end - start).count() << std::endl;

		}

		{
			auto start = std::chrono::high_resolution_clock::now();

			auto endIterator = ph::until([until1](const int& i) { return i==until1; });

			auto count = ph::count(v.begin(), endIterator, until2);

			auto end = std::chrono::high_resolution_clock::now();
			//std::cout << "phCount until until1: " << (end - start).count() << std::endl;

		}

	} // for run <= runs.

}


