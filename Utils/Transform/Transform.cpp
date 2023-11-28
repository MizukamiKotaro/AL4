#include "Transform.h"
#include "../Math/Vector2.h"
#include <cmath>
#include <numbers>

Transform::Transform()
{
	isUseOtherRotateMat_ = false;

	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	translate_ = { 0.0f,0.0f,0.0f };

	worldMat_ = Matrix4x4::MakeAffinMatrix(*this);

	worldPos_.x = worldMat_.m[3][0];
	worldPos_.y = worldMat_.m[3][1];
	worldPos_.z = worldMat_.m[3][2];
}

void Transform::Initialize()
{
	
}

void Transform::UpdateMatrix()
{
	
	if (otherRotType_ == All) {
		rotMat_[X] = Matrix4x4::MakeRotateXMatrix(rotate_.x);
		rotMat_[Y] = Matrix4x4::MakeRotateYMatrix(rotate_.y);
		rotMat_[Z] = Matrix4x4::MakeRotateXMatrix(rotate_.z);
	}
	else if (otherRotType_ == X) {
		rotMat_[X] = otherRotateMat_;
		rotMat_[Y] = Matrix4x4::MakeRotateYMatrix(rotate_.y);
		rotMat_[Z] = Matrix4x4::MakeRotateXMatrix(rotate_.z);
	}
	else if (otherRotType_ == Y) {
		rotMat_[X] = Matrix4x4::MakeRotateXMatrix(rotate_.x);
		rotMat_[Y] = otherRotateMat_;
		rotMat_[Z] = Matrix4x4::MakeRotateXMatrix(rotate_.z);
	}
	else if (otherRotType_ == Z) {
		rotMat_[X] = Matrix4x4::MakeRotateXMatrix(rotate_.x);
		rotMat_[Y] = Matrix4x4::MakeRotateYMatrix(rotate_.y);
		rotMat_[Z] = otherRotateMat_;
	}

	if (otherRotType_ == All) {
		rotMat_[All] = otherRotateMat_;
	}
	else {
		rotMat_[All] = rotMat_[X] * rotMat_[Y] * rotMat_[Z];
	}

	worldMat_ = Matrix4x4::MakeAffinMatrix(*this);

	if (parent_) {
		worldMat_ = Matrix4x4::Multiply(worldMat_, parent_->worldMat_);
	}
	if (worldTranslateParent_) {
		worldMat_.m[3][0] += worldTranslateParent_->worldMat_.m[3][0];
		worldMat_.m[3][1] += worldTranslateParent_->worldMat_.m[3][1];
		worldMat_.m[3][2] += worldTranslateParent_->worldMat_.m[3][2];
	}

	worldPos_.x = worldMat_.m[3][0];
	worldPos_.y = worldMat_.m[3][1];
	worldPos_.z = worldMat_.m[3][2];
}

Vector3 Transform::GetWorldPosition()
{
	return worldPos_;
}

bool Transform::IsCollisionXZ(const Transform& transform)
{

	Vector2 min0 = { worldPos_.x - scale_.x,worldPos_.z - scale_.z };
	Vector2 max0 = { worldPos_.x + scale_.x,worldPos_.z + scale_.z };

	Vector2 min1 = { transform.worldPos_.x - transform.scale_.x,transform.worldPos_.z - transform.scale_.z };
	Vector2 max1 = { transform.worldPos_.x + transform.scale_.x,transform.worldPos_.z + transform.scale_.z };

	if (min0.x <= max1.x && max0.x >= min1.x &&
		min0.y <= max1.y && max0.y >= min1.y) {
		return true;
	}

	return false;
}

