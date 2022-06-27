#include "Vector3.h"
#include <cmath>

float Vector3::length() {
	return sqrt(x * x + y * y + z * z);
};
Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
};

// äOêœÇãÅÇﬂÇÈ
Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

void Vector3::AAA(const Matrix4& v)
{
	x = (x * v.m[0][0]) + (y * v.m[1][0]) + (z * v.m[2][0]);
	y = (x * v.m[0][1]) + (y * v.m[1][1]) + (z * v.m[2][1]);
	z = (x * v.m[0][2]) + (y * v.m[1][2]) + (z * v.m[2][2]);
}
