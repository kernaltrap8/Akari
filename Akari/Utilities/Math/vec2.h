
#pragma once

class vec2
{
public:
	union
	{
		float v[2];
		struct { float x, y; };
	};

	vec2();
	vec2(float x, float y);
	vec2(float* vec);

	vec2 operator+(const vec2 vec);
	vec2 operator+(const float* vec);
	vec2 operator+(const float value);
	vec2 operator-(const vec2 vec);
	vec2 operator-(const float* vec);
	vec2 operator-(const float value);
	vec2 operator-();
	vec2 operator*(const vec2 vec);
	vec2 operator*(const float* vec);
	vec2 operator*(const float value);
	vec2 operator/(const vec2 vec);
	vec2 operator/(const float* vec);
	vec2 operator/(const float value);

	void operator+=(const vec2 vec);
	void operator+=(const float* vec);
	void operator+=(const float value);
	void operator-=(const vec2 vec);
	void operator-=(const float* vec);
	void operator-=(const float value);
	void operator*=(const vec2 vec);
	void operator*=(const float* vec);
	void operator*=(const float value);
	void operator/=(const vec2 vec);
	void operator/=(const float* vec);
	void operator/=(const float value);

	bool operator==(const vec2 vec);
	bool operator==(const float* vec);
	bool operator==(const float value);
	bool operator!=(const vec2 vec);
	bool operator!=(const float* vec);
	bool operator!=(const float value);

	float& operator[](const int i);

	operator float*();
	operator const float*();

	float magnitude();
	float dot(vec2 vec);
	float distance(vec2 vec);
	vec2& normalize();
	vec2& negate();
	vec2& clamp(float min, float max);
};