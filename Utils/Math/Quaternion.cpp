#include "Quaternion.h"
#include <cmath>
#include "Utils/Math/calc.h"
#include "Utils/Math/Matrix4x4.h"

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

Quaternion operator*(const Quaternion obj1, const Quaternion& obj2)
{
	return Quaternion::Multiply(obj1, obj2);
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

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion a = q0;
	a = a.Normalize();
	Quaternion b = q1;
	b = b.Normalize();

	float dot = Dot(a, b);
	if (dot < 0) {
		a = a * (-1);
		dot = -dot;
	}

	if (dot >= 1.0f - 0.0005f) {
		// 0除算とsin(theta)が0に近くなる場合の処理
		return (1.0f - t) * a + t * b;
	}

	float theta = std::acosf(dot);

	float scale0 = std::sinf((1.0f - t) * theta) / std::sinf(theta);
	float scale1 = std::sinf(t * theta) / std::sinf(theta);

	return scale0 * a + scale1 * b;
}

float Quaternion::Dot(const Quaternion& q0, const Quaternion& q1)
{
	Quaternion a = q0;
	a = a.Normalize();
	Quaternion b = q1;
	b = b.Normalize();

	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
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

Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Vector3 lhsVec3 = { lhs.x,lhs.y,lhs.z };
	Vector3 rhsVec3 = { rhs.x,rhs.y,rhs.z };

	float w = lhs.w * rhs.w - Calc::Dot(lhsVec3, rhsVec3);
	Vector3 vec3 = Calc::Cross(lhsVec3, rhsVec3) + rhs.w * lhsVec3 + lhs.w * rhsVec3;

	return Quaternion{ vec3.x,vec3.y,vec3.z,w };
}

Quaternion Quaternion::Identity()
{
	return Quaternion{ 0.0f,0.0f,0.0f,1.0f };
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion{ -this->x,-this->y,-this->z,this->w };
}

float Quaternion::Norm()
{
	return std::sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
}

Quaternion Quaternion::Normalize()
{
	return *this / this->Norm();
}

Quaternion Quaternion::Inverse()
{
	return this->Conjugate() / std::powf(this->Norm(), 2.0f);
}

Matrix4x4 Quaternion::MakeRotateMatrix()
{
	Matrix4x4 result = {
		std::powf(this->w,2) + std::powf(this->x,2) - std::powf(this->y,2) - std::powf(this->z,2),
		2.0f * (this->x * this->y + this->w * this->z),2.0f * (this->x * this->z - this->w * this->y),0.0f,
		2.0f * (this->x * this->y - this->w * this->z),
		std::powf(this->w,2) - std::powf(this->x,2) + std::powf(this->y,2) - std::powf(this->z,2),
		2.0f * (this->y * this->z + this->w * this->x),0.0f,
		2.0f * (this->x * this->z + this->w * this->y),2.0f * (this->y * this->z - this->w * this->x),
		std::powf(this->w,2) - std::powf(this->x,2) - std::powf(this->y,2) + std::powf(this->z,2),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Vector3 Quaternion::RotateVector(const Vector3& vector)
{
	return vector * this->MakeRotateMatrix();
}
