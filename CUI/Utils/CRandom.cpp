#include "CRandom.h"
#include "Utils.h"
static bool isSeeded = false;
static void checkRandomInit()
{
	if (!isSeeded)
	{
		isSeeded = true;
		srand(int(time(NULL)) * (GetTick() & 0xFFFFFFFF));
	}
}
int Random::Nest()
{
	checkRandomInit();
	return rand();
}
int Random::Nest(int min, int max)
{
	checkRandomInit();
	return rand() % (max - min) + min;
}
double Random::NextDouble()
{
	checkRandomInit();
	return (double)(rand() % 20000) / 20000.0;
}
std::vector<BYTE> Random::NextBytes(int count)
{
	checkRandomInit();
	std::vector<BYTE> bytes = std::vector<BYTE>(count);
	bytes.resize(count);
	for (int i = 0; i < count; i++)
	{
		bytes[i] = rand() % 0x100;
	}
	return bytes;
}