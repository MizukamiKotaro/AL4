#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "calc.h"
#include "../Transform/Transform.h"
#include <assert.h>
#include <cmath>

Matrix4x4 Matrix4x4::Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m1.m[row][column] + m2.m[row][column];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m1.m[row][column] - m2.m[row][column];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				result.m[i][row] += m1.m[i][column] * m2.m[column][row];
			}
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::Multiply(const float& num, const Matrix4x4& m) {
	Matrix4x4 result = {};
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = num * m.m[row][column];
		}
	}
	return result;
}

Vector3 Matrix4x4::Multiply(const Vector3& vector, const Matrix4x4& m) {
	Vector3 result = {};
	result.x =
		vector.x * m.m[0][0] + vector.y * m.m[1][0] + vector.z * m.m[2][0] + 1.0f * m.m[3][0];
	result.y =
		vector.x * m.m[0][1] + vector.y * m.m[1][1] + vector.z * m.m[2][1] + 1.0f * m.m[3][1];
	result.z =
		vector.x * m.m[0][2] + vector.y * m.m[1][2] + vector.z * m.m[2][2] + 1.0f * m.m[3][2];
	float w = vector.x * m.m[0][3] + vector.y * m.m[1][3] + vector.z * m.m[2][3] + 1.0f * m.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& m) {
	Matrix4x4 result = {};

	float A[4] = {};

	// 小行列
	Matrix3x3 b[4][4] = {};
	int B[4][4] = {};
	for (int i = 0; i < 4; i++) {
		A[i] = m.m[i][0];
		for (int j = 0; j < 4; j++) {
			B[i][j] = i + j;
			for (int row = 0; row < 3; row++) {
				for (int column = 0; column < 3; column++) {
					if (i <= row) {
						if (j <= column) {
							b[i][j].m[row][column] = m.m[row + 1][column + 1];
						}
						else {
							b[i][j].m[row][column] = m.m[row + 1][column];
						}
					}
					else {
						if (j <= column) {
							b[i][j].m[row][column] = m.m[row][column + 1];
						}
						else {
							b[i][j].m[row][column] = m.m[row][column];
						}
					}
				}
			}
		}
	}

	float num = 0;

	Matrix4x4 c = {};
	for (int row = 0; row < 4; row++) {
		if (row == 0 || row == 2) {
			num += A[row] * Matrix3x3::SarrasRule(b[row][0]);
		}
		else {
			num -= A[row] * Matrix3x3::SarrasRule(b[row][0]);
		}
		for (int column = 0; column < 4; column++) {
			if (B[row][column] % 2 == 0) {
				c.m[row][column] = Matrix3x3::SarrasRule(b[column][row]);
			}
			else {
				c.m[row][column] = -1 * Matrix3x3::SarrasRule(b[column][row]);
			}
		}
	}

	if (num != 0) {
		num = float(1 / num);
	}

	result = Multiply(num, c);

	return result;
}

Matrix4x4 Matrix4x4::Inverse() {
	return Inverse(*this);
}

// 転置行列
Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& m) {
	Matrix4x4 result = {};
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m.m[column][row];
		}
	}
	return result;
}

// 単位行列
Matrix4x4 Matrix4x4::MakeIdentity4x4() {
	Matrix4x4 result = {};
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			if (row == column) {
				result.m[row][column] = 1;
			}
			else {
				result.m[row][column] = 0;
			}
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = { 1, 0, 0, 0, 0,           1,           0,           0,
						  0, 0, 1, 0, translate.x, translate.y, translate.z, 1 };

	return result;
}

Matrix4x4 Matrix4x4::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = { scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1 };

	return result;
}

//Vector3 Matrix4x4::Transform(const Vector3& vector, const Matrix4x4& m) {
//	Vector3 result = Matrix4x4::Multiply(vector, m);
//
//	return result;
//}

