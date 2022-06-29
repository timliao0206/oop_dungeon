#include "Random.h"

int RandomGenerator::_seed = 0;

void RandomGenerator::initSeed(int seed) {
	srand(seed);

	_seed = seed;
}

int RandomGenerator::getRandomNumberFromRange(int min, int max) {
	int n = rand() % (max - min + 1) + min;

	return n;
}