bool Transform::IsCollisionXY(const Transform& transform)
{
	Vector2 min0 = { worldPos_.x - scale_.x,worldPos_.y - scale_.y };
	Vector2 max0 = { worldPos_.x + scale_.x,worldPos_.y + scale_.y };

	Vector2 min1 = { transform.worldPos_.x - transform.scale_.x,transform.worldPos_.y - transform.scale_.y };
	Vector2 max1 = { transform.worldPos_.x + transform.scale_.x,transform.worldPos_.y + transform.scale_.y };

	if (min0.x <= max1.x && max0.x >= min1.x &&
		min0.y <= max1.y && max0.y >= min1.y) {
		return true;
	}

	return false;
}

bool Transform::IsCollisionYZ(const Transform& transform)
{
	Vector2 min0 = { worldPos_.y - scale_.y,worldPos_.z - scale_.z };
	Vector2 max0 = { worldPos_.y + scale_.y,worldPos_.z + scale_.z };

	Vector2 min1 = { transform.worldPos_.y - transform.scale_.y,transform.worldPos_.z - transform.scale_.z };
	Vector2 max1 = { transform.worldPos_.y + transform.scale_.y,transform.worldPos_.z + transform.scale_.z };

	if (min0.x <= max1.x && max0.x >= min1.x &&
		min0.y <= max1.y && max0.y >= min1.y) {
		return true;
	}

	return false;
}

void Transform::SetWorldTranslateParent(const Transform* transform)
{
	if (!worldTranslateParent_) {
		worldTranslateParent_ = transform;
		translate_ -= transform->worldPos_;
	}
}

void Transform::ClearWorldTranslateParent()
{
	if (worldTranslateParent_) {
		translate_ += worldTranslateParent_->worldPos_;
		worldTranslateParent_ = nullptr;
	}
}

void Transform::SetOtherRotateMatrix(const Matrix4x4& mat)
{
	otherRotateMat_ = mat;
	isUseOtherRotateMat_ = true;
}

void Transform::SetOtherRotateMatrix(const Vector3& from, const Vector3& to, Type type)
{
	otherRotateMat_ = Matrix4x4::DirectionToDirection(from, to);
	isUseOtherRotateMat_ = true;
	otherRotType_ = type;
	rotMat_[type] = otherRotateMat_;
}

void Transform::SetOtherRotateMatrix(const Vector3& from, const Vector3& to, const Matrix4x4& multiplyMat, Type type)
{
	otherRotateMat_ = Matrix4x4::DirectionToDirection(from, to);
	otherRotateMat_ = otherRotateMat_ * multiplyMat;
	isUseOtherRotateMat_ = true;
	otherRotType_ = type;
	rotMat_[type] = otherRotateMat_;
}

const Matrix4x4 Transform::GetRotMat() const
{
	if (isUseOtherRotateMat_) {
		return otherRotateMat_;
	}
	else {
		return Matrix4x4::MakeRotateXYZMatrix(rotate_);
	}
	return Matrix4x4();
}

const Matrix4x4 Transform::GetRotMat(Type type) const
{
	return rotMat_[type];
}

void Transform::ClearOtherRotMat()
{
	isUseOtherRotateMat_ = false;

	float pi = std::numbers::pi_v<float>;

	if (otherRotType_ == X) {
		if (otherRotateMat_.m[1][2] >= 0) {
			rotate_.x = std::acosf(otherRotateMat_.m[1][1]);
		}
		else {
			rotate_.x = -std::acosf(otherRotateMat_.m[1][1]);
		}
	}
	else if (otherRotType_ == Y) {
		if (otherRotateMat_.m[2][0] >= 0) {
			rotate_.y = std::acosf(otherRotateMat_.m[0][0]);
		}
		else {
			rotate_.y = -std::acosf(otherRotateMat_.m[0][0]);
		}
	}
	else if (otherRotType_ == Z) {
		if (otherRotateMat_.m[0][2] >= 0) {
			rotate_.z = std::acosf(otherRotateMat_.m[0][0]);
		}
		else {
			rotate_.z = -std::acosf(otherRotateMat_.m[0][0]);
		}
	}

	otherRotType_ = Transform::All;
}