Matrix4x4 Matrix4x4::MakeRotateXMatrix(float radian) {
	Matrix4x4 result = { 1,
						  0,
						  0,
						  0,
						  0,
						  std::cos(radian),
						  std::sin(radian),
						  0,
						  0,
						  -std::sin(radian),
						  std::cos(radian),
						  0,
						  0,
						  0,
						  0,
						  1 };
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateYMatrix(float radian) {
	Matrix4x4 result = { std::cos(radian), 0, -std::sin(radian), 0, 0, 1, 0, 0,
						  std::sin(radian), 0, std::cos(radian),  0, 0, 0, 0, 1 };
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {
		std::cos(radian),
		std::sin(radian),
		0,
		0,
		-std::sin(radian),
		std::cos(radian),
		0,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		0,
		1 };
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateXYZMatrix(const Vector3& rotate) {
	Matrix4x4 result = Matrix4x4::Multiply(
		Matrix4x4::MakeRotateXMatrix(rotate.x),
		Matrix4x4::Multiply(
			Matrix4x4::MakeRotateYMatrix(rotate.y), Matrix4x4::MakeRotateZMatrix(rotate.z)));
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateMatrix(const Vector3& rotate, RotateType rotateOrder) {
	Matrix4x4 result = {};
	switch (rotateOrder)
	{
	case Matrix4x4::kXYZ:
		result = Matrix4x4::Multiply(Matrix4x4::MakeRotateXMatrix(rotate.x),
			Matrix4x4::Multiply(Matrix4x4::MakeRotateYMatrix(rotate.y), Matrix4x4::MakeRotateZMatrix(rotate.z)));
		return result;
		break;
	case Matrix4x4::kYXZ:
		result = Matrix4x4::Multiply(Matrix4x4::MakeRotateYMatrix(rotate.y),
			Matrix4x4::Multiply(Matrix4x4::MakeRotateXMatrix(rotate.x), Matrix4x4::MakeRotateZMatrix(rotate.z)));
		return result;
		break;
	case Matrix4x4::kZXY:
		result = Matrix4x4::Multiply(Matrix4x4::MakeRotateZMatrix(rotate.z),
			Matrix4x4::Multiply(Matrix4x4::MakeRotateXMatrix(rotate.x), Matrix4x4::MakeRotateYMatrix(rotate.y)));
		return result;
		break;
	case Matrix4x4::kXZY:
		result = Matrix4x4::Multiply(Matrix4x4::MakeRotateXMatrix(rotate.x),
			Matrix4x4::Multiply(Matrix4x4::MakeRotateZMatrix(rotate.z), Matrix4x4::MakeRotateYMatrix(rotate.y)));
		return result;
		break;
	case Matrix4x4::kYZX:
		result = Matrix4x4::Multiply(Matrix4x4::MakeRotateYMatrix(rotate.y),
			Matrix4x4::Multiply(Matrix4x4::MakeRotateZMatrix(rotate.z), Matrix4x4::MakeRotateXMatrix(rotate.x)));
		return result;
		break;
	case Matrix4x4::kZYX:
		result = Matrix4x4::Multiply(Matrix4x4::MakeRotateZMatrix(rotate.z),
			Matrix4x4::Multiply(Matrix4x4::MakeRotateYMatrix(rotate.y), Matrix4x4::MakeRotateXMatrix(rotate.x)));
		return result;
		break;
	default:
		return result;
		break;
	}
	return result;
}

Matrix4x4 Matrix4x4::MakeAffinMatrix(
	const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	/*Matrix4x4 result = Matrix4x4::Multiply(Matrix4x4::MakeScaleMatrix(scale),
		Matrix4x4::Multiply(Matrix4x4::MakeRotateXYZMatrix(rotate),
	   Matrix4x4::MakeTranslateMatrix(translate)));*/

	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(rotate);

	Matrix4x4 result = {
		scale.x * rotateMatrix.m[0][0],
		scale.x * rotateMatrix.m[0][1],
		scale.x * rotateMatrix.m[0][2],
		0,
		scale.y * rotateMatrix.m[1][0],
		scale.y * rotateMatrix.m[1][1],
		scale.y * rotateMatrix.m[1][2],
		0,
		scale.z * rotateMatrix.m[2][0],
		scale.z * rotateMatrix.m[2][1],
		scale.z * rotateMatrix.m[2][2],
		0,
		translate.x,
		translate.y,
		translate.z,
		1 };

	return result;
}

Matrix4x4 Matrix4x4::MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate, RotateType rotateOrder) {
	/*Matrix4x4 result = Matrix4x4::Multiply(Matrix4x4::MakeScaleMatrix(scale),
		Matrix4x4::Multiply(Matrix4x4::MakeRotateXYZMatrix(rotate), Matrix4x4::MakeTranslateMatrix(translate)));*/

	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateMatrix(rotate, rotateOrder);

	Matrix4x4 result = {
		scale.x * rotateMatrix.m[0][0],scale.x * rotateMatrix.m[0][1],scale.x * rotateMatrix.m[0][2],0,
		scale.y * rotateMatrix.m[1][0],scale.y * rotateMatrix.m[1][1],scale.y * rotateMatrix.m[1][2],0,
		scale.z * rotateMatrix.m[2][0],scale.z * rotateMatrix.m[2][1],scale.z * rotateMatrix.m[2][2],0,
		translate.x,translate.y,translate.z,1
	};

	return result;
}

Matrix4x4 Matrix4x4::MakeAffinMatrix(const Transform& transform) {

	if (transform.isUseOtherRotateMat_) {

		Matrix4x4 result = {
		transform.scale_.x * transform.otherRotateMat_.m[0][0],
		transform.scale_.x * transform.otherRotateMat_.m[0][1],
		transform.scale_.x * transform.otherRotateMat_.m[0][2],
		0,
		transform.scale_.y * transform.otherRotateMat_.m[1][0],
		transform.scale_.y * transform.otherRotateMat_.m[1][1],
		transform.scale_.y * transform.otherRotateMat_.m[1][2],
		0,
		transform.scale_.z * transform.otherRotateMat_.m[2][0],
		transform.scale_.z * transform.otherRotateMat_.m[2][1],
		transform.scale_.z * transform.otherRotateMat_.m[2][2],
		0,
		transform.translate_.x,
		transform.translate_.y,
		transform.translate_.z,
		1 };

		return result;


	}
	else {
		return MakeAffinMatrix(transform.scale_, transform.rotate_, transform.translate_);
	}
}

Matrix4x4 Matrix4x4::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {
		1.0f / (aspectRatio * tan(fovY / 2)),0,0,0,
		0,1.0f / tan(fovY / 2),0,0,
		0,0,farClip / (farClip - nearClip),1,
		0,0,-nearClip * farClip / (farClip - nearClip),0
	};
	return result;
}

Matrix4x4 Matrix4x4::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {
		2.0f / (right - left),0,0,0,
		0,2.0f / (top - bottom),0,0,
		0,0,1.0f / (farClip - nearClip),0,
		(left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1
	};
	return result;
}

Matrix4x4 Matrix4x4::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {
		width / 2,0,0,0,
		0,-height / 2,0,0,
		0,0,maxDepth - minDepth,0,
		left + width / 2,top + height / 2,minDepth,1
	};
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	
	Vector3 n = axis;

	n = n.Normalize();

	Matrix4x4 result = {
		n.x * n.x * (1.0f - std::cosf(angle)) + std::cosf(angle), n.x * n.y * (1.0f - std::cosf(angle)) + n.z * std::sinf(angle),
		n.x * n.z * (1.0f - std::cosf(angle)) - n.y * std::sinf(angle),0.0f,
		n.x * n.y * (1.0f - std::cosf(angle)) - n.z * std::sinf(angle), n.y * n.y * (1.0f - std::cosf(angle)) + std::cosf(angle),
		n.y * n.z * (1.0f - std::cosf(angle)) + n.x * std::sinf(angle),0.0f,
		n.x * n.z * (1.0f - std::cosf(angle)) + n.y * std::sinf(angle), n.y * n.z * (1.0f - std::cosf(angle)) - n.x * std::sinf(angle),
		n.z * n.z * (1.0f - std::cosf(angle)) + std::cosf(angle),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4x4 Matrix4x4::DirectionToDirection(const Vector3& from, const Vector3& to)
{
	Vector3 u = from;
	u = u.Normalize();
	Vector3 v = to;
	v = v.Normalize();

	Vector3 n = Calc::Cross(u, v).Normalize();
	// 魔逆の場合保留。MTの授業で

	float cosfTheta = Calc::Dot(u, v);
	float sinfTheta = Calc::Cross(u, v).Length();

	if (u.x == -v.x && u.y == -v.y && u.z == -v.z) {
		if (u.x != 0 || u.y != 0) {
			n = { u.y,-u.z,0.0f };
		}
		else {
			n = { u.z,0.0f,-u.x };
		}

		cosfTheta = -1;
		sinfTheta = 0;
	}

	Matrix4x4 result = {
		n.x * n.x * (1.0f - cosfTheta) + cosfTheta, n.x * n.y * (1.0f - cosfTheta) + n.z * sinfTheta,
		n.x * n.z * (1.0f - cosfTheta) - n.y * sinfTheta,0.0f,
		n.x * n.y * (1.0f - cosfTheta) - n.z * sinfTheta, n.y * n.y * (1.0f - cosfTheta) + cosfTheta,
		n.y * n.z * (1.0f - cosfTheta) + n.x * sinfTheta,0.0f,
		n.x * n.z * (1.0f - cosfTheta) + n.y * sinfTheta, n.y * n.z * (1.0f - cosfTheta) - n.x * sinfTheta,
		n.z * n.z * (1.0f - cosfTheta) + cosfTheta,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;

}

Matrix4x4 Matrix4x4::MakeRotateMatrix(const Quaternion& quaternion)
{

	Matrix4x4 result = {
		std::powf(quaternion.w,2) + std::powf(quaternion.x,2) - std::powf(quaternion.y,2) - std::powf(quaternion.z,2),
		2.0f * (quaternion.x * quaternion.y + quaternion.w * quaternion.z),2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y),0.0f,
		2.0f * (quaternion.x * quaternion.y - quaternion.w * quaternion.z),
		std::powf(quaternion.w,2) - std::powf(quaternion.x,2) + std::powf(quaternion.y,2) - std::powf(quaternion.z,2),
		2.0f * (quaternion.y * quaternion.z + quaternion.w * quaternion.x),0.0f,
		2.0f * (quaternion.x * quaternion.z + quaternion.w * quaternion.y),2.0f * (quaternion.y * quaternion.z - quaternion.w * quaternion.x),
		std::powf(quaternion.w,2) - std::powf(quaternion.x,2) - std::powf(quaternion.y,2) + std::powf(quaternion.z,2),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4x4 operator+(const Matrix4x4 m1, const Matrix4x4& m2) {
	return Matrix4x4::Add(m1, m2);
}

Matrix4x4 operator-(const Matrix4x4 m1, const Matrix4x4& m2) {
	return Matrix4x4::Subtract(m1, m2);
}

Matrix4x4 operator*(const Matrix4x4 m1, const Matrix4x4& m2) {
	return Matrix4x4::Multiply(m1, m2);
}

Matrix4x4 operator*(float s, const Matrix4x4& m) {
	return Matrix4x4::Multiply(s, m);
}

Matrix4x4 operator*(const Matrix4x4& m, float s) {
	return Matrix4x4::Multiply(s, m);
}

Vector3 operator*(const Vector3& v, const Matrix4x4& m) {
	return Matrix4x4::Multiply(v, m);
}

Vector3 operator*(const Matrix4x4& m, const Vector3& v) {
	return Matrix4x4::Multiply(v, m);
}