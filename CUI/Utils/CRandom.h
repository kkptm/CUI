#pragma once
#include "defines.h"
#include <vector>
class Random
{
public:
	static int Nest();
	static int Nest(int min, int max);
	static double NextDouble();
	static std::vector<BYTE> NextBytes(int count);
};