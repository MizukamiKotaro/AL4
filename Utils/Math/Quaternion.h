#pragma once
#include "Vector3.h"

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

	static Quaternion MakeRotateAxisAngle(const Vector3& axis, float angle);

public:
	float x;
	float y;
	float z;
	float w;
};

Quaternion operator+(const Quaternion& obj1, const Quaternion& obj2);
Quaternion operator-(const Quaternion& obj1, const Quaternion& obj2);
Quaternion operator*(const Quaternion& obj, float a);
Quaternion operator*(float a, const Quaternion& obj);
Quaternion operator/(const Quaternion& obj, float a);