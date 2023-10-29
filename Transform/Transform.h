#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"

class Transform {

public:
	Transform();

	void UpdateMatrix();

	Vector3 GetWorldPosition();

	bool IsCollisionXZ(const Transform& transform);

public:

	Vector3 scale_;
	Vector3 rotate_;
	Vector3 translate_;

	Matrix4x4 worldMat_;

	Vector3 worldPos_;

	const Transform* parent_ = nullptr;

	const Transform* worldTranslateParent_ = nullptr;
};