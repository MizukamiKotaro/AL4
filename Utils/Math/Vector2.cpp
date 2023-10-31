#include "Vector2.h"

Vector2 operator+(const Vector2& obj1, const Vector2& obj2) {
	Vector2 tmp = {};
	tmp.x = obj1.x + obj2.x;
	tmp.y = obj1.y + obj2.y;
	return tmp;
}

Vector2 operator-(const Vector2& obj1, const Vector2& obj2) {
	Vector2 tmp = {};
	tmp.x = obj1.x - obj2.x;
	tmp.y = obj1.y - obj2.y;
	return tmp;
}

Vector2 operator*(const Vector2& obj, float a) {
	Vector2 tmp = {};

	tmp.x = obj.x * a;
	tmp.y = obj.y * a;
	return tmp;
}

Vector2 operator*(float a, const Vector2& obj) {
	return obj * a;
}


Vector2 operator/(const Vector2& obj, float a) {
	Vector2 tmp = {};

	tmp.x = obj.x / a;
	tmp.y = obj.y / a;
	return tmp;
}