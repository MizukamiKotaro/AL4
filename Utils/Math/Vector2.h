#pragma once

struct Vector2
{
public:


	Vector2& operator=(const Vector2& obj) {
		x = obj.x;
		y = obj.y;
		return *this;
	}

	void operator+=(const Vector2& obj) {
		this->x = this->x + obj.x;
		this->y = this->y + obj.y;
	}

	void  operator-=(const Vector2& obj) {
		this->x -= obj.x;
		this->y -= obj.y;
	}

	void operator*=(float a) {
		this->x *= a;
		this->y *= a;
	}

	void operator/=(float a) {
		this->x /= a;
		this->y /= a;
	}

public:
	float x;
	float y;
};

Vector2 operator+(const Vector2& obj1, const Vector2& obj2);

Vector2 operator-(const Vector2& obj1, const Vector2& obj2);

Vector2 operator*(const Vector2& obj, float a);

Vector2 operator*(float a, const Vector2& obj);

Vector2 operator/(const Vector2& obj, float a);