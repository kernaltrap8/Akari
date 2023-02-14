
#include "vec2.h"
#include <fastmath.h>

vec2::vec2()
	: x(0), y(0) {}

vec2::vec2(float x, float y)
	: x(x), y(y) {}

vec2::vec2(float* vec)
{
	if (vec)
		x = vec[0], y = vec[1];
	else
		*this = vec2();
}

vec2 vec2::operator+(const vec2 vec)
{
	return vec2(x + vec.x, y + vec.y);
}

vec2 vec2::operator+(const float* vec)
{
	return vec2(x + vec[0], y + vec[1]);
}

vec2 vec2::operator+(const float value)
{
	return vec2(x + value, y + value);
}

vec2 vec2::operator-(const vec2 vec)
{
	return vec2(x - vec.x, y - vec.y);
}

vec2 vec2::operator-(const float* vec)
{
	return vec2(x - vec[0], y - vec[1]);
}

vec2 vec2::operator-(const float value)
{
	return vec2(x - value, y - value);
}

vec2 vec2::operator-()
{
	return vec2(-x, -y);
}

vec2 vec2::operator*(const vec2 vec)
{
	return vec2(x * vec.x, y * vec.y);
}

vec2 vec2::operator*(const float* vec)
{
	return vec2(x * vec[0], y * vec[1]);
}

vec2 vec2::operator*(const float value)
{
	return vec2(x * value, y * value);
}

vec2 vec2::operator/(const vec2 vec)
{
	return vec2(x / vec.x, y / vec.y);
}

vec2 vec2::operator/(const float* vec)
{
	return vec2(x / vec[0], y / vec[1]);
}

vec2 vec2::operator/(const float value)
{
	return vec2(x / value, y / value);
}

void vec2::operator+=(const vec2 vec)
{
	x += vec.x, y += vec.y;
}

void vec2::operator+=(const float* vec)
{
	x += vec[0], y += vec[1];
}

void vec2::operator+=(const float value)
{
	x += value, y += value;
}

void vec2::operator-=(const vec2 vec)
{
	x -= vec.x, y -= vec.y;
}

void vec2::operator-=(const float* vec)
{
	x -= vec[0], y -= vec[1];
}

void vec2::operator-=(const float value)
{
	x -= value, y -= value;
}

void vec2::operator*=(const vec2 vec)
{
	x *= vec.x, y *= vec.y;
}

void vec2::operator*=(const float* vec)
{
	x *= vec[0], y *= vec[1];
}

void vec2::operator*=(const float value)
{
	x *= value, y *= value;
}

void vec2::operator/=(const vec2 vec)
{
	x /= vec.x, y /= vec.y;
}

void vec2::operator/=(const float* vec)
{
	x /= vec[0], y /= vec[1];
}

void vec2::operator/=(const float value)
{
	x /= value, y /= value;
}

bool vec2::operator==(const vec2 vec)
{
	return x == vec.x && y == vec.y;
}

bool vec2::operator==(const float* vec)
{
	return x == vec[0] && y == vec[1];
}

bool vec2::operator==(const float value)
{
	return x == value && y == value;
}

bool vec2::operator!=(const vec2 vec)
{
	return x != vec.x || y != vec.y;
}

bool vec2::operator!=(const float* vec)
{
	return x != vec[0] || y != vec[1];
}

bool vec2::operator!=(const float value)
{
	return x != value || y != value;
}

float& vec2::operator[](const int i)
{
	return v[i];
}

vec2::operator float*()
{
	return (float*)&this[0];
}

vec2::operator const float*()
{
	return (const float*)&this[0];
}

float vec2::magnitude()
{
	return std::f_sqrtf(dot(*this));
}

float vec2::dot(vec2 vec)
{
	return (x * vec.x) + (y * vec.y);
}

float vec2::distance(vec2 vec)
{
	vec2 diff = *this - vec;
	return diff.magnitude();
}

vec2& vec2::normalize()
{
	*this /= magnitude();
	return *this;
}

vec2& vec2::negate()
{
	*this = -*this;
	return *this;
}

vec2& vec2::clamp(float min, float max)
{
	x = (x > max) ? max : (x < min) ? min : x;
	y = (y > max) ? max : (y < min) ? min : y;
	return *this;
}