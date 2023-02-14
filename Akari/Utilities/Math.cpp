
#include "Math.h"
#include <vsh/stdc.h>

namespace Math
{
	static int snext = 30112004;
	static int max_rand = 0x7fffffff;

	void srand(unsigned int seed)
	{
		snext = seed;
	}

	int __random_r(int* result)
	{
		if (!result)
			return -1;

		snext = ((snext * 1103515245) + 12345) & max_rand;
		*result = snext;

		return 0;
	}

	int rand()
	{
		int result;
		(void)__random_r(&result);
		return result;
	}

	int irand(int min, int max)
	{
		return min + (rand() % static_cast<int>(max - min + 1));
	}

	float frand(float min, float max)
	{
		return min + ((static_cast<float>(rand()) / max_rand) * (max - min));
	}

	double radians(double degrees)
	{
		return degrees * (_M_PI / 180.0);
	}

	double degrees(double radians)
	{
		return radians * (180.0 / _M_PI);
	}
}