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

// ŠOÏ‚ğ‹‚ß‚é
Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}