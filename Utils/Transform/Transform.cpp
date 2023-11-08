#include "Transform.h"
#include "../Math/Vector2.h"

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

void Transform::SetOtherRotateMatrix(const Vector3& from, const Vector3& to)
{
	otherRotateMat_ = Matrix4x4::DirectionToDirection(from, to);
	isUseOtherRotateMat_ = true;
}

void Transform::SetOtherRotateMatrix(const Vector3& from, const Vector3& to, const Matrix4x4& multiplyMat)
{
	otherRotateMat_ = Matrix4x4::DirectionToDirection(from, to);
	otherRotateMat_ = otherRotateMat_ * multiplyMat;
	isUseOtherRotateMat_ = true;
}
