
#include "vec4.h"
#include <fastmath.h>

vec4::vec4()
	: x(0), y(0), z(0), w(0) {}

vec4::vec4(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w) {}

vec4::vec4(float* vec)
{
	if (vec)
		x = vec[0], y = vec[1], z = vec[2], w = vec[3];
	else
		*this = vec4();
}

vec4 vec4::operator+(const vec4 vec)
{
	return vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
}

vec4 vec4::operator+(const float* vec)
{
	return vec4(x + vec[0], y + vec[1], z + vec[2], w + vec[3]);
}

vec4 vec4::operator+(const float value)
{
	return vec4(x + value, y + value, z + value, w + value);
}

vec4 vec4::operator-(const vec4 vec)
{
	return vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}

vec4 vec4::operator-(const float* vec)
{
	return vec4(x - vec[0], y - vec[1], z - vec[2], w - vec[3]);
}

vec4 vec4::operator-(const float value)
{
	return vec4(x - value, y - value, z - value, w - value);
}

vec4 vec4::operator-()
{
	return vec4(-x, -y, -z, -w);
}

vec4 vec4::operator*(const vec4 vec)
{
	return vec4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
}

vec4 vec4::operator*(const float* vec)
{
	return vec4(x * vec[0], y * vec[1], z * vec[2], w * vec[3]);
}

vec4 vec4::operator*(const float value)
{
	return vec4(x * value, y * value, z * value, w * value);
}

vec4 vec4::operator/(const vec4 vec)
{
	return vec4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
}

vec4 vec4::operator/(const float* vec)
{
	return vec4(x / vec[0], y / vec[1], z / vec[2], w / vec[3]);
}

vec4 vec4::operator/(const float value)
{
	return vec4(x / value, y / value, z / value, w / value);
}

void vec4::operator+=(const vec4 vec)
{
	x += vec.x, y += vec.y, z += vec.z, w += vec.w;
}

void vec4::operator+=(const float* vec)
{
	x += vec[0], y += vec[1], z += vec[2], w += vec[3];
}

void vec4::operator+=(const float value)
{
	x += value, y += value, z += value, w += value;
}

void vec4::operator-=(const vec4 vec)
{
	x -= vec.x, y -= vec.y, z -= vec.z, w -= vec.w;
}

void vec4::operator-=(const float* vec)
{
	x -= vec[0], y -= vec[1], z -= vec[2], w -= vec[3];
}

void vec4::operator-=(const float value)
{
	x -= value, y -= value, z -= value, w -= value;
}

void vec4::operator*=(const vec4 vec)
{
	x *= vec.x, y *= vec.y, z *= vec.z, w *= vec.w;
}

void vec4::operator*=(const float* vec)
{
	x *= vec[0], y *= vec[1], z *= vec[2], w *= vec[3];
}

void vec4::operator*=(const float value)
{
	x *= value, y *= value, z *= value, w *= value;
}

void vec4::operator/=(const vec4 vec)
{
	x /= vec.x, y /= vec.y, z /= vec.z, w /= vec.w;
}

void vec4::operator/=(const float* vec)
{
	x /= vec[0], y /= vec[1], z /= vec[2], w /= vec[3];
}

void vec4::operator/=(const float value)
{
	x /= value, y /= value, z /= value, w /= value;
}

bool vec4::operator==(const vec4 vec)
{
	return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
}

bool vec4::operator==(const float* vec)
{
	return x == vec[0] && y == vec[1] && z == vec[2] && w == vec[3];
}

bool vec4::operator==(const float value)
{
	return x == value && y == value && z == value && w == value;
}

bool vec4::operator!=(const vec4 vec)
{
	return x != vec.x || y != vec.y || z != vec.z || w != vec.w;
}

bool vec4::operator!=(const float* vec)
{
	return x != vec[0] || y != vec[1] || z != vec[2] || w != vec[3];
}

bool vec4::operator!=(const float value)
{
	return x != value || y != value || z != value || w != value;
}

float& vec4::operator[](const int i)
{
	return v[i];
}

vec4::operator float*()
{
	return (float*)&this[0];
}

vec4::operator const float*()
{
	return (const float*)&this[0];
}

float vec4::magnitude()
{
	return std::f_sqrtf(dot(*this));
}

float vec4::dot(vec4 vec)
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z) + (w * vec.w);
}

float vec4::distance(vec4 vec)
{
	vec4 diff = *this - vec;
	return diff.magnitude();
}

vec4& vec4::normalize()
{
	*this /= magnitude();
	return *this;
}

vec4& vec4::negate()
{
	*this = -*this;
	return *this;
}

vec4& vec4::clamp(float min, float max)
{
	x = (x > max) ? max : (x < min) ? min : x;
	y = (y > max) ? max : (y < min) ? min : y;
	z = (z > max) ? max : (z < min) ? min : z;
	w = (w > max) ? max : (w < min) ? min : w;
	return *this;
}