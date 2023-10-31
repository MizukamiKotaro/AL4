#include "Matrix3x3.h"
#include <cmath>
#include <assert.h>

Matrix3x3 Matrix3x3::Add(const Matrix3x3 m1, const Matrix3x3& m2) {
	Matrix3x3 result = {};
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			result.m[row][column] = m1.m[row][column] + m2.m[row][column];
		}
	}
	return result;
}

Matrix3x3 Matrix3x3::Subtract(const Matrix3x3& m1, const Matrix3x3& m2) {
	Matrix3x3 result = {};
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			result.m[row][column] = m1.m[row][column] - m2.m[row][column];
		}
	}
	return result;
}

Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& m1, const Matrix3x3& m2) {
	Matrix3x3 result = {};
	for (int i = 0; i < 3; i++) {
		for (int row = 0; row < 3; row++) {
			for (int column = 0; column < 3; column++) {
				result.m[i][row] += m1.m[i][column] * m2.m[column][row];
			}
		}
	}
	return result;
}

Matrix3x3 Matrix3x3::Multiply(float num, const Matrix3x3& m) {
	Matrix3x3 result = {};
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			result.m[row][column] = m.m[row][column] * num;
		}
	}
	return result;
}

Vector2 Matrix3x3::Multiply(const Vector2& vector, const Matrix3x3& m) {
	Vector2 result = {};
	result.x = vector.x * m.m[0][0] + vector.y * m.m[1][0] + 1.0f * m.m[2][0];
	result.y = vector.x * m.m[0][1] + vector.y * m.m[1][1] + 1.0f * m.m[2][1];
	float w = vector.x * m.m[0][2] + vector.y * m.m[1][2] + 1.0f * m.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;

	return result;
}

Matrix3x3 Matrix3x3::Inverse(const Matrix3x3& m) {
	float num = Matrix3x3::SarrasRule(m);
	struct MyMatrix2x2
	{
		float m[2][2];
	};
	MyMatrix2x2 m2[3][3] = {};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int row = 0; row < 2; row++) {
				for (int column = 0; column < 2; column++) {
					if (row >= i) {
						if (column >= j) {
							m2[i][j].m[row][column] = m.m[row + 1][column + 1];
						}
						else {
							m2[i][j].m[row][column] = m.m[row + 1][column];
						}
					}
					else {
						if (column >= j) {
							m2[i][j].m[row][column] = m.m[row][column + 1];
						}
						else {
							m2[i][j].m[row][column] = m.m[row][column];
						}
					}
				}
			}
		}
	}

	if (num != 0) {
		num = 1.0f / num;
	}
	Matrix3x3 result = {};

	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			if ((row + column) % 2 == 0) {
				result.m[row][column] = num * (m2[column][row].m[0][0] * m2[column][row].m[1][1] - m2[column][row].m[0][1] * m2[column][row].m[1][0]);
			}
			else {
				result.m[row][column] = -num * (m2[column][row].m[0][0] * m2[column][row].m[1][1] - m2[column][row].m[0][1] * m2[column][row].m[1][0]);
			}
		}
	}

	return result;
}

Matrix3x3 Matrix3x3::Inverse() {
	return Inverse(*this);
}

Matrix3x3 Matrix3x3::MakeAffineTransformMatrix(const Vector2& scale, float radian, const Vector2& translate) {
	Matrix3x3 matrix = {
	   {{float(scale.x * std::cos(radian)), float(scale.x * std::sin(radian)),0},
	   {float(-scale.y * std::sin(radian)), float(scale.y * std::cos(radian)),0},
	   {translate.x,translate.y,1}
	   }
	};
	return matrix;
}

float Matrix3x3::SarrasRule(const Matrix3x3& m) {
	float result = m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1];
	return result;
}

Matrix3x3 Matrix3x3::MakeOrthographicMatrix(float left, float top, float right, float bottom) {
	Matrix3x3 matrix = {};
	matrix.m[0][0] = 2.0f / (right - left); matrix.m[0][1] = 0; matrix.m[0][2] = 0;
	matrix.m[1][0] = 0; matrix.m[1][1] = 2.0f / (top - bottom); matrix.m[1][2] = 0;
	matrix.m[2][0] = (left + right) / (left - right); matrix.m[2][1] = (top + bottom) / (bottom - top); matrix.m[2][2] = 1;
	return matrix;
}

Matrix3x3 Matrix3x3::MakeViewportMatrix(float left, float top, float right, float bottom) {
	Matrix3x3 matrix = {};
	matrix.m[0][0] = (right - left) / 2; matrix.m[0][1] = 0; matrix.m[0][2] = 0;
	matrix.m[1][0] = 0; matrix.m[1][1] = (top - bottom) / 2; matrix.m[1][2] = 0;
	matrix.m[2][0] = left + (right - left) / 2; matrix.m[2][1] = top + (bottom - top) / 2; matrix.m[2][2] = 1;
	return matrix;
}

Matrix3x3 Matrix3x3::MakeWvpVpMatrix(const Vector2& objectScale, float objectRadian, const Vector2& objectTranslate,
	const Vector2& cameraScale, float cameraRadian, const Vector2& cameraTranslate,
	const Vector2& orthoTopLeft, const Vector2& orthoBottomRight, const Vector2& viewportTopLeft, const Vector2& viewportBottomRight) {

	Matrix3x3 objectWorldMatrix = Matrix3x3::MakeAffineTransformMatrix(objectScale, objectRadian, objectTranslate);
	Matrix3x3 cameraWorldMatrix = Matrix3x3::MakeAffineTransformMatrix(cameraScale, cameraRadian, cameraTranslate);
	Matrix3x3 viewMatrix = Matrix3x3::Inverse(cameraWorldMatrix);
	Matrix3x3 orthoMatrix = Matrix3x3::MakeOrthographicMatrix(orthoTopLeft.x, orthoTopLeft.y, orthoBottomRight.x, orthoBottomRight.y);
	Matrix3x3 ViewportMatrix = Matrix3x3::MakeViewportMatrix(viewportTopLeft.x, viewportTopLeft.y, viewportBottomRight.x, viewportBottomRight.y);

	Matrix3x3 wvpVpMatrix = Matrix3x3::Multiply(objectWorldMatrix, viewMatrix);
	wvpVpMatrix = Matrix3x3::Multiply(wvpVpMatrix, orthoMatrix);
	wvpVpMatrix = Matrix3x3::Multiply(wvpVpMatrix, ViewportMatrix);

	return wvpVpMatrix;
}

Matrix3x3 operator+(const Matrix3x3 m1, const Matrix3x3& m2) {
	return Matrix3x3::Add(m1, m2);
}

Matrix3x3 operator-(const Matrix3x3 m1, const Matrix3x3& m2) {
	return  Matrix3x3::Subtract(m1, m2);
}

Matrix3x3 operator*(const Matrix3x3 m1, const Matrix3x3& m2) {
	return Matrix3x3::Multiply(m1, m2);
}

Matrix3x3 operator*(float s, const Matrix3x3& m) {
	return Matrix3x3::Multiply(s, m);
}

Matrix3x3 operator*(const Matrix3x3& m, float s) {
	return Matrix3x3::Multiply(s, m);
}

Vector2 operator*(const Vector2& v, const Matrix3x3& m) {
	return Matrix3x3::Multiply(v, m);
}

Vector2 operator*(const Matrix3x3& m, const Vector2& v) {
	return Matrix3x3::Multiply(v, m);
}
