#include "Random.h"

//constructor that seeds the rng with a std::random_device
Random::Random()
{
	//Creates a random device
	std::random_device rd;

	//Generates the mersenne twister seed using the random device
	rng.seed(rd());
}

//Seeds the rng with a passed in seed
void Random::seedRNG(unsigned int seedVal)
{
	//seeds the rng with the seedVal
	rng.seed(seedVal);
}

int Random::generateNum(int end)
{
	//rolls an N-sided die (between 0 and end)
	std::uniform_int_distribution<unsigned int> dN(0, end);

	//returns the result of the die roll
	return dN(rng);
}

int Random::generateNum(int start, int end)
{
	//rolls an N-sided die (between start and end)
	std::uniform_int_distribution<unsigned int> dN(start, end);

	//returns the result of the die roll
	return dN(rng);
}