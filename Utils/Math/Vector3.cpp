#include "Vector3.h"

#include <cmath>

float Vector3::Length() {
	float result = static_cast<float>(sqrt(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2)));
	return result;
}

float Vector3::Length(const Vector3& pos) {
	float result = static_cast<float>(sqrt(powf(this->x - pos.x, 2) + powf(this->y - pos.y, 2) + powf(this->z - pos.z, 2)));
	return result;
}

Vector3 Vector3::Normalize() {
	float len = Length();

	Vector3 result = {};

	if (len != 0) {
		result = { this->x / len, this->y / len, this->z / len };
	}

	return result;
}


Vector3 operator+(const Vector3& obj1, const Vector3& obj2) {
	Vector3 tmp = {};
	tmp.x = obj1.x + obj2.x;
	tmp.y = obj1.y + obj2.y;
	tmp.z = obj1.z + obj2.z;
	return tmp;
}


//減算
Vector3 operator-(const Vector3& obj1, const Vector3& obj2) {
	Vector3 tmp = {};
	tmp.x = obj1.x - obj2.x;
	tmp.y = obj1.y - obj2.y;
	tmp.z = obj1.z - obj2.z;
	return tmp;
}

//スカラー

//乗算
Vector3 operator*(float a, const Vector3& obj) {
	Vector3 tmp = {};

	tmp.x = obj.x * a;
	tmp.y = obj.y * a;
	tmp.z = obj.z * a;
	return tmp;
}

Vector3 operator*(const Vector3& obj, float a) {
	Vector3 tmp = {};

	tmp.x = obj.x * a;
	tmp.y = obj.y * a;
	tmp.z = obj.z * a;
	return tmp;
}

//除算
Vector3 operator/(const Vector3& obj, float a) {

	if (a == 0) {
		return {};
	}
	Vector3 tmp = {};

	tmp.x = obj.x / a;
	tmp.y = obj.y / a;
	tmp.z = obj.z / a;
	return tmp;
}

Vector3 operator+(const Vector3& obj) {
	return obj;
}

Vector3 operator-(const Vector3& obj) {
	return { -obj.x,-obj.y,-obj.z };
}