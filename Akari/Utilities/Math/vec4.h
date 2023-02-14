
#pragma once

class vec4
{
public:
	union
	{
		float v[4];
		struct { float x, y, z, w; };
		struct { float pitch, yaw, roll; };
		struct { float r, g, b, a; };
	};

	vec4();
	vec4(float x, float y, float z, float w);
	vec4(float* vec);

	vec4 operator+(const vec4 vec);
	vec4 operator+(const float* vec);
	vec4 operator+(const float value);
	vec4 operator-(const vec4 vec);
	vec4 operator-(const float* vec);
	vec4 operator-(const float value);
	vec4 operator-();
	vec4 operator*(const vec4 vec);
	vec4 operator*(const float* vec);
	vec4 operator*(const float value);
	vec4 operator/(const vec4 vec);
	vec4 operator/(const float* vec);
	vec4 operator/(const float value);

	void operator+=(const vec4 vec);
	void operator+=(const float* vec);
	void operator+=(const float value);
	void operator-=(const vec4 vec);
	void operator-=(const float* vec);
	void operator-=(const float value);
	void operator*=(const vec4 vec);
	void operator*=(const float* vec);
	void operator*=(const float value);
	void operator/=(const vec4 vec);
	void operator/=(const float* vec);
	void operator/=(const float value);

	bool operator==(const vec4 vec);
	bool operator==(const float* vec);
	bool operator==(const float value);
	bool operator!=(const vec4 vec);
	bool operator!=(const float* vec);
	bool operator!=(const float value);

	float& operator[](const int i);

	operator float*();
	operator const float*();

	float magnitude();
	float dot(vec4 vec);
	float distance(vec4 vec);
	vec4& normalize();
	vec4& negate();
	vec4& clamp(float min, float max);
};

using Color = vec4;