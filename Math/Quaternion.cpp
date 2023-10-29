#include "Quaternion.h"
#include <cmath>

Quaternion operator+(const Quaternion& obj1, const Quaternion& obj2) {
	Quaternion tmp = {};
	tmp.x = obj1.x + obj2.x;
	tmp.y = obj1.y + obj2.y;
	tmp.z = obj1.z + obj2.z;
	tmp.w = obj1.w + obj2.w;
	return tmp;
}

Quaternion operator-(const Quaternion& obj1, const Quaternion& obj2) {
	Quaternion tmp = {};
	tmp.x = obj1.x - obj2.x;
	tmp.y = obj1.y - obj2.y;
	tmp.z = obj1.z - obj2.z;
	tmp.w = obj1.w - obj2.w;
	return tmp;
}

Quaternion operator*(const Quaternion& obj, float a) {
	Quaternion tmp = {};

	tmp.x = obj.x * a;
	tmp.y = obj.y * a;
	tmp.z = obj.z * a;
	tmp.w = obj.w * a;
	return tmp;
}

Quaternion operator*(float a, const Quaternion& obj) {
	return obj * a;
}


Quaternion operator/(const Quaternion& obj, float a) {
	Quaternion tmp = {};

	tmp.x = obj.x / a;
	tmp.y = obj.y / a;
	tmp.z = obj.z / a;
	tmp.w = obj.w / a;
	return tmp;
}

Quaternion Quaternion::MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	Quaternion result = { 0.0f,0.0f,0.0f,0.0f };

	Vector3 n = axis;

	n = n.Normalize();

	result.w = std::cosf(angle * 0.5f);
	result.x = n.x * std::sinf(angle * 0.5f);
	result.y = n.y * std::sinf(angle * 0.5f);
	result.z = n.z * std::sinf(angle * 0.5f);

	return result;
}
