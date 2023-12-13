#pragma once
#include "Vector3.h"

struct Matrix4x4;

struct Quaternion
{
public:


	Quaternion& operator=(const Quaternion& obj) {
		x = obj.x;
		y = obj.y;
		z = obj.z;
		w = obj.w;
		return *this;
	}

	void operator+=(const Quaternion& obj) {
		this->x = this->x + obj.x;
		this->y = this->y + obj.y;
		this->z = this->z + obj.z;
		this->w = this->w + obj.w;
	}

	void  operator-=(const Quaternion& obj) {
		this->x -= obj.x;
		this->y -= obj.y;
		this->z -= obj.z;
		this->w -= obj.w;
	}

	void operator*=(float a) {
		this->x *= a;
		this->y *= a;
		this->z *= a;
		this->w *= a;
	}

	void operator/=(float a) {
		this->x /= a;
		this->y /= a;
		this->z /= a;
		this->w /= a;
	}

	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	static float Dot(const Quaternion& q0, const Quaternion& q1);

	static Quaternion MakeRotateAxisAngle(const Vector3& axis, float angle);

	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	static Quaternion Identity();
	// 共役Quaternionを返す
	Quaternion Conjugate();

	float Norm();

	Quaternion Normalize();

	Quaternion Inverse();

	Matrix4x4 MakeRotateMatrix();

	Vector3 RotateVector(const Vector3& vector);

public:
	float x;
	float y;
	float z;
	float w;
};

Quaternion operator+(const Quaternion& obj1, const Quaternion& obj2);
Quaternion operator-(const Quaternion& obj1, const Quaternion& obj2);
Quaternion operator*(const Quaternion obj1, const Quaternion& obj2);
Quaternion operator*(const Quaternion& obj, float a);
Quaternion operator*(float a, const Quaternion& obj);
Quaternion operator/(const Quaternion& obj, float a);