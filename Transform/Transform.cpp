#include "Transform.h"
#include "../Math/Vector2.h"

Transform::Transform()
{
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	translate_ = { 0.0f,0.0f,0.0f };

	worldMat_ = Matrix4x4::MakeAffinMatrix(*this);

	worldPos_.x = worldMat_.m[3][0];
	worldPos_.y = worldMat_.m[3][1];
	worldPos_.z = worldMat_.m[3][2];
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
