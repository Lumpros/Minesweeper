#include "RNG.h"

#include <random>

uint16_t RNG::RandUInt16(uint16_t min, uint16_t max)
{
	if (min > max)
		std::swap(min, max);

	static std::random_device rdevice;
	static std::mt19937 generator(rdevice());

	std::uniform_int_distribution<uint16_t> distribution(min, max);

	return distribution(generator);
}