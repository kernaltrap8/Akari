
#include "vec3.h"
#include <fastmath.h>

vec3::vec3()
	: x(0), y(0), z(0) {}

vec3::vec3(float x, float y, float z)
	: x(x), y(y), z(z) {}

vec3::vec3(float* vec)
{
	if (vec)
		x = vec[0], y = vec[1], z = vec[2];
	else
		*this = vec3();
}

vec3 vec3::operator+(const vec3 vec)
{
	return vec3(x + vec.x, y + vec.y, z + vec.z);
}

vec3 vec3::operator+(const float* vec)
{
	return vec3(x + vec[0], y + vec[1], z + vec[2]);
}

vec3 vec3::operator+(const float value)
{
	return vec3(x + value, y + value, z + value);
}

vec3 vec3::operator-(const vec3 vec)
{
	return vec3(x - vec.x, y - vec.y, z - vec.z);
}

vec3 vec3::operator-(const float* vec)
{
	return vec3(x - vec[0], y - vec[1], z - vec[2]);
}

vec3 vec3::operator-(const float value)
{
	return vec3(x - value, y - value, z - value);
}

vec3 vec3::operator-()
{
	return vec3(-x, -y, -z);
}

vec3 vec3::operator*(const vec3 vec)
{
	return vec3(x * vec.x, y * vec.y, z * vec.z);
}

vec3 vec3::operator*(const float* vec)
{
	return vec3(x * vec[0], y * vec[1], z * vec[2]);
}

vec3 vec3::operator*(const float value)
{
	return vec3(x * value, y * value, z * value);
}

vec3 vec3::operator/(const vec3 vec)
{
	return vec3(x / vec.x, y / vec.y, z / vec.z);
}

vec3 vec3::operator/(const float* vec)
{
	return vec3(x / vec[0], y / vec[1], z / vec[2]);
}

vec3 vec3::operator/(const float value)
{
	return vec3(x / value, y / value, z / value);
}

void vec3::operator+=(const vec3 vec)
{
	x += vec.x, y += vec.y, z += vec.z;
}

void vec3::operator+=(const float* vec)
{
	x += vec[0], y += vec[1], z += vec[2];
}

void vec3::operator+=(const float value)
{
	x += value, y += value, z += value;
}

void vec3::operator-=(const vec3 vec)
{
	x -= vec.x, y -= vec.y, z -= vec.z;
}

void vec3::operator-=(const float* vec)
{
	x -= vec[0], y -= vec[1], z -= vec[2];
}

void vec3::operator-=(const float value)
{
	x -= value, y -= value, z -= value;
}

void vec3::operator*=(const vec3 vec)
{
	x *= vec.x, y *= vec.y, z *= vec.z;
}

void vec3::operator*=(const float* vec)
{
	x *= vec[0], y *= vec[1], z *= vec[2];
}

void vec3::operator*=(const float value)
{
	x *= value, y *= value, z *= value;
}

void vec3::operator/=(const vec3 vec)
{
	x /= vec.x, y /= vec.y, z /= vec.z;
}

void vec3::operator/=(const float* vec)
{
	x /= vec[0], y /= vec[1], z /= vec[2];
}

void vec3::operator/=(const float value)
{
	x /= value, y /= value, z /= value;
}

bool vec3::operator==(const vec3 vec)
{
	return x == vec.x && y == vec.y && z == vec.z;
}

bool vec3::operator==(const float* vec)
{
	return x == vec[0] && y == vec[1] && z == vec[2];
}

bool vec3::operator==(const float value)
{
	return x == value && y == value && z == value;
}

bool vec3::operator!=(const vec3 vec)
{
	return x != vec.x || y != vec.y || z != vec.z;
}

bool vec3::operator!=(const float* vec)
{
	return x != vec[0] || y != vec[1] || z != vec[2];
}

bool vec3::operator!=(const float value)
{
	return x != value || y != value || z != value;
}

float& vec3::operator[](const int i)
{
	return v[i];
}

vec3::operator float*()
{
	return (float*)&this[0];
}

vec3::operator const float*()
{
	return (const float*)&this[0];
}

float vec3::magnitude()
{
	return std::f_sqrtf(dot(*this));
}

float vec3::dot(vec3 vec)
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

float vec3::distance(vec3 vec)
{
	vec3 diff = *this - vec;
	return diff.magnitude();
}

vec3& vec3::normalize()
{
	*this /= magnitude();
	return *this;
}

vec3& vec3::negate()
{
	*this = -*this;
	return *this;
}

vec3& vec3::clamp(float min, float max)
{
	x = (x > max) ? max : (x < min) ? min : x;
	y = (y > max) ? max : (y < min) ? min : y;
	z = (z > max) ? max : (z < min) ? min : z;
	return *this;
}