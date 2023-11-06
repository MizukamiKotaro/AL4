#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"

class Transform {

public:
	Transform();

	void Initialize();

	void UpdateMatrix();

	Vector3 GetWorldPosition();

	bool IsCollisionXZ(const Transform& transform);

	void SetWorldTranslateParent(const Transform* transform);

	void ClearWorldTranslateParent();

	void SetOtherRotateMatrix(const Matrix4x4& mat);

	void SetOtherRotateMatrix(const Vector3& from, const Vector3& to);

	void SetOtherRotateMatrix(const Vector3& from, const Vector3& to, const Matrix4x4& multiplyMat);

public:

	Vector3 scale_;
	Vector3 rotate_;
	Vector3 translate_;

	Matrix4x4 worldMat_;

	Vector3 worldPos_;

	const Transform* parent_ = nullptr;

	const Transform* worldTranslateParent_ = nullptr;

	Matrix4x4 otherRotateMat_;

	bool isUseOtherRotateMat_;

};