#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"

class Transform {

public:

	enum Type {
		X,
		Y,
		Z,
		All
	};

	Transform();

	void Initialize();

	void UpdateMatrix();

	Vector3 GetWorldPosition();

	bool IsCollisionXZ(const Transform& transform);

	bool IsCollisionXY(const Transform& transform);

	bool IsCollisionYZ(const Transform& transform);

	void SetWorldTranslateParent(const Transform* transform);

	void ClearWorldTranslateParent();

	void SetOtherRotateMatrix(const Matrix4x4& mat);

	void SetOtherRotateMatrix(const Vector3& from, const Vector3& to, Type type = All);

	void SetOtherRotateMatrix(const Vector3& from, const Vector3& to, const Matrix4x4& multiplyMat, Type type = All);

	const Matrix4x4 GetOtherRotMat() const { return otherRotateMat_; }

	void SetOtherRotMat(const Matrix4x4& mat, Type type = All) { otherRotateMat_ = mat; otherRotType_ = type; }

	const Matrix4x4 GetRotMat() const;

	const Matrix4x4 GetRotMat(Type type) const;

	void ClearOtherRotMat();

public:

	Vector3 scale_;
	Vector3 rotate_;
	Vector3 translate_;

	Matrix4x4 worldMat_;

	Vector3 worldPos_;

	const Transform* parent_ = nullptr;

	const Transform* worldTranslateParent_ = nullptr;

	Matrix4x4 otherRotateMat_;

	Type otherRotType_ = All;

	bool isUseOtherRotateMat_;

	Matrix4x4 rotMat_[4];
private:


};