#pragma once
#include <random>

class Random
{
public:

	//Mersenne Twister
	std::mt19937 rng;

	Random(); //constructor that seeds the rng with a std::random_device
	void seedRNG(unsigned int seedVal); //seeds the rng with the seedVal
	int generateNum(int end); // returns the result of an N-sided die roll (between 0 and N);
	int generateNum(int start, int end);
};