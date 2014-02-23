#include <iterator>
#include <chrono>
#include <random>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include "ph.hpp"

int main() {

	std::random_device rnd;

	std::mt19937 gen(rnd());

	std::uniform_int_distribution<> dis('A', 'z');

	char* str = new char[1024];
	std::memset(str, '\0', 1024);

	for(auto it=str; it<str+1023; ++it) {
		*it = dis(gen);
	}

	// test str complete. now for testing.


	{
		auto start = std::chrono::high_resolution_clock::now();

		auto endIterator = str+strlen(str); // O(n)

		auto pos = std::find(str, endIterator, 'a');

		auto end = std::chrono::high_resolution_clock::now();

		std::cout<< "std::find: " << (end - start).count() << std::endl;
	}

	{
		auto start = std::chrono::high_resolution_clock::now();

		auto endIterator = ph::LazyStrIterator{};

		auto pos = ph::find(str, endIterator, 'a');

		auto end = std::chrono::high_resolution_clock::now();

		std::cout<< "ph::find: " << (end - start).count() << std::endl;
	}

	{
		auto start = std::chrono::high_resolution_clock::now();

		auto endIterator = ph::Until([](const char& c) { return c=='A'; });

		auto pos = ph::find(str, endIterator, 'a');

		auto end = std::chrono::high_resolution_clock::now();

		std::cout<< "ph::find with Until: " << (end-start).count() << std::endl;
	}

	{
		auto start = std::chrono::high_resolution_clock::now();

		auto endIterator1 = ph::Until([](const char& c) { return c=='A'; });
		auto endIterator2 = ph::Until([](const char& c) { return c=='B'; });

		auto pos = ph::find(str, endIterator1 && endIterator2, 'a');

		auto end = std::chrono::high_resolution_clock::now();

		std::cout<< "ph::find with two untils: " << (end-start).count() << std::endl;
	}


	std::vector<int> v(1024);
	for(auto& i: v) {
		std::uniform_int_distribution<int> intDis(0, 65536);
		i = intDis(gen);
	}

	// TODO: get rid of this requirement.
	v[1023] = 200;

	{
		auto start = std::chrono::high_resolution_clock::now();

		for(auto it = v.begin(); it != v.end(); ++it) {
			if(*it == 200)
				break;
			if(*it == 100)
				break;
			if(*it == 300)
				break;
		}

		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "vectorFind: " << (end - start).count() << std::endl;

	}

	{
		auto start = std::chrono::high_resolution_clock::now();

		auto endIterator1 = ph::Until([](const int& i) { return i==100; });
		auto endIterator2 = ph::Until([](const int& i) { return i==200; });

		auto endIterator = endIterator1 && endIterator2;

		ph::find(v.begin(), endIterator, 300);

		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "phFind: " << (end - start).count() << std::endl;

	}

}


