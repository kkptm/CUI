#include "CRandom.h"
int Random::Nest()
{
	return rand();
}
int Random::Nest(int min, int max)
{
		return rand() % (max - min + 1) + min;
}
double Random::NextDouble()
{
	return (double)rand() / (double)RAND_MAX;
}
std::vector<BYTE> Random::NextBytes(int count)
{
	std::vector<BYTE> bytes;
	for (int i = 0; i < count; i++)
	{
		bytes.push_back(rand() % 0x100);
	}
	return bytes;
}