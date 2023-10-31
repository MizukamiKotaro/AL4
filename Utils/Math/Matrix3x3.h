#pragma once
#include "Vector2.h"

class Matrix3x3
{
public:
	static Matrix3x3 Add(const Matrix3x3 m1, const Matrix3x3& m2);

	static Matrix3x3 Subtract(const Matrix3x3& m1, const Matrix3x3& m2);

	static Matrix3x3 Multiply(const Matrix3x3& m1, const Matrix3x3& m2);

	static Matrix3x3 Multiply(float num, const Matrix3x3& m);

	static Vector2 Multiply(const Vector2& vector, const Matrix3x3& m);

	static Matrix3x3 Inverse(const Matrix3x3& m);

	Matrix3x3 Inverse();

	static Matrix3x3 MakeAffineTransformMatrix(const Vector2& scale, float radian, const Vector2& translate);

	static float SarrasRule(const Matrix3x3& m);

	static Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom);

	static Matrix3x3 MakeViewportMatrix(float left, float top, float right, float bottom);

	static Matrix3x3 MakeWvpVpMatrix(const Vector2& objectScale, float objectRadian, const Vector2& objectTranslate,
		const Vector2& cameraScale, float cameraRadian, const Vector2& cameraTranslate,
		const Vector2& orthoTopLeft, const Vector2& orthoBottomRight, const Vector2& viewportTopLeft, const Vector2& viewportBottomRight);

public:
	float m[3][3];
};

Matrix3x3 operator+(const Matrix3x3 m1, const Matrix3x3& m2);

Matrix3x3 operator-(const Matrix3x3 m1, const Matrix3x3& m2);

Matrix3x3 operator*(const Matrix3x3 m1, const Matrix3x3& m2);

Matrix3x3 operator*(float s, const Matrix3x3& m);

Matrix3x3 operator*(const Matrix3x3& m, float s);

Vector2 operator*(const Vector2& v, const Matrix3x3& m);

Vector2 operator*(const Matrix3x3& m, const Vector2& v);