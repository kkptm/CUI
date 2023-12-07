#include "CRandom.h"
#include "Utils.h"
int Random::Nest()
{
	return (rand() | (GetTick() & 0xFF));
}
int Random::Nest(int min, int max)
{
		return (rand() | (GetTick() & 0xFF)) % (max - min) + min;
}
double Random::NextDouble()
{
	return (double)(rand() | (GetTick() & 0xFF)) / (double)RAND_MAX;
}
std::vector<BYTE> Random::NextBytes(int count)
{
	std::vector<BYTE> bytes= std::vector<BYTE>(count);
	bytes.resize(count);
	for (int i = 0; i < count; i++)
	{
		bytes[i] = (rand() | (GetTick() & 0xFF)) % 0x100;
	}
	return bytes;
}