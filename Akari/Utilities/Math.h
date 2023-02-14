
#pragma once

#include "Math/Easing.h"
#include "Math/vec2.h"
#include "Math/vec3.h"
#include "Math/vec4.h"

namespace Math
{
	void srand(unsigned int seed);
	int __random_r(int* result);
	int rand();
	int irand(int min, int max);
	float frand(float min, float max);

	double radians(double degrees);
	double degrees(double radians);
}