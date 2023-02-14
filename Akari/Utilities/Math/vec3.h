
#pragma once

class vec3
{
public:
	union
	{
		float v[3];
		struct { float x, y, z; };
		struct { float pitch, yaw, roll; };
	};

	vec3();
	vec3(float x, float y, float z);
	vec3(float* vec);

	vec3 operator+(const vec3 vec);
	vec3 operator+(const float* vec);
	vec3 operator+(const float value);
	vec3 operator-(const vec3 vec);
	vec3 operator-(const float* vec);
	vec3 operator-(const float value);
	vec3 operator-();
	vec3 operator*(const vec3 vec);
	vec3 operator*(const float* vec);
	vec3 operator*(const float value);
	vec3 operator/(const vec3 vec);
	vec3 operator/(const float* vec);
	vec3 operator/(const float value);

	void operator+=(const vec3 vec);
	void operator+=(const float* vec);
	void operator+=(const float value);
	void operator-=(const vec3 vec);
	void operator-=(const float* vec);
	void operator-=(const float value);
	void operator*=(const vec3 vec);
	void operator*=(const float* vec);
	void operator*=(const float value);
	void operator/=(const vec3 vec);
	void operator/=(const float* vec);
	void operator/=(const float value);

	bool operator==(const vec3 vec);
	bool operator==(const float* vec);
	bool operator==(const float value);
	bool operator!=(const vec3 vec);
	bool operator!=(const float* vec);
	bool operator!=(const float value);

	float& operator[](const int i);

	operator float*();
	operator const float*();

	float magnitude();
	float dot(vec3 vec);
	float distance(vec3 vec);
	vec3& normalize();
	vec3& negate();
	vec3& clamp(float min, float max);
};
