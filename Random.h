#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class RandomGenerator {
private:
	static int _seed;

public:
	static void initSeed(int);
	static int getRandomNumberFromRange(int, int);
};



#endif // !RANDOM_H
