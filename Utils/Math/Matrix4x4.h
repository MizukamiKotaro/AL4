#pragma once
#include "Vector3.h"
#include "Quaternion.h"

class  Transform;

struct Matrix4x4 {
public:
	enum RotateType {
		kXYZ,
		kYXZ,
		kZXY,
		kXZY,
		kYZX,
		kZYX
	};
public:
	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 Multiply(const float& num, const Matrix4x4& m);

	static Vector3 Multiply(const Vector3& vector, const Matrix4x4& m);

	static Matrix4x4 Inverse(const Matrix4x4& m);

	Matrix4x4 Inverse();

	// 転置行列
	static Matrix4x4 Transpose(const Matrix4x4& m);

	// 単位行列
	static Matrix4x4 MakeIdentity4x4();

	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	/*static Vector3 Transform(const Vector3& vector, const Matrix4x4& m);*/

	static Matrix4x4 MakeRotateXMatrix(float radian);

	static Matrix4x4 MakeRotateYMatrix(float radian);

	static Matrix4x4 MakeRotateZMatrix(float radian);

	static Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate);

	static Matrix4x4 MakeRotateMatrix(const Vector3& rotate, RotateType rotateOrder);

	static Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	static Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate, RotateType rotateOrder);

	static Matrix4x4 MakeAffinMatrix(const Transform& transform);
	//透視投影行列
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
	//正射影行列
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	//ビューポート変換行列
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	// 任意軸回転行列の作成
	static Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);
	// ある方向からある方向への回転
	static Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) { return Matrix4x4::Multiply(vector, MakeRotateMatrix(quaternion)); }

public:
	float m[4][4];
};

Matrix4x4 operator+(const Matrix4x4 m1, const Matrix4x4& m2);

Matrix4x4 operator-(const Matrix4x4 m1, const Matrix4x4& m2);

Matrix4x4 operator*(const Matrix4x4 m1, const Matrix4x4& m2);

Matrix4x4 operator*(float s, const Matrix4x4& m);

Matrix4x4 operator*(const Matrix4x4& m, float s);

Vector3 operator*(const Vector3& v, const Matrix4x4& m);

Vector3 operator*(const Matrix4x4& m, const Vector3& v);